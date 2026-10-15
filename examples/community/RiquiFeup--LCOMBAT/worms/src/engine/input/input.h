#ifndef INPUT_H
#define INPUT_H

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief Game-level key actions, abstracted from physical scancodes.
 */
typedef enum {
    KEY_NONE = 0,
    KEY_UP,
    KEY_DOWN,
    KEY_LEFT,
    KEY_RIGHT,
    KEY_ENTER,
    KEY_SPACE,
    KEY_ESC,
    KEY_1,
    KEY_2,
    KEY_Q,
    KEY_COUNT
} InputKey;

/**
 * @brief Subscribes keyboard/mouse interrupts and enables mouse data reporting.
 * @param kbd_bit   Filled with the keyboard IRQ bitmask.
 * @param mouse_bit Filled with the mouse IRQ bitmask.
 * @return 0 on success, 1 on failure.
 */
int input_init(uint8_t *kbd_bit, uint8_t *mouse_bit);

/** 
 * @brief Disables mouse data reporting and unsubscribes both interrupts.
 */
void input_cleanup(void);

/** 
 * @brief Reads a keyboard scancode. Call when the keyboard IRQ fires.
 */
void input_handle_keyboard(void);

/** 
 * @brief Reads a mouse packet. Call when the mouse IRQ fires.
 */
void input_handle_mouse(void);

/**
 * @brief Returns true if the given key is currently held down.
 *
 * Multiple keys can be held simultaneously; each is tracked independently.
 * KEY_NONE and KEY_COUNT always return false.
 *
 * @param key Key to query.
 * @return true if the key is pressed, false otherwise.
 */
bool input_is_key_pressed(InputKey key);

/**
 * @brief Returns a non-zero seed derived from the current RTC time.
 *
 * Hardware access stays inside the engine/input layer so game modules do not
 * include RTC driver headers directly.
 *
 * @return Non-zero seed suitable for srand().
 */
uint32_t input_get_rtc_seed(void);

/** 
 * @brief Returns the current mouse X, clamped to [0, screen_width-1]. 
 */
int16_t input_get_mouse_x(void);

/** 
 * @brief Returns the current mouse Y, clamped to [0, screen_height-1]. 
 */
int16_t input_get_mouse_y(void);

/** 
 * @brief Returns true if the left mouse button is pressed.
 */
bool input_is_mouse_lb(void);

/** 
 * @brief Returns true if the right mouse button is pressed.
 */
bool input_is_mouse_rb(void);

#endif /* INPUT_H */
