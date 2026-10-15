#include <lcom/lcf.h>

#include <lcom/lab5.h>

#include "../lab3/kbc.h"
#include "spriteXMP.h"
#include "terminal.h"

#include <stdint.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
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

int drawRectangleTest(Terminal* terminal, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {
    for (int i = y; i < y + height; i++) drawLine(terminal, x, i, width, color);
    return 0;
}

int(video_test_init)(uint16_t mode, uint8_t delay) {
    enterMode(mode);
    tickdelay(micros_to_ticks(delay * 1000000));
    vg_exit();
    return 0;
}

int(video_test_rectangle)(uint16_t mode, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {
    Terminal* terminal = newTerminal(mode);
    drawRectangleTest(terminal, x, y, width, height, color);

    uint8_t bit_no = 1;
    int hook_id = 1;

    kbcSubscribe((uint8_t*)&hook_id);
    message msg;
    int r, ipc_status;

    printf("Finished drawing\n");

    while (_getLastByte() != 0x81) {
        if ((r = driver_receive(ANY, &msg, &ipc_status))) {
            printf("Cannot read driver %x\n", r);
            continue;
        }

        if (!is_ipc_notify(ipc_status)) continue;
        switch (_ENDPOINT_P(msg.m_source)) {
            case HARDWARE:
                if (msg.m_notify.interrupts & (1 << bit_no)) {
                    kbcIh();
                    if (kbcIsKeyReady()) kbcResetKeys();
                }

                break;
            default:
                break;
        }
    }

	freeTerminal(terminal);
    kbcUnsubscribe();
    return 0;
}

int(video_test_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y) {
    Terminal* terminal = newTerminal(0x105);
    enum xpm_image_type type = XPM_INDEXED;
    SpriteXPM* sprite = newSpriteXPM(xpm, type, 0, 0);

    drawSpriteXPM(sprite, terminal, x, y, false);
    uint8_t bit_no = 1;
    int hook_id = 1;

    kbcSubscribe((uint8_t*)&hook_id);
    message msg;
    int r, ipc_status;

    while (_getLastByte() != 0x81) {
        if ((r = driver_receive(ANY, &msg, &ipc_status))) {
            printf("Cannot read driver %x\n", r);
            continue;
        }

        if (!is_ipc_notify(ipc_status)) continue;
        switch (_ENDPOINT_P(msg.m_source)) {
            case HARDWARE:
                if (msg.m_notify.interrupts & (1 << bit_no)) {
                    kbcIh();
                    if (kbcIsKeyReady()) kbcResetKeys();
                }

                break;
            default:
                break;
        }
    }

    kbcUnsubscribe();
    freeSpriteXPM(sprite);
    freeTerminal(terminal);
    return 0;
}
