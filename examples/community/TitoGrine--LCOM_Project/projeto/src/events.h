#ifndef _EVENTS_H_
#define _EVENTS_H_

#include <lcom/lcf.h>
#include <stdbool.h>
#include <stdint.h>

//#include "Images.h"
//#include "queue.h"
#include "sprite.h"
//#include "graphics.h"
#include "keyboard.h"
#include "words.h"
#include "rtc.h"

/**
 * @brief Indicates the state of a mouse button.
 * 
 */
enum button_event {
  IGNORE = 0,   /**< No relevant activity. */
  PRESSED,      /**< Button is pressed. */
  RELEASED      /**< Button was pressed in the previous interruption and is no longer pressed in the curent interruption. */
};

/**
 * @brief Indicates if there was a key event, i.e if a key was pressed,
 * read a makecode, or if a key was releases, read a breakcode. If 
 * neither then the state is NOTHING.
 * 
 */
enum key_event {
  NOTHING = 0,    /**< If a key was neither pressed or released, meaning, nothing happened. */
  PRESS,          /**< A key has been pressed. */
  RELEASE         /**< A key has been released. */
};

/**
 * @brief Struct to store information about middle, left and right 
 * mouse button.
 * 
 */
typedef struct{
  enum button_event mb;   /**< Middle mouse button's state information. */
  enum button_event rb;   /**< Right mouse button's state information. */
  enum button_event lb;   /**< Left mouse button's state information. */
}mouse_buttons;

/**
 * @brief Struct to store information about if a keyboard key was
 * pressed or released and the respective makecode/breakcode.
 * 
 */
typedef struct{
  enum key_event key;     /**< Key's state information (PRESS, RELEASE or NOTHING). */
  uint8_t key_code[2];    /**< Scancode array. */
}keyboard_keys;

/**
 * @brief Struct that stores the current program state.
 * 
 */
enum states {
  INITIAL_MENU,     /**< Program is in main menu */
  SETUP_PLAYER_1,   /**< Program is in the player 1 setup */
  SETUP_PLAYER_2,   /**< Program is in the player 2 setup */
  CHOOSE_WORD,      /**< Program is allowing a player to choose a word */
  PLAY,             /**< Program is allowing player to draw choosen word */
  END,              /**< Program is in victory screen after a player won */
  GUESS,            /**< Program is allowing a player to guess the word drawn */
  POINTS,           /**< Program is displaying score obtained by each player */
  DRAW,             /**< Program is allowing a player to draw casually */
};

/**
 * @brief Stores the type of paint functionality that is being used to 
 * draw.
 * 
 */
enum paint_types{
  PENCIL,          /**< Pencil mode, draws a circle sprite mimicking a pencil. */
  PEN,             /**< Pen mode, draw a diagonal line mimicking a fountain pen. */
  ERASER,          /**< Erase mode, allways draws a rounded white rectangle, mimicking an eraser. */
  SPRAY,           /**< Spray mode, draw a random pattern of pixels mimicking a spray can. */
  BUCKET           /**< Fill mode, fills all pixels a confined area with the current color. */
};

/**
 * @brief Struct responsable to store all information about the program
 * including current program state, mouse and keyboard events, selected
 * paint modes, previous and current cursor position.
 * 
 */
typedef struct{
  mouse_buttons mouse;          /**< Mouse events. */
  keyboard_keys keyboard;       /**< Keyboard events. */
  enum states state;            /**< Current program state. */
  enum paint_types paint_mode;  /**< Paint mode. */
  int16_t x_prev;               /**< Previous cursor position on the x axis. */
  int16_t y_prev;               /**< Previous cursor position on the y axis. */
  int16_t x_pos;                /**< Current cursor position on the y axis. */
  int16_t y_pos;                /**< Current cursor position on the y axis. */
} Events;

/**
 * @brief Returns a given number in binary. Whether the given number
 * is in 2's complement or already in binary, it always returns in
 * binary
 * 
 * @param binary          Number to convert to binary
 * 
 * @return int16_t        Given number in binary
 */
int16_t complement_to_decimal(uint16_t binary);

/**
 * @brief Initializes every mouse button and program state to their
 * default state.
 * 
 */
void intialize_states();

/**
 * @brief Creates most sprites used in the program in their default
 * initial position and speed. Excludes cursor sprites.
 * 
 */
void set_up_sprites();

/**
 * @brief Destroys all sprites used in the game. Frees up ocupied
 * memory used for the sprites.
 * 
 */
void destroy_sprites();

/**
 * @brief Sets up the default mouse pointer creating the sprite, 
 * drawing it and initializing the cursor position values in the
 * Events struct.
 * 
 */
void set_up_cursor();

/**
 * @brief Creates all sprites related to the different paint 
 * cursors.
 * 
 */
void set_up_cursors();

/**
 * @brief Draws all main menu sprites, including background, 
 * buttons, date icon and default mouse pointer.
 * 
 */
void set_up_main_menu();

/**
 * @brief Draws all setup menu sprites, including background, 
 * buttons, date icon and avatars.
 * 
 */
void set_up_player_menu();

