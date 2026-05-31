// LibertyOS Kernel Version: 0.5

#include <stdint.h>


volatile uint16_t* vga = (volatile uint16_t*)0xB8000;


int row = 0;
int col = 0;
int input_col = 0;

int shift = 0;
int ctrl = 0;

const char* kernel_ver = "v0.5";
const char* shell_ver = "v0.5";

//ASCII Table
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

//ASCII Table (Shift)
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


#define HISTORY_MAX 10
char history[HISTORY_MAX][256];
int history_len = 0;
int history_index = 0;
int history_max = HISTORY_MAX;


char cpu_vendor[13];
char cpu_brand[49];


static inline void outb(uint16_t port, uint8_t value) {  
    __asm__ volatile ("outb %0, %1" : : "a"(value), "Nd"(port));
}


void hide_hw_cursor() {
    outb(0x3D4, 0x0A);
    outb(0x3D5, 0x20);
}

// Shift Handler:
char scancode_to_char(uint8_t scancode) {
    if (shift) return scancode_table_shift[scancode];
    return scancode_table[scancode];
}


static inline uint8_t inb(uint16_t port) {
    uint8_t result;
    
    __asm__ volatile ("inb %1, %0" : "=a"(result) : "Nd"(port));
    return result;
}

void auto_scroll() {
    if (row >= 25) {
        for (int i = 0; i < 24; i++) {
            for (int k = 0; k < 80; k++) {
                vga[i * 80 + k] = vga[(i+1) * 80 + k];
            }
        }
        for (int k = 0; k < 80; k++) {
            vga[24 * 80 + k] = 0x0F20;
        }
        row = 24;
    }
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
            auto_scroll();
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
        auto_scroll();
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

// Clear Command Checker:
int str_eq(const char* a, const char* b) {
    for (int i = 0; a[i] || b[i]; i++){
        if (a[i] != b[i]) return 0;
    }
    return 1;
}

// Echo Command Checker:
int str_starts_with(const char* str, const char* prefix) {
    for (int i = 0; prefix[i] != '\0'; i++){
        if (str[i] != prefix[i]) return 0;
    }
    return 1;
}

// Colour Handler.
void print_str_color(const char* msg, uint8_t color) {
    for (int i = 0; msg[i] != '\0'; i++) {
        if (msg[i] == '\n') {
            row++;
            col = 0;
            auto_scroll();
            continue;
        }
        vga[(row * 80) + col] = (uint16_t)((color << 8) | (uint8_t)msg[i]);
        if (col >= 80) {
            row++;
            col = 0;
        }
        col++;
    }
}

// Get CPU model:
void get_cpu_vendor(char* vendor) {
    uint32_t ebx, ecx, edx;
    // leaf0:
    __asm__ volatile (
        "cpuid"
        : "=b"(ebx), "=c"(ecx), "=d"(edx)
        : "a"(0)
    );
    // ebx:
    vendor[0] = ebx & 0x0FF;
    vendor[1] = (ebx >> 8) & 0x0FF;
    vendor[2] = (ebx >> 16) & 0x0FF;
    vendor[3] = (ebx >> 24) & 0x0FF;
    // edx:
    vendor[4] = edx & 0x0FF;
    vendor[5] = (edx >> 8) & 0x0FF;
    vendor[6] = (edx >> 16) & 0x0FF;
    vendor[7] = (edx >> 24) & 0x0FF;
    // ecx:
    vendor[8] = ecx & 0x0FF;
    vendor[9] = (ecx >> 8) & 0x0FF;
    vendor[10] = (ecx >> 16) & 0x0FF;
    vendor[11] = (ecx >> 24) & 0x0FF;
    // End:
    vendor[12] = '\0';
}

// Get CPU Brand:
void get_cpu_brand(char* brand) {
    uint32_t eax, ebx, ecx, edx;
    // leaf1:
    __asm__ volatile (
        "cpuid"
        : "=a"(eax), "=b"(ebx), "=c"(ecx), "=d"(edx)
        : "a"(0x80000002)
    );
    // eax:
    brand[0] = eax & 0x0FF;
    brand[1] = (eax >> 8) & 0x0FF;
    brand[2] = (eax >> 16) & 0x0FF;
    brand[3] = (eax >> 24) & 0x0FF;
    // ebx:
    brand[4] = ebx & 0x0FF;
    brand[5] = (ebx >> 8) & 0x0FF;
    brand[6] = (ebx >> 16) & 0x0FF;
    brand[7] = (ebx >> 24) & 0x0FF;
    // ecx:
    brand[8] = ecx & 0x0FF;
    brand[9] = (ecx >> 8) & 0x0FF;
    brand[10] = (ecx >> 16) & 0x0FF;
    brand[11] = (ecx >> 24) & 0x0FF;
    // edx:
    brand[12] = edx & 0x0FF;
    brand[13] = (edx >> 8) & 0x0FF;
    brand[14] = (edx >> 16) & 0x0FF;
    brand[15] = (edx >> 24) & 0x0FF;
    
    // leaf2:
    __asm__ volatile (
        "cpuid"
        : "=a"(eax), "=b"(ebx), "=c"(ecx), "=d"(edx)
        : "a"(0x80000003)
    );
    // eax:
    brand[16] = eax & 0x0FF;
    brand[17] = (eax >> 8) & 0x0FF;
    brand[18] = (eax >> 16) & 0x0FF;
    brand[19] = (eax >> 24) & 0x0FF;
    // ebx:
    brand[20] = ebx & 0x0FF;
    brand[21] = (ebx >> 8) & 0x0FF;
    brand[22] = (ebx >> 16) & 0x0FF;
    brand[23] = (ebx >> 24) & 0x0FF;
    // ecx:
    brand[24] = ecx & 0x0FF;
    brand[25] = (ecx >> 8) & 0x0FF;
    brand[26] = (ecx >> 16) & 0x0FF;
    brand[27] = (ecx >> 24) & 0x0FF;
    //edx:
    brand[28] = edx & 0x0FF;
    brand[29] = (edx >> 8) & 0x0FF;
    brand[30] = (edx >> 16) & 0x0FF;
    brand[31] = (edx >> 24) & 0x0FF;
    
    // leaf3:
    __asm__ volatile (
        "cpuid"
        : "=a"(eax), "=b"(ebx), "=c"(ecx), "=d"(edx)
        : "a"(0x80000004)
    );
    // eax:
    brand[32] = eax & 0x0FF;
    brand[33] = (eax >> 8) & 0x0FF;
    brand[34] = (eax >> 16) & 0x0FF;
    brand[35] = (eax >> 24) & 0x0FF;
    // ebx:
    brand[36] = ebx & 0x0FF;
    brand[37] = (ebx >> 8) & 0x0FF;
    brand[38] = (ebx >> 16) & 0x0FF;
    brand[39] = (ebx >> 24) & 0x0FF;
    // ecx:
    brand[40] = ecx & 0x0FF;
    brand[41] = (ecx >> 8) & 0x0FF;
    brand[42] = (ecx >> 16) & 0x0FF;
    brand[43] = (ecx >> 24) & 0x0FF;
    // edx:
    brand[44] = edx & 0x0FF;
    brand[45] = (edx >> 8) & 0x0FF;
    brand[46] = (edx >> 16) & 0x0FF;
    brand[47] = (edx >> 24) & 0x0FF;
    // End:
    brand[48] = '\0';
}

// Get RAM Info:
uint32_t get_total_mem_mb(uint32_t* mb_info) {
    uint8_t* ptr = (uint8_t*)mb_info + 8;
    uint32_t total_size = mb_info[0];
    uint8_t* end = (uint8_t*)mb_info + total_size;
    uint32_t total_kb = 0;
    
    while (ptr < end) {
        uint32_t type = *(uint32_t*)ptr;
        uint32_t size = *(uint32_t*)(ptr + 4);
        
        if (type == 6) {
            uint8_t* entry = ptr + 16;
            while (entry < ptr + size) {
                uint32_t entry_type = *(uint32_t*)(entry + 16);
                uint32_t len_low = *(uint32_t*)(entry + 8);
                if (entry_type == 1) {
                    total_kb += len_low /1024;
                }
                entry += *(uint32_t*)(entry + 20) ? *(uint32_t*)(entry + 20) : 24;
            }
        }
        if (type == 0) break;
        ptr += (size + 7) & ~7;
    }
    return total_kb / 1024;
}

void print_uint(uint32_t n) {
    if (n == 0) {
        print_str("0");
    } else {
        char int_buffer[12];
        int len = 0;
        while (n > 0) {
            int_buffer[len++] = '0' + (n % 10);
            n /= 10;
        }
        for (int i = len - 1; i >= 0; i--) {
            vga[(row * 80) + col] = (uint16_t)((0x0F << 8) | int_buffer[i]);
            col++;
        }
    }
}


void kernel_main(uint32_t* mb_info) {
    int i = 0;
    int j = 0;
    int k = 0;

    int cursor_status = 0;
    
    get_cpu_vendor(cpu_vendor);
    get_cpu_brand(cpu_brand);

    // Clear Screen.
    for (i = 0; i < 80 * 25; i++) vga[i] = 0x0F20;
    
    hide_hw_cursor();
    
    const char msg[] = "========== LibertyOS: Kernel v0.5 (Pre-Alpha)  ==========";
    
    print_str(msg);
    print_str("\n\n\n>>> ");
    input_col = col;
    
    cursor_col = col;
    cursor_row = row;
    draw_cursor();
    
    
    // Main Loop:
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
            for (i = buf_cursor; i < buf_len; i++) {
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
        
        
        // Shift keys:
        if (key == 0x2A || key == 0x36) shift = 1;
        if (key == 0xAA || key == 0xB6) shift = 0;
        
        // Control keys:
        if (key == 0x1D || key == 0xE01D) ctrl = 1;
        if (key == 0x9D || key == 0xE09D) ctrl = 0;
        
        // Control Handler:
        if (ctrl && key == 0x2E) {
            erase_cursor();
            buf_len = 0;
            buf_cursor = 0;
            input_buf[0] = '\0';
            print_str("\n^C\n>>> ");
            input_col = col;
            cursor_col = col;
            cursor_row = row;
            draw_cursor();
            cursor_status = 1;
            continue;
        }
        
        
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
        
        
        // Up + Dn Arrows:
        if (key == 0xE048) {
            erase_cursor();
            if (history_index > 0) {
                history_index--;
                i = 0;
                for (i = 0; history[history_index][i] != '\0'; i++) {
                    input_buf[i] = history[history_index][i];
                }
                input_buf[i] = '\0';
                buf_len = i;
                buf_cursor = buf_len;
                render_input();
                col = input_col + buf_cursor;
                cursor_col = col;
                cursor_row = row;
                draw_cursor();
            }
            continue;
        }
        if (key == 0xE050) {
            erase_cursor();
            if (history_index < history_len) {
                history_index++;
                i = 0;
                for (i = 0; history[history_index][i] != '\0'; i++) {
                    input_buf[i] = history[history_index][i];
                }
                input_buf[i] = '\0';
                buf_len = i;
                buf_cursor = buf_len;
                render_input();
                col = input_col + buf_cursor;
                cursor_col = col;
                cursor_row = row;
                draw_cursor();
            } else {
                buf_len = 0;
                buf_cursor = 0;
                input_buf[0] = '\0';
                render_input();
                col = input_col;
                cursor_col = col;
                cursor_row = row;
                draw_cursor();
            }
            continue;
        }
        
        
        if (key >= 0x80 || c == 0) continue;
        
        
        // When Return key is pressed.
        if (c == '\n') {
            // Command Handler:
            erase_cursor();
            input_buf[buf_len] = '\0';
            // Clear Command:
            if (str_eq(input_buf, "clear")) {
                row = 0; col = 0;
                for (i = 0; i < 80 * 25; i++) vga[i] = 0x0F20;
                
                print_str(">>> ");
            // Echo Command:
            } else if (str_starts_with(input_buf, "echo ")){
                print_str("\n");
                print_str(input_buf + 5);
                print_str("\n>>> ");
            // Reboot Command:
            } else if (str_eq(input_buf, "reboot")) {
                outb(0x64, 0xFE);
            //Fast Fetch Command + ASCII Art:
            } else if (str_eq(input_buf, "ff") || str_eq(input_buf, "fastfetch")) {
                print_str("\n");
                print_str_color("        ___deeply___        ", 0x0B);
                print_str_color("    LibertyOS", 0x0F); print_str("\n");
                print_str_color("       /  flame  \\      ", 0x0E);
                print_str_color("    ---------", 0x08); print_str("\n");
                print_str_color("      | torch |        ", 0x0B);
                print_str_color("OS: ", 0x0A); print_str_color("LibertyOS Pre-Alpha", 0x0F); print_str("\n");
                print_str_color("      |  /\\  |         ", 0x0B);
                print_str_color("Kernel: ", 0x0A); print_str_color(kernel_ver, 0x0F); print_str("\n");
                print_str_color("    __|_/  \\_|__       ", 0x0B);
                print_str_color("Arch: ", 0x0A); print_str_color("x86 32-bit", 0x0F); print_str("\n");
                print_str_color("   |  Liberty   |      ", 0x0B);
                print_str_color("Shell: ", 0x0A); print_str_color("LibertyShell ", 0x0F); print_str_color(shell_ver, 0x0F); print_str("\n");
                print_str_color("   |    face    |      ", 0x0B);
                print_str_color("Build: ", 0x0A); print_str_color("Pre-Alpha", 0x0F); print_str("\n");
                print_str_color("   |___________|       ", 0x0B);
                print_str_color("CPU: ", 0x0A); print_str_color(cpu_vendor, 0x0F); print_str("\n");
                print_str_color("        |||            ", 0x0B);
                print_str_color("CPU Brand: ", 0x0A); print_str_color(cpu_brand, 0x0F); print_str("\n");
                print_str_color("       =====           ", 0x07);
                print_str_color("Memory: ", 0x0A);
                print_uint(get_total_mem_mb(mb_info));
                print_str_color(" MB\n", 0x0F);
                print_str("\n>>> ");
            } else {
                print_str("\nCommand not found! Please check your spelling and try again.\n>>> ");
            }
            
            // Save the Command to History Logic:
            if (history_len < history_max) {
                for (i = 0; input_buf[i] != '\0'; i++) {
                    history[history_len][i] = input_buf[i];
                }
                history[history_len][i] = '\0';
                history_len++;
                history_index = history_len;
            } else {
                for (i = 0; i < history_max -1; i++) {
                    for (k = 0; history[i+1][k] != '\0'; k++) {
                        history[i][k] = history[i+1][k];
                    }
                    history[i][k] = '\0';
                }
                for (k = 0; input_buf[k] != '\0'; k++) {
                    history[history_max -1][k] = input_buf[k];
                }
                history[history_max -1][k] = '\0';
                history_index = history_max;
            }
            buf_len = 0;
            buf_cursor = 0;
            input_col = col;
            
        } else {
            erase_cursor();
            print_char(c);
        }
        

      
        // Draw the Cursor.
        cursor_col = col;
        cursor_row = row;
        cursor_status =1; draw_cursor();        
    }
}