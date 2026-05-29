#include <stdint.h>


volatile uint16_t* vga = (volatile uint16_t*)0xB8000;

int row = 0;
int col = 0;
int input_col = 0;
int shift = 0;

const char scancode_table[128] = {
    [0x01]=0,     // Escape.
    [0x02]='1', [0x03]='2', [0x04]='3', [0x05]='4',
    [0x06]='5', [0x07]='6', [0x08]='7', [0x09]='8',
    [0x0A]='9', [0x0B]='0', [0x0C]='-', [0x0D]='=',
    [0x0E]=0,     // Backspace (handle separately later).
    [0x0F]='\t',  // Tab.
    [0x10]='q', [0x11]='w', [0x12]='e', [0x13]='r',
    [0x14]='t', [0x15]='y', [0x16]='u', [0x17]='i',
    [0x18]='o', [0x19]='p', [0x1A]='[', [0x1B]=']',
    [0x1C]='\n',  // Enter / Return.
    [0x1E]='a', [0x1F]='s', [0x20]='d', [0x21]='f',
    [0x22]='g', [0x23]='h', [0x24]='j', [0x25]='k',
    [0x26]='l', [0x27]=';', [0x28]='\'', [0x29]='`',
    [0x2B]='\\',
    [0x2C]='z', [0x2D]='x', [0x2E]='c', [0x2F]='v',
    [0x30]='b', [0x31]='n', [0x32]='m', [0x33]=',',
    [0x34]='.', [0x35]='/', [0x39]=' ' // Space.
};

const char scancode_table_shift[128] = {
    [0x02]='!', [0x03]='@', [0x04]='#', [0x05]='$',
    [0x06]='%', [0x07]='^', [0x08]='&', [0x09]='*',
    [0x0A]='(', [0x0B]=')', [0x0C]='_', [0x0D]='+',
    [0x10]='Q', [0x11]='W', [0x12]='E', [0x13]='R',
    [0x14]='T', [0x15]='Y', [0x16]='U', [0x17]='I',
    [0x18]='O', [0x19]='P', [0x1A]='{', [0x1B]='}',
    [0x1C]='\n', //Enter / Return.
    [0x1E]='A', [0x1F]='S', [0x20]='D', [0x21]='F',
    [0x22]='G', [0x23]='H', [0x24]='J', [0x25]='K',
    [0x26]='L', [0x27]=':', [0x28]='"', [0x29]='~',
    [0x2B]='|',
    [0x2C]='Z', [0x2D]='X', [0x2E]='C', [0x2F]='V',
    [0x30]='B', [0x31]='N', [0x32]='M', [0x33]='<',
    [0x34]='>', [0x35]='?', [0x39]=' '
};

char input_buf[256];
int buf_len = 0;
int buf_cursor = 0;

int cursor_col = 0;
int cursor_row = 0;


static inline void outb(uint16_t port, uint8_t value) {  
    __asm__ volatile ("outb %0, %1" : : "a"(value), "Nd"(port));
}


void hide_hw_cursor() {
    outb(0x3D4, 0x0A);
    outb(0x3D5, 0x20);
}


char scancode_to_char(uint8_t scancode) {
    if (shift) return scancode_table_shift[scancode];
    return scancode_table[scancode];
}

static inline uint8_t inb(uint16_t port) {
    uint8_t result;
    
    __asm__ volatile ("inb %1, %0" : "=a"(result) : "Nd"(port));
    return result;
}

uint16_t user_input() {
    if ((inb(0x64) &1) ==1) {
        uint8_t input = inb(0x60);
        if (input == 0xE0) {
            while ((inb(0x64) &1) !=1) {}
            uint16_t input1 = inb(0x60);
            return (0xE0 << 8) | input1;
        }   
        return input;
    } else {return 0;}
}

void print_str(const char* msg) {
    for (int i = 0; msg[i] != '\0'; i++) {
        if (msg[i] == '\n') {
            row++;
            col = 0;
            continue;
        }
        vga[(row * 80) + col] = (uint16_t)((0x0F << 8) | (uint8_t)msg[i]);
        if (col >= 80) {
            row++;
            col = 0;
        }
        col++;
    }
}

