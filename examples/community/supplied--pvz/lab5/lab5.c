// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <lcom/lab5.h>

#include <stdint.h>
#include <stdio.h>

#include "video.h"
#include "keyboard.h"
#include "i8042.h"

int main(int argc, char *argv[]) {
    // sets the language of LCF messages (can be either EN-US or PT-PT)
    lcf_set_language("EN-US");

    // enables to log function invocations that are being "wrapped" by LCF
    // [comment this out if you don't want/need it]
    lcf_trace_calls("/home/lcom/labs/lab5/trace.txt");

    // enables to save the output of printf function calls on a file
    // [comment this out if you don't want/need it]
    lcf_log_output("/home/lcom/labs/lab5/output.txt");

    // handles control over to LCF
    // [LCF handles command line arguments and invokes the right function]
    if (lcf_start(argc, argv))
        return 1;

    // LCF clean up tasks
    // [must be the last statement before return]
    lcf_cleanup();

    return 0;
}

int(video_test_init)(uint16_t mode, uint8_t delay) {
    if (vg_init(mode) == NULL) {
        vg_exit();
        return 1;
    }

    tickdelay(micros_to_ticks(delay * 1000000));

    if (vg_exit() != 0) return 1;

    return 0;
}

int(video_test_rectangle)(uint16_t mode, uint16_t x, uint16_t y,
                          uint16_t width, uint16_t height, uint32_t color) {
    // Map VRAM and set video mode  
    if (vg_init(mode) == NULL) {
        vg_exit();
        return 1;
    }

    // Draw the rectangle
    if (vg_draw_rectangle(x, y, width, height, color) != 0) {
        vg_exit();
        return 1;
    }

    // Subscribe keyboard interrupts 
    uint8_t kbd_bit;
    if (kbd_subscribe_int(&kbd_bit) != 0) {
        vg_exit();
        return 1;
    }

    // Wait for ESC breakcode 
    int ipc_status;
    message msg;
    uint32_t kbd_irq_set = BIT(kbd_bit);

    while (get_scancode() != ESC_BREAKCODE) {
        if (driver_receive(ANY, &msg, &ipc_status) != 0) continue;

        if (is_ipc_notify(ipc_status) && _ENDPOINT_P(msg.m_source) == HARDWARE) {
            if (msg.m_notify.interrupts & kbd_irq_set) {
                kbc_ih();
            }
        }
    }

    // Unsubscribe keyboard interrupts 
    if (kbd_unsubscribe_int() != 0) {
        vg_exit();
        return 1;
    }

    // Return to text mode 
    if (vg_exit() != 0) return 1;

    return 0;
}

int(video_test_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y) {
    // Switch to VBE mode 0x105 (indexed color, 1024x768 @ 8bpp)
    if (vg_init(0x105) == NULL) {
        vg_exit();
        return 1;
    }

    // Draw the XPM pixmap at (x, y)
    if (vg_draw_xpm(xpm, XPM_INDEXED, x, y) != 0) {
        vg_exit();
        return 1;
    }

    // Subscribe keyboard interrupts
    uint8_t kbd_bit;

    if (kbd_subscribe_int(&kbd_bit) != 0) {
        vg_exit();
        return 1;
    }

    // Wait for ESC breakcode
    int ipc_status;
    message msg;
    uint32_t kbd_irq_set = BIT(kbd_bit);

    while (get_scancode() != ESC_BREAKCODE) {
        if (driver_receive(ANY, &msg, &ipc_status) != 0) continue;
        if (is_ipc_notify(ipc_status) && _ENDPOINT_P(msg.m_source) == HARDWARE) {
            if (msg.m_notify.interrupts & kbd_irq_set) {
                kbc_ih();
            }
        }
    }

    // Unsubscribe keyboard interrupts
    if (kbd_unsubscribe_int() != 0) {
        vg_exit();
        return 1;
    }

    // Restore text mode
    if (vg_exit() != 0) return 1;

    return 0;
}
