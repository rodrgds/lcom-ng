#ifndef __MODEL_H
#define __MODEL_H
#include "../devices/graphics/vbe.h"
#include "../draws/draw.h"
#include "defines.h"
#include "devices/graphics/video.h"
#include "devices/keyboard/i8042.h"
#include "devices/keyboard/keyboard.h"
#include "devices/mouse/mouse.h"
#include "devices/rtc/rtc.h"
#include "devices/timer/i8254.h"
#include "devices/timer/timer.h"
#include "devices/utils.h"
#include "sprite.h"
#include "visuals/xpms/arrow.xpm"
#include "visuals/xpms/bar.xpm"
#include "visuals/xpms/boy.xpm"
#include "visuals/xpms/boywalk1.xpm"
#include "visuals/xpms/boywalk1reverse.xpm"
#include "visuals/xpms/boywalk2.xpm"
#include "visuals/xpms/boywalk2reverse.xpm"
#include "visuals/xpms/boywin.xpm"
#include "visuals/xpms/buttonleft.xpm"
#include "visuals/xpms/buttonleftpressed.xpm"
#include "visuals/xpms/buttonright.xpm"
#include "visuals/xpms/buttonrightpressed.xpm"
#include "visuals/xpms/centerFire.xpm"
#include "visuals/xpms/centerToxic.xpm"
#include "visuals/xpms/centerWater.xpm"
#include "visuals/xpms/doorblue.xpm"
#include "visuals/xpms/doorred.xpm"
#include "visuals/xpms/dots.xpm"
#include "visuals/xpms/exit_button.xpm"
#include "visuals/xpms/girl.xpm"
#include "visuals/xpms/girlwalk1.xpm"
#include "visuals/xpms/girlwalk1reverse.xpm"
#include "visuals/xpms/girlwalk2.xpm"
#include "visuals/xpms/girlwalk2reverse.xpm"
#include "visuals/xpms/girlwin.xpm"
#include "visuals/xpms/greenleverright.xpm"
#include "visuals/xpms/hand.xpm"
#include "visuals/xpms/leaderboard_button.xpm"
#include "visuals/xpms/leftFire.xpm"
#include "visuals/xpms/leftToxic.xpm"
#include "visuals/xpms/leftWater.xpm"
#include "visuals/xpms/num_0.xpm"
#include "visuals/xpms/num_1.xpm"
#include "visuals/xpms/num_2.xpm"
#include "visuals/xpms/num_3.xpm"
#include "visuals/xpms/num_4.xpm"
#include "visuals/xpms/num_5.xpm"
#include "visuals/xpms/num_6.xpm"
#include "visuals/xpms/num_7.xpm"
#include "visuals/xpms/num_8.xpm"
#include "visuals/xpms/num_9.xpm"
#include "visuals/xpms/opendoor.xpm"
#include "visuals/xpms/redleverleft.xpm"
#include "visuals/xpms/rightFire.xpm"
#include "visuals/xpms/rightToxic.xpm"
#include "visuals/xpms/rightWater.xpm"
#include "visuals/xpms/start.xpm"
#include "visuals/xpms/tryagain.xpm"
#include "visuals/xpms/wall.xpm"
#include "visuals/xpms/wall2.xpm"
#include "visuals/xpms/wall20_20.xpm"
#include <lcom/lcf.h>
typedef enum {
  NORMAL,
  WALKLEFT,
  WALKRIGHT,
  WINNING
} SpriteState;

typedef enum {
  RUNNING,
  EXIT
} SystemState;

typedef enum {
  START,
  GAME,
  LEADERBOARD,
  GAMEOVER,
  WIN,
  END
} MenuState;

typedef struct {
  uint8_t year;
  uint8_t month;
  uint8_t day;
  uint8_t hour;
  uint8_t minute;
  int score;
} LeaderboardEntry;

/**
 * @brief Interrupt handler for the timer.
 */
void(timer_int_handler)();

/**
 * @brief Loads the sprites.
 */
void load_sprites();

/**
 * @brief Updates the array with the given level.
 * @param level The level to update the array with.
 */
void updateArrayWithLevel(int level);

/**
 * @brief Checks for collision between a sprite and a given position.
 * @param sp The sprite to check collision with.
 * @param x The x-coordinate of the position to check.
 * @param y The y-coordinate of the position to check.
 * @return The sprite that collided with the position, or NULL if no collision occurred.
 */
Sprite *checkCollision(Sprite *sp, uint16_t x, uint16_t y);

/**
 * @brief Updates the timer.
 */
void update_timer();

/**
 * @brief Updates the keyboard.
 */
void update_keyboard();

/**
 * @brief Updates the mouse.
 */
void update_mouse();

/**
 * @brief Destroys all sprites.
 */
void destroy_sprites();

/**
 * @brief Moves the cursor based on the given mouse packet.
 * @param pp The mouse packet containing the movement information.
 */
void move_cursor(struct packet *pp);

/**
 * @brief Handles the action for a sprite.
 * @param sp The sprite to handle the action for.
 */
void action_handler(Sprite *sp);

/**
 * @brief Updates the sprite.
 * @param sp The sprite to update.
 * @return 1 if the sprite was updated successfully, 0 otherwise.
 */
int update(Sprite *sp);

/**
 * @brief Updates the speed of the sprite.
 * @param sp The sprite to update the speed for.
 */
void updateSpeed(Sprite *sp);

/**
 * @brief Updates the position of the sprite.
 * @param sp The sprite to update the position for.
 * @return 1 if the sprite's position was updated successfully, 0 otherwise.
 */
int updatePosition(Sprite *sp);

/**
 * @brief Moves the sprite horizontally.
 * @param sp The sprite to move.
 * @return 1 if the sprite was moved successfully, 0 otherwise.
 */
int move_x(Sprite *sp);

/**
 * @brief Moves the sprite vertically.
 * @param sp The sprite to move.
 * @return 1 if the sprite was moved successfully, 0 otherwise.
 */
int move_y(Sprite *sp);

/**
 * @brief Sets the ground for the sprite.
 * @param sp The sprite to set the ground for.
 */
void set_ground(Sprite *sp);

/**
 * @brief Makes the sprite jump.
 * @param sp The sprite to make jump.
 */
void jump(Sprite *sp);

/**
 * @brief Applies gravity to the sprite.
 * @param sp The sprite to apply gravity to.
 */
void gravity(Sprite *sp);

/**
 * @brief Moves the sprite to the left.
 * @param sp The sprite to move.
 */
void left(Sprite *sp);

/**
 * @brief Moves the sprite to the right.
 * @param sp The sprite to move.
 */
void right(Sprite *sp);

/**
 * @brief Checks for mouse click and performs the corresponding action.
 * @param pp The mouse packet containing the click information.
 */
void check_mouse_click(struct packet pp);

/**
 * @brief Updates the real-time clock.
 */
void update_rtc();

/**
 * @brief Resets the states of the game.
 */
void reset_states();

/**
 * @brief Adds a score to the leaderboard.
 * @param year The year of the score.
 * @param month The month of the score.
 * @param day The day of the score.
 * @param hour The hour of the score.
 * @param minute The minute of the score.
 * @param score The score to add.
 */
void add_to_leaderboard(uint8_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, int score);

/**
 * @brief Initializes the leaderboard.
 */
void initialize_leaderboard();

/**
 * @brief Writes the leaderboard data to a file.
 */
void write_leaderboard_data();

/**
 * @brief Reads the leaderboard data from a file.
 */
void read_leaderboard_data();
#endif