void insert_char(char c) {
    for (int i = buf_len; i >= buf_cursor; i--) {
        input_buf[i+1] = input_buf[i];
    }
    input_buf[buf_cursor] = c;
    buf_cursor++;
    buf_len++;
}

void render_input() {
    col = input_col;
    for (int i = 0; i < buf_len; i++) {
        vga[(row*80) + col] = (uint16_t)((0x0F << 8) | (uint8_t) input_buf[i]);
        col++;
    }
    while (col < (input_col + 76)) {
        vga[(row*80) +col] = (uint16_t)((0x0F << 8) | ' ');
        col++;
    }
}

void print_char(char c) {
    if (c == '\n') {
        row++;
        col=0;
        return;
    }
    insert_char(c);
    render_input();
    col = input_col + buf_cursor;
}

void draw_cursor() {
    uint16_t cell = vga[(cursor_row * 80) +cursor_col];
    uint8_t ch = cell & 0xFF;
    vga[(cursor_row * 80) + cursor_col] = (uint16_t)((0xF0 << 8) | ch);
}

void erase_cursor() {
    uint16_t cell = vga[(cursor_row * 80) +cursor_col];
    uint8_t ch = cell & 0xFF;
    vga[(cursor_row * 80) + cursor_col] = (uint16_t)((0x0F << 8) | ch);
}

void kernel_main() {
    int j = 0;
    int cursor_status = 0;

    for (int i = 0; i < 80 * 25; i++) vga[i] = 0x0F20;
    
    hide_hw_cursor();
    
    const char msg[] = "========== LibertyOS Version: Pre-Alpha ==========";
    
    print_str(msg);
    print_str("\n\n\n>>> ");
    input_col = col;
    
    cursor_col = col;
    cursor_row = row;
    draw_cursor();
    
    
    while (1) {
        uint16_t key = user_input();
        char c = scancode_to_char(key);
       
        
        // Cursor Blink.
        j++;
        if (j >= 4000000) {
            if (cursor_status == 0) {
                cursor_status = 1;
                draw_cursor();
            } else {
                cursor_status = 0;
                erase_cursor();
            }
            j = 0;
        }     
        
        // Backspace.
        if (key == 0x0E && buf_cursor > 0) {
            erase_cursor();
            buf_cursor--;
            for (int i = buf_cursor; i < buf_len; i++) {
               input_buf[i] = input_buf[i+1]; 
            }
            buf_len--;
            render_input();
            col = input_col + buf_cursor;
            cursor_col = col;
            cursor_row = row;
            draw_cursor();
            cursor_status = 1;
            continue;
        } 
        if (key == 0x0E && buf_cursor == 0){
            // Add bell.
            continue;
        }
        
        
        if (key == 0x2A || key == 0x36) shift = 1;
        if (key == 0xAA || key == 0xB6) shift = 0;
        
        
        // Left + Right Arrow Keys.
        if (key == 0xE04B && buf_cursor > 0) {
            erase_cursor();
            buf_cursor--;
            col = input_col + buf_cursor;
            cursor_col = col;
            cursor_row = row;
            draw_cursor();
            continue;
        }
        if (key == 0xE04D && buf_cursor < buf_len) {
            erase_cursor();
            buf_cursor++;
            col = input_col + buf_cursor;
            cursor_col = col;
            cursor_row = row;
            draw_cursor();
            continue;
        }     
        
        // Home + End Keys.
        if (key == 0xE047) {
            erase_cursor();
            buf_cursor = 0;
            col = input_col;
            cursor_col = col;
            cursor_row = row;
            draw_cursor();
            continue;
        }
        if (key == 0xE04F) {
            erase_cursor();
            buf_cursor = buf_len;
            col = input_col + buf_len;
            cursor_col = col;
            cursor_row = row;
            draw_cursor();
            continue;
        }
        
        
        if (key >= 0x80 || c == 0) continue;
        
        erase_cursor();
        print_char(c);
        
        // When Return key is pressed.
        if (c == '\n') {
        buf_len = 0;
        buf_cursor = 0;
        print_str(">>> ");
        input_col = col;
        }
      
        // Draw the Cursor.
        cursor_col = col;
        cursor_row = row;
        cursor_status =1; draw_cursor();
        
    }
}