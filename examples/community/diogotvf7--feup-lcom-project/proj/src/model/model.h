#pragma once
#include <lcom/lcf.h>
#include "sprite.h"
#include "controller/mouse/mouse.h"
#include "controller/rtc/rtc.h"
#include "controller/keyboard/keyboard.h"
#include "controller/graphics_card/graphics_card.h"
#include "controller/uart/ser_port.h"
#include "model/queue.h"
#include "controller/timer/timer.h"
#include "view/view.h"
#include "model/base_frame.h"
#include "global_vars.h"
#include "colors.h"

/*    importing xpms   */
#include "xpm/mouse.xpm"
#include "xpm/topBarGameMode.xpm"
#include "xpm/quitButton.xpm"
#include "xpm/startButton.xpm"
#include "xpm/letters.xpm"
#include "xpm/numbers.xpm"
#include "xpm/font.xpm"
#include "xpm/Play_Again.xpm"
#include "xpm/Quit.xpm"
#include "xpm/Leaderboard.xpm"
#include "xpm/leaderboardTable.xpm"
#include "xpm/leaderboardButtonInitialPage.xpm"
#include "xpm/coopGuessButton.xpm"
#include "xpm/coopDrawButton.xpm"
#include "xpm/Victory.xpm"
#include "xpm/Defeat.xpm"
#include "xpm/dealer.xpm"

#define MAX_WORD_LENGTH 12
#define MAX_LINE_LENGTH 50

typedef enum {
    DRAWER,
    GUESSER,
    SINGLE
} PlayerRole;

/**
 * @brief Enumeration representing the system state.
 */
typedef enum {
    RUNNING, /**< The system is running. */
    EXIT /**< The system is exiting. */
} SystemState;

/**
 * @brief Enumeration representing the menu state.
 */
typedef enum {
    START, /**< The start menu state. */
    GAME, /**< The game menu state. */
    LEADERBOARD, /**< The leaderboard menu state. */
    END /**< The end game menu state. */
} MenuState;

typedef enum {
    SINGLEPLAYER,
    DRAW,
    GUESS
} GameState;

/**
 * @brief Structure representing a leaderboard value.
 */
typedef struct leaderboardValue {
    uint8_t month; /**< The month value of the leaderboard entry. */
    uint8_t day; /**< The day value of the leaderboard entry. */
    uint8_t hour; /**< The hour value of the leaderboard entry. */
    uint8_t minute; /**< The minute value of the leaderboard entry. */
    uint8_t second; /**< The second value of the leaderboard entry. */
    uint8_t score; /**< The score value of the leaderboard entry. */
} leaderboardValue;

/**
 * @brief Updates the timer state.
 */
void update_timer_state();

/**
 * @brief Updates the keyboard state.
 */
void update_keyboard_state();

/**
 * @brief Updates the mouse state.
 */
void update_mouse_state();

/**
 * @brief Updates the uart state.
*/
void update_uart_state();

/**
 * @brief Sets up the sprites.
 */
void setup_sprites();

/**
 * @brief Destroys the sprites.
 */
void destroy_sprites();

/**
 * @brief Updates the leaderboard with a new value.
 *
 * @param newValue The new leaderboard value to update.
 */
void updateLeaderboard(leaderboardValue *newValue);

/**
 * @brief Loads the leaderboard values from a file.
 *
 * @param leaderboard The array to store the loaded leaderboard values.
 */
void loadLeaderboardFromFile(leaderboardValue leaderboard[]);

/**
 * @brief Saves the leaderboard values to a file.
 *
 * @param leaderboard The array containing the leaderboard values to save.
 */
void saveLeaderboardToFile(const leaderboardValue leaderboard[]);

/**
 * @brief Clears the leaderboard file.
 */
void clearLeaderboardFile();

/**
 * @brief Adds a value to the leaderboard.
 */
void addValueToLeaderboard();

/**
 * @brief Checks the result of the game.
 *
 * @return True if the result is correct, false otherwise.
 */
bool checkResult();

/**
 * @brief Gets a random word for the game.
 *
 * @return A pointer to the random word.
 */
char* getRandomWord();

/**
 * @brief Updates the state on mouse click.
 */
void updateStateMouseClick();

/**
 * @brief Updates the state on keyboard click.
 */
void updateStateKeyboardClick();

/**
 * @brief Converts a string to QWERTY format.
 *
 * @param str The string to convert.
 * @param word The array to store the converted word.
 * @param word_size The size of the converted word.
 * @return The number of characters converted.
 */
int convert_to_qwerty(char* str, uint8_t word[], int* word_size);


