#include "input.h"

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "keyboard.h"
#include "i8042.h"
#include "mouse.h"
#include "graphics.h"
#include "rtc.h"

static bool key_states[KEY_COUNT];  /* true = currently held; zero-initialized */
static int16_t mouse_x = 0;
static int16_t mouse_y = 0;
static bool mouse_lb = false;
static bool mouse_rb = false;
static int16_t screen_w = 0;
static int16_t screen_h = 0;

static bool is_two_bytes = false;
static uint8_t sc_bytes[2] = {0, 0};
static uint8_t sc_size = 0;

#define SC_ESC 0x01
#define SC_1 0x02
#define SC_2 0x03
#define SC_Q 0x10
#define SC_ENTER 0x1C
#define SC_SPACE 0x39
#define SC_UP 0x48
#define SC_DOWN 0x50
#define SC_LEFT 0x4B
#define SC_RIGHT 0x4D
#define RTC_SEED_YEAR_SHIFT 24
#define RTC_SEED_MONTH_SHIFT 20
#define RTC_SEED_DAY_SHIFT 16
#define RTC_SEED_HOURS_SHIFT 12
#define RTC_SEED_MINUTES_SHIFT 6

// Maps a scancode to an InputKey
static InputKey scancode_to_key(bool two_bytes, uint8_t sc){
    if (two_bytes){
        switch (sc){
            case SC_UP: return KEY_UP;
            case SC_DOWN: return KEY_DOWN;
            case SC_LEFT: return KEY_LEFT;
            case SC_RIGHT: return KEY_RIGHT;
            default: return KEY_NONE;
        }
    }
    switch (sc){
        case SC_ESC: return KEY_ESC;
        case SC_Q: return KEY_Q;
        case SC_ENTER: return KEY_ENTER;
        case SC_SPACE: return KEY_SPACE;
        case SC_1: return KEY_1;
        case SC_2: return KEY_2;
        default: return KEY_NONE;
    }
}

int input_init(uint8_t *kbd_bit, uint8_t *mouse_bit){
    if (kbd_bit == NULL || mouse_bit == NULL) return 1;

    if (mouse_enable_data_reporting() != 0){
        return 1;
    }

    if (kbd_subscribe_int(kbd_bit) != 0) {
        mouse_write_cmd(MOUSE_DIS_DATA);
        return 1;
    }

    if (mouse_subscribe_int(mouse_bit) != 0){
        kbd_unsubscribe_int();
        mouse_write_cmd(MOUSE_DIS_DATA);
        return 1;
    }
    screen_w = (int16_t)get_h_res();
    screen_h = (int16_t)get_v_res();
    mouse_x = screen_w / 2;
    mouse_y = screen_h / 2;
    return 0;
}

void input_cleanup(void){
    kbd_unsubscribe_int();
    mouse_unsubscribe_int();
    mouse_write_cmd(MOUSE_DIS_DATA);
}

void input_handle_keyboard(void){
    bool make = false;
    bool esc_pressed = false;

    kbc_ih();

    if (!kbc_process_scancode(&is_two_bytes, &sc_size, sc_bytes, &make, &esc_pressed)) return;

    uint8_t sc = ((sc_size == 2) ? sc_bytes[1] : sc_bytes[0]) & 0x7F;
    InputKey key = scancode_to_key(sc_size == 2, sc);
    if (key == KEY_NONE) return;

    key_states[key] = make;   /* true on press, false on release */
}

void input_handle_mouse(void){
    mouse_ih();
    if (!mouse_sync_and_build_packet()) return;

    struct packet *p = mouse_get_packet();

    // delta_x: positive = right, negative = left (matches screen X, so we add)
    // delta_y: positive = up, negative = down (opposite to screen Y which grows downward, so we subtract)
    mouse_x += (int16_t)p->delta_x;
    mouse_y -= (int16_t)p->delta_y;
    
    if (mouse_x < 0) mouse_x = 0;
    if (mouse_x >= screen_w) mouse_x = screen_w - 1;
    if (mouse_y < 0) mouse_y = 0;
    if (mouse_y >= screen_h) mouse_y = screen_h - 1;

    mouse_lb = p->lb;
    mouse_rb = p->rb;
}

bool input_is_key_pressed(InputKey key){
    if (key == KEY_NONE || key >= KEY_COUNT) return false;
    return key_states[key];
}

uint32_t input_get_rtc_seed(void){
    rtc_setup();

    const real_time_info *time = rtc_get_time();
    if (time == NULL) return 1;

    uint32_t seed = ((uint32_t) time->year << RTC_SEED_YEAR_SHIFT) |
                    ((uint32_t) time->month << RTC_SEED_MONTH_SHIFT) |
                    ((uint32_t) time->day << RTC_SEED_DAY_SHIFT) |
                    ((uint32_t) time->hours << RTC_SEED_HOURS_SHIFT) |
                    ((uint32_t) time->minutes << RTC_SEED_MINUTES_SHIFT) |
                    (uint32_t) time->seconds;

    return (seed == 0) ? 1 : seed;
}

int16_t input_get_mouse_x(void){
    return mouse_x;
}

int16_t input_get_mouse_y(void){
    return mouse_y;
}

bool input_is_mouse_lb(void){
    return mouse_lb;
}

bool input_is_mouse_rb(void){
    return mouse_rb;
}
