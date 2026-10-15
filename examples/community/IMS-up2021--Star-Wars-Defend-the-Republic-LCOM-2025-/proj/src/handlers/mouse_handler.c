/**
 * @file mouse_handler.c
 * @brief Handles mouse input and UI interactions for different game states.
 */

#include <math.h>
#include "mouse_handler.h"
#include "menu.h"
#include "gameState.h"
#include "entity.h"
#include "game.h"
#include "buttons.h"
#include "characters.h"
#include "kbd_handler.h"

int kbd_state;

CharacterPos player_spawn_positions[5] = {
    {65, 190},
    {65, 320},
    {65, 449},
    {65, 570},
    {65, 699}
};

Cursor *cursor;
Position mouse_pos;

/**
 * @brief Handles mouse input events based on the current game state.
 *
 * In MAIN_MENU, interprets mouse clicks to change state to PLAYING, INSTRUCTIONS, or EXIT.
 * In PLAYING, handles spawning characters depending on the selected button and kbd_state.
 * Also processes PAUSE, RESUME, and FAST button clicks to change the play state.
 * In INSTRUCTIONS, allows returning to MAIN_MENU via the BACK button.
 *
 * @param mouse_packet Struct containing mouse input data (buttons and movement deltas).
 */
void mouse_event_handler(struct packet mouse_packet) {
    switch (state) {

        case MAIN_MENU:
            update_mouse_location(mouse_packet.delta_x, mouse_packet.delta_y);

            // Only respond to left button click
            if (!mouse_packet.lb) return;

            /**
             * @brief Check if PLAY button was clicked.
             */
            if (cursor->pos_x >= PLAY_BTN_X1 && cursor->pos_x <= PLAY_BTN_X2 &&
                cursor->pos_y >= PLAY_BTN_Y1 && cursor->pos_y <= PLAY_BTN_Y2) {
                state = PLAYING;
            }

            /**
             * @brief Check if INSTRUCTIONS button was clicked.
             */
            if (cursor->pos_x >= INSTR_BTN_X1 && cursor->pos_x <= INSTR_BTN_X2 &&
                cursor->pos_y >= INSTR_BTN_Y1 && cursor->pos_y <= INSTR_BTN_Y2) {
                state = INSTRUCTIONS;
            }

            /**
             * @brief Check if EXIT button was clicked.
             */
            if (cursor->pos_x >= EXIT_BTN_X1 && cursor->pos_x <= EXIT_BTN_X2 &&
                cursor->pos_y >= EXIT_BTN_Y1 && cursor->pos_y <= EXIT_BTN_Y2) {
                printf("Exit button clicked!\n");
                state = EXIT;
                return;
            }
            break;

        case PLAYING:
            update_mouse_location(mouse_packet.delta_x, mouse_packet.delta_y);
            if (!mouse_packet.lb) return;

            /**
             * @brief Check each spawn button. Depending on kbd_state, spawn corresponding character.
             */
            if (cursor->pos_x >= BTN1_X1 && cursor->pos_x <= BTN1_X2 &&
                cursor->pos_y >= BTN1_Y1 && cursor->pos_y <= BTN1_Y2) {
                if(kbd_state == 1) {
                    add_player_unit_to_game(player_spawn_positions[0], PLAYER_CHAR_TYPE_1);
                } else if(kbd_state == 2) {
                    add_player_unit_to_game(player_spawn_positions[0], PLAYER_CHAR_TYPE_2);
                } else if(kbd_state == 3) {
                    add_player_unit_to_game(player_spawn_positions[0], PLAYER_CHAR_TYPE_3);
                }
                printf("Spawned player unit at position 0\n");
                return;
            }

            if (cursor->pos_x >= BTN2_X1 && cursor->pos_x <= BTN2_X2 &&
                cursor->pos_y >= BTN2_Y1 && cursor->pos_y <= BTN2_Y2) {
                if(kbd_state == 1) {
                    add_player_unit_to_game(player_spawn_positions[1], PLAYER_CHAR_TYPE_1);
                } else if(kbd_state == 2) {
                    add_player_unit_to_game(player_spawn_positions[1], PLAYER_CHAR_TYPE_2);
                } else if(kbd_state == 3) {
                    add_player_unit_to_game(player_spawn_positions[1], PLAYER_CHAR_TYPE_3);
                }
                printf("Spawned player unit at position 1\n");
                return;
            }

            if (cursor->pos_x >= BTN3_X1 && cursor->pos_x <= BTN3_X2 &&
                cursor->pos_y >= BTN3_Y1 && cursor->pos_y <= BTN3_Y2) {
                if(kbd_state == 1) {
                    add_player_unit_to_game(player_spawn_positions[2], PLAYER_CHAR_TYPE_1);
                } else if(kbd_state == 2) {
                    add_player_unit_to_game(player_spawn_positions[2], PLAYER_CHAR_TYPE_2);
                } else if(kbd_state == 3) {
                    add_player_unit_to_game(player_spawn_positions[2], PLAYER_CHAR_TYPE_3);
                }
                printf("Spawned player unit at position 2\n");
                return;
            }

            if (cursor->pos_x >= BTN4_X1 && cursor->pos_x <= BTN4_X2 &&
                cursor->pos_y >= BTN4_Y1 && cursor->pos_y <= BTN4_Y2) {
                if(kbd_state == 1) {
                    add_player_unit_to_game(player_spawn_positions[3], PLAYER_CHAR_TYPE_1);
                } else if(kbd_state == 2) {
                    add_player_unit_to_game(player_spawn_positions[3], PLAYER_CHAR_TYPE_2);
                } else if(kbd_state == 3) {
                    add_player_unit_to_game(player_spawn_positions[3], PLAYER_CHAR_TYPE_3);
                }
                printf("Spawned player unit at position 3\n");
                return;
            }

            if (cursor->pos_x >= BTN5_X1 && cursor->pos_x <= BTN5_X2 &&
                cursor->pos_y >= BTN5_Y1 && cursor->pos_y <= BTN5_Y2) {
                if(kbd_state == 1) {
                    add_player_unit_to_game(player_spawn_positions[4], PLAYER_CHAR_TYPE_1);
                } else if(kbd_state == 2) {
                    add_player_unit_to_game(player_spawn_positions[4], PLAYER_CHAR_TYPE_2);
                } else if(kbd_state == 3) {
                    add_player_unit_to_game(player_spawn_positions[4], PLAYER_CHAR_TYPE_3);
                }
                printf("Spawned player unit at position 4\n");
                return;
            }

            /**
             * @brief Check for PAUSE button click.
             */
            if (cursor->pos_x >= PAUSE_BTN_X1 && cursor->pos_x <= PAUSE_BTN_X2 &&
                cursor->pos_y >= PAUSE_BTN_Y1 && cursor->pos_y <= PAUSE_BTN_Y2) {
                printf("Pause button clicked!\n");
                set_play_state(PLAY_PAUSED);
                return;
            }

            /**
             * @brief Check for RESUME button click.
             */
            if (cursor->pos_x >= RESUME_BTN_X1 && cursor->pos_x <= RESUME_BTN_X2 &&
                cursor->pos_y >= RESUME_BTN_Y1 && cursor->pos_y <= RESUME_BTN_Y2) {
                printf("Resume button clicked!\n");
                set_play_state(PLAY_NORMAL);
                return;
            }

            /**
             * @brief Check for FAST button click.
             */
            if (cursor->pos_x >= FAST_BTN_X1 && cursor->pos_x <= FAST_BTN_X2 &&
                cursor->pos_y >= FAST_BTN_Y1 && cursor->pos_y <= FAST_BTN_Y2) {
                printf("Fast button clicked! Spawning fast character...\n");
                set_play_state(PLAY_FAST);
                return;
            }

            break;

        case INSTRUCTIONS:
            update_mouse_location(mouse_packet.delta_x, mouse_packet.delta_y);
            if (!mouse_packet.lb) return;

            /**
             * @brief Check if BACK button was clicked to return to MAIN_MENU.
             */
            if (cursor->pos_x >= BACK_BTN_X1 && cursor->pos_x <= BACK_BTN_X2 &&
                cursor->pos_y >= BACK_BTN_Y1 && cursor->pos_y <= BACK_BTN_Y2) {
                state = MAIN_MENU;
            }

            break;

        case EXIT:
            break;
    }
}
