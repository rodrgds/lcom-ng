// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <lcom/lab5.h>

#include <stdint.h>
#include <stdio.h>

// Any header files included below this line should have been created by you
#include "v_card.h"
#include "keyboard.h"

#define ESC_BREAKCODE 0x81

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
    //initialize video graphics and map memory
    if (vg_init(mode) == NULL) {
        return 1;
    }

    sleep(delay); //wait *delay* seconds

    if (vg_exit() != OK) {   //return to text mode
        return 1;
    }

    return 0;
}

int(video_test_rectangle)(uint16_t mode, uint16_t x, uint16_t y,
                          uint16_t width, uint16_t height, uint32_t color) {
    
    // set the video mode and map memory
    if (vg_init(mode) == NULL) {
        return 1;
    }

    //draw the rectangle
    if (vg_draw_rectangle(x, y, width, height, color) != 0) {
        vg_exit();
        return 1;
    }

    //wait for the esc key
    int ipc_status, r;
    message msg;
    uint8_t bit_no;

    if (kbd_subscribe_int(&bit_no) != 0) return 1;
    uint32_t irq_set = BIT(bit_no);

    while (kbd_get_scancode() != ESC_BREAKCODE) {
        if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) { 
            printf("driver_receive failed with: %d", r);
            continue;
        }
        if (is_ipc_notify(ipc_status)) {
            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE: 
                    if (msg.m_notify.interrupts & irq_set) {
                        kbc_ih();
                    }
                    break;
                default:
                    break;
            }
        }
    }

    if (kbd_unsubscribe_int() != 0) return 1;

    //reset video mode back to text
    if (vg_exit() != OK) {
        return 1;
    }

    return 0;
}

int(video_test_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y) {
    xpm_image_t img;
    uint8_t *pixmap;
    
    pixmap = xpm_load(xpm, XPM_INDEXED, &img);
    if (pixmap == NULL) {
        printf("Failed to load XPM image.\n");
        return 1;
    }

    if (vg_init(0x105) == NULL) {
        printf("Failed to initialize VBE mode 0x105.\n");
        return 1;
    }

    if (vg_draw_xpm(img, pixmap, x, y) != 0) {
        vg_exit();
        return 1;
    }

    int ipc_status, r;
    message msg;
    uint8_t bit_no;

    if (kbd_subscribe_int(&bit_no) != 0) {
        vg_exit();
        return 1;
    }
    uint32_t irq_set = BIT(bit_no);

    while (kbd_get_scancode() != ESC_BREAKCODE) {
        if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) { 
            printf("driver_receive failed with: %d", r);
            continue;
        }
        if (is_ipc_notify(ipc_status)) {
            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE: 
                    if (msg.m_notify.interrupts & irq_set) {
                        kbc_ih();
                    }
                    break;
                default:
                    break;
            }
        }
    }

    if (kbd_unsubscribe_int() != 0) {
        vg_exit();
        return 1;
    }

    if (vg_exit() != OK) {
        return 1;
    }

    return 0;
}