/**
 * @brief Draws all paint menu sprites, including background, paint 
 * buttons and pallets, date icon and if in PLAY mode the players
 * avatars and scores.
 * 
 */
void set_up_paint_menu();

/**
 * @brief Draws all victory menu sprites, including background, 
 * winner frame and balloons.
 * 
 */
void set_up_victory_menu();

/**
 * @brief Gets three random words from the word bank, draws word
 * header and the words.
 * 
 */
void set_up_words();

/**
 * @brief Draws initial main menu buttons in their default state.
 * 
 */
void draw_menu_buttons();

/**
 * @brief Draws all existing avatars in their default positions to
 * fit in the avatar grid in set up menu.
 * 
 */
void draw_avatars();

/**
 * @brief Draw all buttons painting related and if in PLAY mode also
 * draws players avatars and scores on stats frame.
 * 
 */
void draw_paint_buttons();

/**
 * @brief Moves the ballons by first redrawing all static sprites
 * from the victory menu, updates the balloon sprites position, 
 * checks for collisions and then draws the ballons on the screen.
 * 
 */
void move_balloons();

/**
 * @brief Checks every pair of balloons for collision between
 * themselves and then every ballon for collisions with the window 
 * limits updating their positions accordingly.
 * 
 */
void check_collisions();

/**
 * @brief Checks if the given sprite collided with any of the window
 * sides and updates the sprites speed accordingly.
 * 
 * @param sp       Sprite to check collisions
 */
void detect_collision(Sprite *sp);

/**
 * @brief Given two sprites checks if there were any collisions
 * between the two and updates their speed and position accordingly.
 * 
 * @param sp_1      Sprite to check collision
 * @param sp_2      Sprite to check collision
 */
void detect_collision_pair(Sprite *sp_1, Sprite *sp_2);

/**
 * @brief Checks if the user has the mouse pointer hovering the date
 * icon. If so and the date is not showing then it prints the date 
 * and time, if not and the date and time are showing then it erases
 * it.
 * 
 */
void check_rtc();

/**
 * @brief Checks if the user is hovering the word header, if so
 * it checks if it selected the left, middle or right word, and
 * sets the choosen word accordingly.
 * 
 */
void check_words();

/**
 * @brief Checks if any current mouse button state is RELEASED and
 * if so, changes it to IGNORE. 
 * Prevents multiple release reads from the event handler since the
 * timer interruptions are faster then mouse interruptions when a
 * button is released the timer reads release multiple times before
 * mouse interruption correctly updates the buttons states. 
 * 
 */
void check_released();

/**
 * @brief Checks if the cursor is hovering any of the avatars and if
 * so it toggles that avatar for the current player selecting
 * 
 */
void check_avatar();

/**
 * @brief Toggles avatar choosen by the player. If another avatar 
 * was already toggled it erases previous toggle. Doesn't toggle
 * the given avatar if player two selected the same as player one.
 * 
 * @param avatar      Avatar to be toggled
 * @param toggle      Toggle sprite (differs if it's player 1 or 2)
 */
void toggle_avatar(Sprite* avatar, Sprite* toggle);

/**
 * @brief Depending on the current paint functionality and mouse event paint the 
 * canvas accordingly.
 * 
 */
void paint();

/**
 * @brief Checks if the RTC is not in the middle of an update and if the player is
 * hovering the date icon, and if so gets the current time and displays it.
 * 
 */
void update_rtc();

/**
 * @brief Updates the counter display with the current time in the chronometer and
 * decreases it by one second.
 * 
 */
void update_chrono();

/**
 * @brief Updates mouse displacement based on current and previous position.
 * 
 */
void update_delta();

/**
 * @brief Makes sure mouse pointers sprite is updated. Checks the displacement
 * didn't exceed window limits and calculates correct speed so it doesn't. It
 * then updates current and previous position and moves the mouse pointer. Resets
 * the x and y speed. 
 * 
 * @param sprite      Cursors sprite
 */
void update_cursor(Sprite* sprite);

/**
 * @brief Updates the position of ink sprite, i.e the sprite used to paint in the
 * canvas.
 * 
 * @param sp        Sprite, used to paint, to be updated
 */
void update_ink(Sprite* sp);

/**
 * @brief Checks if the player released the left button while hovering
 * any of the paint functionalities buttons and if so does necessary
 * changes.
 * 
 */
void update_paint_type();

/**
 * @brief Checks if player released the left button while hovering the color or 
 * grey palettes, and if so, gets the color where the mouse pointer clicked and 
 * sets it to the current color.
 * 
 */
void update_color();

/**
 * @brief Based on the mouse data passed as argument it updates each mouse 
 * button current state and the mouse displacement.
 * 
 * @param packet_data       Mouse's interruption data packet
 */
void update_mouse_event(struct packet *packet_data);

/**
 * @brief // COMMENT
 * 
 * @param scancode 
 */
void update_keyboard_event(uint8_t scancode[]);

/**
 * @brief Responsable for handling every event in the program. Checks current
 * program state and meaningful periferal events and makes the necessary changes
 * accordingly.
 * 
 */
void update_state();

#endif
