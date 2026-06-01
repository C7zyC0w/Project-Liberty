/*
 * Project-Liberty/LibertyOS/kernel/kernel.c
 *
 * Copyright: C7zyC0w
 */

//#include "kernel.h"
#include "vga.h"
#include "keyboard.h"
#include "shell.h"
#include "cpu.h"
#include "memory.h"

#include <stdint.h>


const char* kernel_ver = "v0.6";
const char* shell_ver = "v0.5";


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

    print_str("========== LibertyOS: Kernel "); print_str(kernel_ver); print_str(" (Pre-Alpha)  ==========");
    print_str("\n              Shell Version: "); print_str(shell_ver);
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
