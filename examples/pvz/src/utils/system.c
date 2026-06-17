#include <timer.h>
#include <keyboard.h>
#include <mouse.h>
#include "video.h"
#include <stdio.h>

#include "constants.h"
#include "system.h"

int init_systems(uint8_t *timer_bit, uint8_t *kbd_bit, uint8_t *mouse_bit) {
    // 1. Initialize video mode (800x600 in 24-bit color)
    if (vg_init(0x115) == NULL) {
        printf("Failed to init video!\n");
        return 1;
    }

    // 2. Subscribe to interrupts
    if (timer_subscribe_int(timer_bit) != 0) return 1;
    if (kbd_subscribe_int(kbd_bit) != 0) return 1;
    if (mouse_subscribe_int(mouse_bit) != 0) return 1;

    // 3. Enable mouse data reporting
    if (mouse_write_command(EN_DATA_REPORT) != 0) {
        printf("Failed to enable mouse data reporting!\n");
        timer_unsubscribe_int();
        kbd_unsubscribe_int();
        mouse_unsubscribe_int();
        vg_cleanup();
        vg_exit();
        return 1;
    }

    return 0;
}

int cleanup_systems(void) {
    mouse_write_command(DIS_DATA_REPORT);
    mouse_unsubscribe_int();
    kbd_unsubscribe_int();
    timer_unsubscribe_int();
    vg_cleanup();
    vg_exit();
    return 0;
}
