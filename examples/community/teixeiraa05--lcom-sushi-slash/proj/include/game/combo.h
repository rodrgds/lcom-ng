#ifndef __COMBO_H
#define __COMBO_H

#include "sprite.h"
#include "queue.h"
#include <lcom/lcf.h>

/**
 * @file combo.h
 * @brief Declares boss combo prompt runtime data.
 */

/**
 * @brief Key-sequence prompt used by boss encounters.
 */
typedef struct combo {
    /** @brief Queue of required key inputs for the combo. */
    Queue combo;

    /** @brief Sprite for the current letter displayed on screen. */
    sprite_t current_letter;

    /** @brief Horizontal coordinate where letters appear on screen. */
    uint16_t x;

    /** @brief Vertical coordinate where letters appear on screen. */
    uint16_t y;
    
    /** @brief Total number of letters in the combo. */
    uint8_t total_letters;

    /** @brief Number of letters already completed. */
    uint8_t completed;

    /** @brief Original string sequence of the combo. */
    char original_sequence[MAX_QUEUE_SIZE + 1];
    
} combo_t;

/**
 * @brief Initialize a combo prompt from an explicit key sequence.
 *
 * @param sequence Null-terminated sequence of required key characters.
 * @param x Initial horizontal coordinate for the current-key prompt.
 * @param y Initial vertical coordinate for the current-key prompt.
 * @return Initialized combo prompt.
 */
combo_t combo_init(const char* sequence, uint16_t x, uint16_t y);

/**
 * @brief Create a random uppercase-key combo prompt.
 *
 * @param length Number of letters to generate.
 * @param x Initial horizontal coordinate for the current-key prompt.
 * @param y Initial vertical coordinate for the current-key prompt.
 * @param prng_seed Pointer to the PRNG seed state.
 * @return Initialized random combo prompt.
 */
combo_t create_random_combo(uint8_t length, uint16_t x, uint16_t y, uint32_t *prng_seed);

/**
 * @brief Get the keyboard-key XPM image for a character.
 *
 * @param c Character to convert.
 * @return XPM image for the key, or NULL if unsupported.
 */
xpm_map_t char_to_keyboard_xpm(char c);

/**
 * @brief Advances the combo to the next letter and updates the display.
 *
 * @param combo Pointer to the combo to advance.
 * @return 0 if advanced successfully, 1 if combo is already complete.
 */
int combo_advance(combo_t* combo);

/**
 * @brief Checks if the combo is complete.
 *
 * @param combo Pointer to the combo to check.
 * @return 1 if combo is complete, 0 otherwise.
 */
int combo_is_complete(const combo_t* combo);

/**
 * @brief Gets the current letter required by the combo.
 *
 * @param combo Pointer to the combo.
 * @return The current letter, or 0 if combo is empty or complete.
 */
char combo_get_current(const combo_t* combo);

/**
 * @brief Move the displayed current-key prompt.
 *
 * @param combo Combo whose prompt position should change.
 * @param x New horizontal coordinate.
 * @param y New vertical coordinate.
 */
void combo_set_position(combo_t* combo, uint16_t x, uint16_t y);

/**
 * @brief Release resources owned by a combo prompt.
 *
 * @param combo Combo whose loaded letter sprite should be destroyed.
 */
void combo_destroy(combo_t* combo);

/**
 * @brief Reset the combo sequence progress back to the beginning.
 *
 * @param combo Combo to reset.
 */
void combo_reset(combo_t* combo);

#endif /* __COMBO_H */
