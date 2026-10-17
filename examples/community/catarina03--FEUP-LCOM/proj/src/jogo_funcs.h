#include <lcom/lcf.h>
#pragma once
#include "keyboard.h"
#include "game_structs.h"
#include "i8042.h"
#include "game_macros.h"

#include "img_files/fire_bullet-0.xpm"
#include "img_files/fire_bullet-1.xpm"
#include "img_files/fire_bullet-2.xpm"

#include "coins.h"
#include "collisions.h"
#include "enemies.h"
#include "numbers_letters.h"

/**
 * @brief Handles the movements of the tux with a state machine.
 * 
 * @param tux - Tux pointer to a Tux struct Object that contain all the necessary Tux information
 * @return int 0 if successful
 */
int handle_tux_movs(Tux *tux);

/**
 * @brief Checks the current state of the game and handles it accordingly.
 * 
 * @param g - Game pointer to a Game struct object that contains all the necessary game information
 * @param c - Coin pointer to a Coin struct object that cointains all the necessary coin information
 * @param t - Tux pointer to a Tux struct Object that contain all the necessary Tux information
 * @param mouse_event - mouse_ev pointer to a mouse_event struct object that contains all the necessary mouse event information
 * @return int 0 if successful
 */
int checkGameState(Game *g, Cursor *c, Tux *t, struct mouse_ev *mouse_event);

/**
 * @brief Subscribes the timer, keyboard and mouse interrupts.
 * 
 * @param timer_irq_set - Timer IRQ
 * @param kbd_irq_set - Keyboard IRQ
 * @param mouse_irq_set - Mouse IRQ
 * @return int 0 if successful, 1 otherwise
 */
int subscribes_interrupts(uint32_t *timer_irq_set, uint32_t *kbd_irq_set, uint32_t *mouse_irq_set);

/**
 * @brief Unsubscribes the timer, keyboard and mouse interrupts.
 * 
 * @return int 0 if successful, 1 otherwise
 */
int unsubscribes_interrupts();

/**
 * @brief Handles the movement of the mouse cursor.
 * 
 * @param mygame - Game pointer to a Game struct object that contains all the necessary game information
 * @param cursor - Cursor pointer to a Cursor struct object that contains all the necessary cursor information
 * @param mouse_packet - packet pointer to a mouse_packet struct object that contains all the necessary mouse packet information
 * @param sign_x - Signal of the movement in the X axis
 * @param sign_y - Signal of the movement in the Y axis
 * @return int 0 if successful
 */
int mouse_movement(Game *mygame, Cursor *cursor, struct packet *mouse_packet, bool sign_x, bool sign_y);

/**
 * @brief Handles tux and enemies collisions.
 * 
 * @param t - Tux pointer to a Tux struct Object that contain all the necessary Tux information
 * @param g - Game pointer to a Game struct object that contains all the necessary game information
 */
void handle_tux_collisions(Tux *t, Game * g);

/**
 * @brief Handles the keyboard input while the game is in GAME_RUNNING state with a state machine to determine the tux movements/actions.
 * 
 * @param g - Game pointer to a Game struct object that contains all the necessary game information
 * @param t - Tux pointer to a Tux struct Object that contain all the necessary Tux information
 * @return int 0 if successful
 */
int scancode_handler(Game *g,Tux*t);

/**
 * @brief Handles the actions taking place when the game state is GAME_RUNNING.
 * 
 * @param t - Tux pointer to a Tux struct Object that contain all the necessary Tux information
 * @param g - Game pointer to a Game struct object that contains all the necessary game information
 * @param letras - Letras pointer to a Letras struct object that contains all the necessary letras information
 * @param numeros - Numeros pointer to a Numeros struct object that contains all the necessary numeros information
 * @return int 0 if successful
 */
int game_run(Tux *t, Game * g,Letras * letras,Numeros *numeros);

/**
 * @brief Draws all active fireball at the moment.
 * 
 * @param t - Tux pointer to a Tux struct Object that contain all the necessary Tux information
 */
void drawFireballs(Tux *t);

/**
 * @brief Create a Fireball object
 * 
 * @param t - Tux pointer to a Tux struct Object that contain all the necessary Tux information
 * @param front - If the fireball will move to the front or to the back (depends of the postion of the tux, 1 if front, 0 if back)
 */
void createFireball(Tux *t,int front);

/**
 * @brief Creates a new Fireball object.
 * 
 * @param x - X coordinate of the fireball's position
 * @param y - Y coordinate of the fireball's position
 * @param front - If the fireball will move to the front or to the back (depends of the postion of the tux, 1 if front, 0 if back)
 * @return FireBall* - Fireball pointer to the fireball created
 */
FireBall* newFireball(int x,int y,int front);

/**
 * @brief Updates a fireball's position on the screen.
 * 
 * @param f - Fireball pointer to a Fireball struct object that contains all the necessary fireball information
 */
void updateFireball(FireBall* f);

/**
 * @brief Deletes a certain fireball.
 * 
 * @param f - Fireball pointer to a Fireball struct object that contains all the necessary fireball information
 */
void deleteFireball(FireBall* f);

/**
 * @brief Manages the active fireballs in the fireball array.
 *
 * @param g - Game pointer to a Game struct object that contains all the necessary game information
 * @param t - Tux pointer to a Tux struct Object that contain all the necessary Tux information
 */
void updateFireballs(Game * g,Tux* t);

/**
 * @brief Draws the highscores and the date/time in which they were made.
 * 
 * @param g - Game pointer to a Game struct object that contains all the necessary game information
 * @param l - Tux pointer to a Tux struct object that contains all the necessary tux information
 * @param n - Numeros pointer to a Numeros struct object that contains all the necessary numeros information
 */
void draw_highscores(Game *g ,Letras*l,Numeros n);

/**
 * @brief Updates the highscores stored in the array after a new highscore.
 * 
 * @param g - Game pointer to a Game struct object that contains all the necessary game information.
 * @return int 1 if successful
 */
int  update_highscores(Game *g);
