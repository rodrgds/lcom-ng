#ifndef GAME_H
#define GAME_H

#include <lcom/lcf.h>
#include "graphics.h"
#include "keyboard.h"
#include "Sprite.h"
#include <math.h>

/**
 * @brief  Initializes graphics mode and any game‐wide resources.
 * @return 0 on success, non‐zero on failure.
 */
int start_game();

/**
 * @brief  Fills the entire screen with a solid color.
 * @param  color   0xRRGGBB color to fill with.
 * @param  buffer  Target buffer (e.g., double_buffer).
 */
void draw_screen(uint32_t color, uint8_t *buffer);

/**
 * @brief  Runs the “reaction speed” mini‐game.
 * @return 0 on success, non‐zero on error or quit.
 */
int video_test_reaction();

/**
 * @brief  Displays the result screen for a single reaction attempt.
 * @param  reaction_time  Time in milliseconds for this attempt.
 * @param  times          Attempt number (1–5).
 * @param  progress_bar   Sprite to draw progress (optional).
 */
void show_result_screen(uint32_t reaction_time, int times, Sprite *progress_bar);

/**
 * @brief  Displays the final average reaction time screen.
 * @param  total         Sum of reaction times; will be divided by 5.
 * @param  progress_bar  Sprite to draw final progress.
 */
void show_ending_screen(uint32_t total, Sprite *progress_bar);

/**
 * @brief  Shows the “Too Soon!” screen when the player presses too early.
 * @param  progress_bar  Progress‐bar sprite (to keep UI consistent).
 * @param  index         Which attempt index (0–4).
 */
void show_too_soon_screen(Sprite *progress_bar, int index);

/**
 * @brief   Renders and handles the main menu, returning the selected game.
 * @return  0 = Reaction Test, 1 = Memory Game, 2 = Verbal Memory Test.
 */
int show_main_menu();

/**
 * @brief   Starts the memory sequence recall game.
 */
void play_memory_game();

/**
 * @brief   Generates a random sequence of uppercase letters.
 * @param   length  Number of letters to generate.
 */
void generate_sequence(int length);

/**
 * @brief   Displays the generated letter sequence on‐screen briefly.
 * @param   length  Number of letters in the sequence.
 */
void display_sequence(int length);

/**
 * @brief   Shows user‐typed sequence as they type.
 * @param   answer  Buffer of user’s input letters.
 * @param   length  Number of letters to display.
 */
void show_sequence(const char *answer, int length);

/**
 * @brief   Reads user keyboard input for the memory game.
 * @param   length  Expected number of letters.
 * @return  true if the user’s input matches the sequence; false otherwise.
 */
bool get_user_input(int length);

/**
 * @brief   Compares the user’s answer to the generated sequence.
 * @param   answer  User’s input letters.
 * @param   length  Number of letters in the sequence.
 * @return  true if they match; false otherwise.
 */
bool compare_answer(const char *answer, int length);

/**
 * @brief   Converts a keyboard scan code to its ASCII letter.
 * @param   sc  Scan code (make code) from the keyboard.
 * @return  Uppercase letter ‘A’–‘Z’, or 0 if not a valid letter key.
 */
char convert_scancode_to_char(uint8_t sc);

/**
 * @brief   Displays a centered message on a gray background.
 * @param   msg  Null‐terminated string to display.
 */
void show_message_screen(const char *msg);

/**
 * @brief   Shows a countdown (“Get Ready”) before the next level.
 */
void show_countdown_screen();

/**
 * @brief   Generates a new RGB color variation.
 * @param   color  Base 0xRRGGBB color.
 * @return  Modified color value.
 */
uint32_t new_color(uint32_t color);

/**
 * @brief   Displays the “You Lost!” screen and waits for ESC to return.
 */
void show_lost_screen_and_wait_for_esc();

/**
 * @brief   Draws a filled square.
 * @param   x      Top‐left X coordinate.
 * @param   y      Top‐left Y coordinate.
 * @param   size   Side length in pixels.
 * @param   color  0xRRGGBB fill color.
 */
void draw_square(int x, int y, int size, uint32_t color);

/**
 * @brief   Swaps the double buffer into video memory.
 */
void buffer_swap();

/**
 * @brief   Plays the verbal memory test game where players identify new vs seen words.
 */
void play_verbal_memory_test(void);

/**
 * @brief   Draws a word and UI for the verbal memory test.
 * @param   word      The current word to display.
 * @param   lives     Number of lives remaining.
 * @param   selected  0 = “NEW” selected, 1 = “SEEN” selected.
 */
void draw_verbal_memory_screen(const char *word, int lives, int selected);

/**
 * @brief   Checks if a word has already been shown in the verbal memory test.
 * @param   word        The word to check.
 * @param   seen_words  Array of pointers to words already shown.
 * @param   seen_count  Number of words in seen_words.
 * @return  true if the word exists in seen_words; false otherwise.
 */
bool word_already_shown(const char *word, const char *seen_words[], int seen_count);

#endif


