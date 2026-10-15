/**
 * @file timer_event_handler.c
 * @brief Handles periodic events triggered by the timer, updating game logic and rendering based on the current game state.
 */

#include "timer_handler.h"
#include "gameState.h"
#include "manager.h"
#include "menu.h"
#include "enemy.h"

#include "kbd_handler.h"
#include "mouse_handler.h"
#include "characters.h"
#include "health.h"

#include "game.h"

Player *player_health;
Player *enemy_health;

unsigned int health_player;
unsigned int health_enemy;

/**
 * @brief Handles timer interrupts by updating game logic and rendering depending on the current state.
 *
 * - In `MAIN_MENU`, it draws the menu.
 * - In `PLAYING`, it updates/spawns units, draws entities and health bars, and adjusts timer frequencies based on `play_state`.
 * - In `INSTRUCTIONS`, it draws the instructions screen.
 * - In `EXIT`, it performs no actions.
 *
 * @param game_state The current state of the game.
 */
void timer_event_handler(gameState game_state) {
    switch (state) {
        case MAIN_MENU:
            draw_menu();
            break;
        case PLAYING:
            update_and_spawn_enemies();
            update_and_spawn_player_units();
            update_healthbar(player_health, true);
            update_healthbar(enemy_health, false);
            switch(kbd_state){
                case 2:
                    draw_characters2();
                    break;
                case 3:
                    draw_characters3();
                    break;
                case 1:
                    draw_game();
                    break;
                default:
                    break;
            }
            draw_healthbar(player_health, true);
            draw_healthbar(enemy_health, false);
            draw_player_units();
            draw_enemies();
            
            switch (play_state) {
                case PLAY_PAUSED:
                    timer_set_frequency(0, 1);
                    set_timer_frequency_for_enemies(0);
                    set_timer_frequency_for_heroes(0);
                    break;
                case PLAY_NORMAL:
                    timer_set_frequency(0, 60);
                    set_timer_frequency_for_enemies(60);
                    set_timer_frequency_for_heroes(60);
                    break;
                case PLAY_FAST:
                    timer_set_frequency(0, 120);
                    set_timer_frequency_for_enemies(120);
                    set_timer_frequency_for_heroes(120);
                    break;
            }

            break;
        case INSTRUCTIONS:
            draw_instructions();
            break;
        case EXIT:
            break;
    }
}

