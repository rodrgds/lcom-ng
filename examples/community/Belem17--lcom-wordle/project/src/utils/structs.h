#ifndef STRUCTS_H
#define STRUCTS_H

#include <stdbool.h>
#include <stdint.h>
#include "frames.h"

/// @brief Game state definitions.
enum State {
    STATE_MENU = 0,
    STATE_GAME = 1,
    STATE_WIN  = 2,
    STATE_LOSE = 3,
    STATE_END  = -1
};

/// @brief Structure to store signal inputs (keyboard and mouse).
struct Signal {
    bool mouse_pressed; ///< True if mouse left button was pressed
    bool key_pressed;   ///< True if a key was pressed
    int key_code;       ///< The scancode of the key pressed
};

/// @brief Structure representing 2D coordinates.
struct Coordenates {
    int x; ///< X coordinate
    int y; ///< Y coordinate
};

/// @brief Structure holding the mouse's current coordinates.
struct mouseCoord {
    int mouse_x; ///< Mouse X position
    int mouse_y; ///< Mouse Y position
};

#define MAX_ATTEMPTS 6   ///< Maximum number of attempts allowed in the game
#define MAX_WORD_LEN 6   ///< Maximum length of a word (including null terminator)

/**
 * @brief Structure representing the entire state of the game.
 */
struct Game {
    char attempts[MAX_ATTEMPTS][MAX_WORD_LEN]; ///< Array storing guessed words
    int traces[MAX_ATTEMPTS][MAX_WORD_LEN - 1]; ///< Used to store evaluation (e.g., correctness) of letters
    int current_attempt;  ///< Index of the current attempt
    char word[MAX_WORD_LEN]; ///< Word currently being typed
    char target[MAX_WORD_LEN]; ///< Target word (correct answer)
    char* target_word; ///< Dynamically loaded target word
    char** word_list;  ///< List of all possible target words
    struct mouseCoord mouseC; ///< Current mouse coordinates
    struct mouseCoord oldMouseC; ///< Previous mouse coordinates
    struct Signal signal; ///< Input signals (keyboard/mouse)
    int state; ///< Current game state (menu, game, win, etc.)
    uint8_t *map; ///< Optional background map
    bool submitted; ///< Whether the current word has been submitted
    bool game_needs_init;     
    enum State previous_state; ///< Previous game state (for transition detection)

};

/**
 * @brief Structure holding all graphical resources (sprites).
 */
typedef struct {
    Frame * Alphabet[26]; ///< Array of frames for A-Z
    Frame * Title;        ///< Title frame
    Frame * Mouse;        ///< Mouse cursor frame
    Frame * startButton;  ///< Start button frame
    Frame * winscreen;    ///< Win screen frame
    Frame * losescreen;   ///< Lose screen frame
    Frame * grid;         ///< Game grid frame
} Resources;

/**
 * @brief Initializes all global graphical resources.
 */
void initialize();

#endif
