
#include "game.h"

/** @defgroup game game
 * @{
 *
 * Functions that draw the UI of the game
 */


uint8_t k_index = 0;
uint8_t k_bytes[2];
extern uint8_t k_scancode;

extern int m_index;
extern uint8_t m_bytes[3];
extern struct packet m_packet;
extern vbe_mode_info_t mode_info;

extern GameState gameState;
extern bool gameState_change;
extern bool multi;

bool door1_open = false;
bool door2_open = false;

extern Sprite *menu_bg, *title, *start, *hover_start, *quit, *hover_quit, *cursor, 
        *level1, *hover_level1, *level2, *hover_level2, *level3, *hover_level3, *waiting, *wait,*energy,
        *maze, *player, *player2, *life, *arrow, *button1, *button1_down, *button2, *button2_down, *door1, *door2,
        *game_over, *win;

extern Sprite *num0, *num1,*num2, *num3, *num4, *num5, *num6, *num7, *num8, *num9, *dot;
extern Sprite *smallNum0, *smallNum1,*smallNum2, *smallNum3, *smallNum4, *smallNum5, *smallNum6, *smallNum7, *smallNum8, *smallNum9, *divisor;


/* Menu */
/** 
 * @brief Draws all the sprites corresponding to the the menu.
 */
void (draw_menu)(){
  drawing_bg(menu_bg);
  drawing_sprite(start);
  drawing_sprite(quit);
  drawing_sprite(cursor);
  drawing_sprite(title);
  update_flip_frames(); 
}

/** 
 * @brief Updates the menu based on cursor position.
 */
void (update_menu)(){
  drawing_bg(menu_bg);
  drawing_sprite(title);

  if (collision(cursor,start)) drawing_sprite(hover_start);
  else drawing_sprite(start);

  if (collision(cursor,quit)) drawing_sprite(hover_quit);
  else drawing_sprite(quit);

  drawing_sprite(cursor);

  update_flip_frames();
}

/** 
 * @brief Checks if two sprites collide.
 * 
 * @param sp1 Pointer to the first sprite.
 * @param sp2 Pointer to the second sprite.
 * 
 * @return true if the sprites collide, false otherwise.
 */
bool (collision)(Sprite * sp1, Sprite * sp2) {
  if (sp1->x + sp1->width < sp2->x || sp1->x > sp2->x + sp2->width) return false;
  if (sp1->y + sp1->height < sp2->y || sp1->y > sp2->y + sp2->height) return false;

  return true;
}


/* Levels */
/** 
 * @brief Draws all the sprites corresponding to the levels menu.
 */
void (draw_menu_levels)() {
  drawing_sprite(menu_bg);
  drawing_sprite(title);
  drawing_sprite(level1);
  drawing_sprite(level2);
  drawing_sprite(level3);
  update_flip_frames();
}

/** 
 * @brief Updates the levels menu based on cursor position.
 */
void (update_menu_levels)(){
  drawing_bg(menu_bg);
  drawing_sprite(title);

  if (collision(cursor,level1)) drawing_sprite(hover_level1);
  else drawing_sprite(level1);

  if (collision(cursor,level2)) drawing_sprite(hover_level2);
  else drawing_sprite(level2);

  if (collision(cursor,level3)) drawing_sprite(hover_level3);
  else drawing_sprite(level3);

  drawing_sprite(cursor);

  update_flip_frames();
}

/** 
 * @brief Loads the specified level.
 * If there's an existing sprite maze, it destroys it to clear the screen before loading the new maze.
 * It selects the appropriate maze and associated sprites.
 * It retrieves the current game time in order to change the level sprites according to the time.
 * 
 * @param level The level number to load.
 */
void (load_level)(int level) {
  uint8_t hours, minutes, seconds;
  get_game_time(&hours, &minutes, &seconds);

  if (maze != NULL) 
    destroy_sprite(maze);

  switch (level) {
    case 1:
      if (hours >= 6 && hours < 14) {
        maze = create_sprite((xpm_map_t) mazeDay1, 0, 0, 0);
      } else if (hours >= 20 || hours < 6) {
        maze = create_sprite((xpm_map_t) mazeDark1, 0, 0, 0);
      } else {
        maze = create_sprite((xpm_map_t) maze1, 0, 0, 0);
      }
      button1 = create_sprite((xpm_map_t) button_red_, 90 , 497 , 0);
      button2 = create_sprite((xpm_map_t) button_green_, 774 , 170 , 0);
      button1_down = create_sprite((xpm_map_t) button_red_down_, 89 , 499 , 0);
      button2_down = create_sprite((xpm_map_t) button_green_down_, 773 , 172 , 0);
      door1 = create_sprite((xpm_map_t) door_red_, 210 , 450 , 0);
      door2 = create_sprite((xpm_map_t) door_green_, 760 , 340 , 0);
      break;

    case 2:
      if (hours >= 6 && hours < 14) {
        maze = create_sprite((xpm_map_t) mazeDay2, 0, 0, 0);
      } else if (hours >= 20 || hours < 6) {
        maze = create_sprite((xpm_map_t) mazeDark2, 0, 0, 0);
      } else {
        maze = create_sprite((xpm_map_t) maze2, 0, 0, 0);
      }
      button1 = create_sprite((xpm_map_t) button_red_, 306 , 62 , 0);
      button2 = create_sprite((xpm_map_t) button_green_, 450 , 170 , 0);
      button1_down = create_sprite((xpm_map_t) button_red_down_, 305 , 64 , 0);
      button2_down = create_sprite((xpm_map_t) button_green_down_, 449 , 172 , 0);
      door1 = create_sprite((xpm_map_t) door_red_, 455 , 197 , 0);
      door2 = create_sprite((xpm_map_t) door_green_, 580 , 197 , 0);
      break;

    case 3:
      if (hours >= 6 && hours < 14) {
        maze = create_sprite((xpm_map_t) mazeDay3, 0, 0, 0);
      } else if (hours >= 20 || hours < 6) {
        maze = create_sprite((xpm_map_t) mazeDark3, 0, 0, 0);
      } else {
        maze = create_sprite((xpm_map_t) maze3, 0, 0, 0);
      }
      button1 = create_sprite((xpm_map_t) button_red_, 220 , 207 , 0); 
      button2 = create_sprite((xpm_map_t) button_green_, 522 , 350, 0);
      button1_down = create_sprite((xpm_map_t) button_red_down_, 219 , 209 , 0); 
      button2_down = create_sprite((xpm_map_t) button_green_down_, 521 , 352, 0);
      door1 = create_sprite((xpm_map_t) door_red_, 219 , 160 , 0);
      door2 = create_sprite((xpm_map_t) door_green_, 590 , 557 , 0);
      break;

    default:
      return;
  }
}


/* Game */
/** 
 * @brief Updates the game depending on the time, cursor position and player position.
 */
void (update_game)() {
  uint8_t hours, minutes, seconds;
  get_game_time(&hours, &minutes, &seconds);
  drawing_lantern(maze, player, 60);

  if (multi) {
    drawing_lantern(maze, player2, 60);
    drawing_sprite(player2);
  }
  else {
    drawing_lantern(maze, cursor, 60);
  }

  drawing_sprite(arrow);
  drawing_sprite(player);
  drawing_sprite(life);
  draw_time_small(hours, minutes, 10, 575);

  if (collision(player,button1) || (collision(player2, button1))){
    door1_open = true;
  }
  if (collision(player,button2) || (collision(player2,button2))){
    door2_open = true;
  }

  if (!multi)
    drawing_sprite(cursor);

  update_flip_frames();
}

/** 
 * @brief Gets the next sprite for player 1 based on the scancode.
 * 
 * @param scancode The keyboard scancode.
 * 
 * @return The corresponding sprite.
 */
xpm_map_t (get_next_sprite_player1)(uint8_t scancode) {
    switch (scancode) {
        case A_KEY_MK:
            return (xpm_map_t)left1;
        case D_KEY_MK:
            return (xpm_map_t)right1;
        case W_KEY_MK:
            return (xpm_map_t)up1;
        case S_KEY_MK:
            return (xpm_map_t)down1;
        case A_KEY_BRK:
            return (xpm_map_t)left2;
        case D_KEY_BRK:
            return (xpm_map_t)right2;
        case W_KEY_BRK:
            return (xpm_map_t)up2;
        case S_KEY_BRK:
            return (xpm_map_t)down2;

        default:
            return (xpm_map_t)right1;
    }
}

/** 
 * @brief Gets the next sprite for player 2 based on the scancode.
 * 
 * @param scancode The keyboard scancode.
 * 
 * @return The corresponding sprite.
 */
xpm_map_t (get_next_sprite_player2)(uint8_t scancode) {
    switch (scancode) {
        case A_KEY_MK:
            return (xpm_map_t)left1second;
        case D_KEY_MK:
            return (xpm_map_t)right1second;
        case W_KEY_MK:
            return (xpm_map_t)up1second;
        case S_KEY_MK:
            return (xpm_map_t)down1second;
        case A_KEY_BRK:
            return (xpm_map_t)left2second;
        case D_KEY_BRK:
            return (xpm_map_t)right2second;
        case W_KEY_BRK:
            return (xpm_map_t)up2second;
        case S_KEY_BRK:
            return (xpm_map_t)down2second;

        default:
            return (xpm_map_t)right1second;
    }
}

/**
 * @brief Handles the in-game keyboard scancode for a specific player. 
 * It checks if the player can move in the specified direction without colliding with the maze.
 *  If there's no collision, it updates the player's position accordingly by modifying the x and y coordinates of the player's sprite.
 *  It then loads the appropriate sprite for the player's movement direction.
 * 
 * @param scancode The scancode received from the keyboard.
 * @param player Pointer to the player's sprite.
 */
void (handle_ingame_scancode)(uint8_t scancode, Sprite *player) {
    switch (scancode) {
        case D_KEY_MK:
          if (!check_collision(player, maze, 5, 0)) {
            player->x = player->x + 5;
            loading_xpm(get_next_sprite_player1(D_KEY_MK), player);
            update_game();
          }
          break;

        case A_KEY_MK:
          if (!check_collision(player, maze, -5, 0)) {
            player->x = player->x - 5;
            loading_xpm(get_next_sprite_player1(A_KEY_MK), player);
            update_game();
          }
          break;

        case W_KEY_MK:
          if (!check_collision(player, maze, 0, -5)) {
            player->y = player->y - 5;
            loading_xpm(get_next_sprite_player1(W_KEY_MK), player);
            update_game();
          }
          break;
        
        case S_KEY_MK:
          if (!check_collision(player, maze, 0, 5) ) {
            player->y = player->y + 5;
            loading_xpm(get_next_sprite_player1(S_KEY_MK), player);
            update_game();
          }
          break;

        case A_KEY_BRK:
            loading_xpm(get_next_sprite_player1(A_KEY_BRK), player);
            update_game();
            break;

        case D_KEY_BRK:
            loading_xpm(get_next_sprite_player1(D_KEY_BRK), player);
            update_game();
            break;

        case S_KEY_BRK:
            loading_xpm(get_next_sprite_player1(S_KEY_BRK), player);
            update_game();
            break;

        case W_KEY_BRK:
            loading_xpm(get_next_sprite_player1(W_KEY_BRK), player);
            update_game();
            break;

        default:
            return;
    }
}

/**
 * @brief Handles the in-game keyboard scancode for multiplayer mode.
 *  It checks if the player can move in the specified direction without colliding with the maze.
 *  If there's no collision, it updates the player's position accordingly by modifying the x and y coordinates of the player's sprite.
 *  It then loads the appropriate sprite for the player's movement direction.
 * 
 * @param scancode The scancode received from the keyboard.
 * @param player2 Pointer to the second player's sprite.
 */
void (handle_ingame_scancode_multi)(uint8_t scancode, Sprite *player2) {
    switch (scancode) {
        case D_KEY_MK:
          if (!check_collision(player2, maze, 5, 0)) {
            player2->x = player2->x + 5;
            loading_xpm(get_next_sprite_player2(D_KEY_MK), player2);
            update_game();
          }
          break;

        case A_KEY_MK:
          if (!check_collision(player2, maze, -5, 0)) {
            player2->x = player2->x - 5;
            loading_xpm(get_next_sprite_player2(A_KEY_MK), player2);
            update_game();
          }
          break;

        case W_KEY_MK:
          if (!check_collision(player2, maze, 0, -5)) {
            player2->y = player2->y - 5;
            loading_xpm(get_next_sprite_player2(W_KEY_MK), player2);
            update_game();
          }
          break;
        
        case S_KEY_MK:
          if (!check_collision(player2, maze, 0, 5)) {
            player2->y = player2->y + 5;
            loading_xpm(get_next_sprite_player2(S_KEY_MK), player2);
            update_game();
          }
          break;

        case A_KEY_BRK:
            loading_xpm(get_next_sprite_player2(A_KEY_BRK), player2);
            update_game();
            break;

        case D_KEY_BRK:
            loading_xpm(get_next_sprite_player2(D_KEY_BRK), player2);
            update_game();
            break;

        case S_KEY_BRK:
            loading_xpm(get_next_sprite_player2(S_KEY_BRK), player2);
            update_game();
            break;

        case W_KEY_BRK:
            loading_xpm(get_next_sprite_player2(W_KEY_BRK), player2);
            update_game();
            break;

        default:
            return;
    }
}

/**
 * @brief Handles mouse movement within the game window.
 * 
 * @param cursor Pointer to the sprite representing the mouse cursor.
 */
void (handle_mouse_movement)(Sprite * cursor){
  if(!(cursor->x + m_packet.delta_x <= 0)) cursor->x += m_packet.delta_x;
  if(!(cursor->y - m_packet.delta_y <= 0)) cursor->y -= m_packet.delta_y;
  if(cursor->x + cursor->width >= mode_info.XResolution)cursor->x = mode_info.XResolution - cursor->width;
  if(cursor->y + cursor->height >= mode_info.YResolution)cursor->y = mode_info.YResolution - cursor->height;
  if(cursor->x + cursor->width >= MOUSE_LIMIT_X)cursor->x = MOUSE_LIMIT_X - cursor->width;
  if(cursor->y + cursor->height >= MOUSE_LIMIT_Y)cursor->y = MOUSE_LIMIT_Y - cursor->height;
}

/**
 * @brief Updates the life bar based on the total game time.
 * 
 * @param total_seconds Total seconds elapsed in the game.
 */
void (update_life_bar)(int total_seconds) {
    switch(total_seconds){
        case 30:
            loading_xpm((xpm_map_t)life5, life);
            break;
        case 70:
            loading_xpm((xpm_map_t)life4, life);
            break;
        case 120:
            loading_xpm((xpm_map_t)life3, life);
            break;
        case 180:
            loading_xpm((xpm_map_t)life2, life);
            break;
        default:
            return;
    }
    update_game();
}

/**
 * @brief Loops the arrow sprite based on the total game time.
 * 
 * @param total_seconds Total seconds elapsed in the game.
 */
void (update_arrow_sprite)(int total_seconds) {
    int total_milliseconds = (300 - total_seconds) * 1000; 
    int frame = (total_milliseconds / 1000) % 6; 

    switch(frame) {
        case 0:
            loading_xpm((xpm_map_t)arrow6, arrow);
            break;
        case 1:
            loading_xpm((xpm_map_t)arrow5, arrow);
            break;
        case 2:
            loading_xpm((xpm_map_t)arrow4, arrow);
            break;
        case 3:
            loading_xpm((xpm_map_t)arrow3, arrow);
            break;
        case 4:
            loading_xpm((xpm_map_t)arrow2, arrow);
            break;
        case 5:
            loading_xpm((xpm_map_t)arrow1, arrow);
            break;
        default:
            return;
    }
    update_game();
}

/**
 * @brief Loops the energy sprite based on the total waiting time.
 * 
 * @param total_seconds Total seconds elapsed in the waiting frame.
 */

void (update_energy_sprite)(int total_seconds) {
    int total_milliseconds = (300 - total_seconds) * 1000; 
    int frame = (total_milliseconds / 1000) % 6; 

    switch(frame) {
        case 0:
            loading_xpm((xpm_map_t)energy1_, energy);
            break;
        case 1:
            loading_xpm((xpm_map_t)energy2_, energy);
            break;
        case 2:
            loading_xpm((xpm_map_t)energy3_, energy);
            break;
        case 3:
            loading_xpm((xpm_map_t)energy4_, energy);
            break;
        case 4:
            loading_xpm((xpm_map_t)energy5_, energy);
            break;
        default:
            return;
    }
    update_game();
}


/**
 * @brief Draws the number sprite in the game at a specified position on the screen.
 * 
 * @param num_sprite Pointer to the sprite representing the number.
 * @param x X-coordinate of the position to draw the number.
 * @param y Y-coordinate of the position to draw the number.
 */
void (draw_number)(Sprite *num_sprite, int x, int y) {
    num_sprite->x = x;
    num_sprite->y = y;
    drawing_sprite(num_sprite);
}

/**
 * @brief Draws the current game time on the screen.
 * 
 * @param total_seconds Total seconds elapsed in the game.
 * @param x X-coordinate of the position to draw the time.
 * @param y Y-coordinate of the position to draw the time.
 */
void (draw_time)(int total_seconds, int x, int y) {
    uint8_t minutes = total_seconds / 60;
    uint8_t seconds = total_seconds % 60;

    int x_offset = x;
    int spacing = num0->width + 10; 

    switch (minutes / 10) {
        case 0: draw_number(num0, x_offset, y); break;
        case 1: draw_number(num1, x_offset, y); break;
        case 2: draw_number(num2, x_offset, y); break;
        case 3: draw_number(num3, x_offset, y); break;
        case 4: draw_number(num4, x_offset, y); break;
        case 5: draw_number(num5, x_offset, y); break;
        case 6: draw_number(num6, x_offset, y); break;
        case 7: draw_number(num7, x_offset, y); break;
        case 8: draw_number(num8, x_offset, y); break;
        case 9: draw_number(num9, x_offset, y); break;
    }
    x_offset += num0->width;

    switch (minutes % 10) {
        case 0: draw_number(num0, x_offset, y); break;
        case 1: draw_number(num1, x_offset, y); break;
        case 2: draw_number(num2, x_offset, y); break;
        case 3: draw_number(num3, x_offset, y); break;
        case 4: draw_number(num4, x_offset, y); break;
        case 5: draw_number(num5, x_offset, y); break;
        case 6: draw_number(num6, x_offset, y); break;
        case 7: draw_number(num7, x_offset, y); break;
        case 8: draw_number(num8, x_offset, y); break;
        case 9: draw_number(num9, x_offset, y); break;
    }
    x_offset += spacing;  
    draw_number(dot, x_offset, y + (num0->height - dot->height) / 2); 
    x_offset += spacing;  

    switch (seconds / 10) {
        case 0: draw_number(num0, x_offset, y); break;
        case 1: draw_number(num1, x_offset, y); break;
        case 2: draw_number(num2, x_offset, y); break;
        case 3: draw_number(num3, x_offset, y); break;
        case 4: draw_number(num4, x_offset, y); break;
        case 5: draw_number(num5, x_offset, y); break;
        case 6: draw_number(num6, x_offset, y); break;
        case 7: draw_number(num7, x_offset, y); break;
        case 8: draw_number(num8, x_offset, y); break;
        case 9: draw_number(num9, x_offset, y); break;
    }
    x_offset += num0->width;

    switch (seconds % 10) {
        case 0: draw_number(num0, x_offset, y); break;
        case 1: draw_number(num1, x_offset, y); break;
        case 2: draw_number(num2, x_offset, y); break;
        case 3: draw_number(num3, x_offset, y); break;
        case 4: draw_number(num4, x_offset, y); break;
        case 5: draw_number(num5, x_offset, y); break;
        case 6: draw_number(num6, x_offset, y); break;
        case 7: draw_number(num7, x_offset, y); break;
        case 8: draw_number(num8, x_offset, y); break;
        case 9: draw_number(num9, x_offset, y); break;
    }
}

/**
 * @brief Draws the current game time (in small format) on the screen.
 * 
 * @param hours Hours component of the time.
 * @param minutes Minutes component of the time.
 * @param x X-coordinate of the position to draw the time.
 * @param y Y-coordinate of the position to draw the time.
 */
void (draw_time_small)(uint8_t hours, uint8_t minutes, int x, int y) {
  int x_offset = x;
    int spacing = 2;

    switch (hours / 10) {
        case 0: draw_number(smallNum0, x_offset, y); break;
        case 1: draw_number(smallNum1, x_offset, y); break;
        case 2: draw_number(smallNum2, x_offset, y); break;
        case 3: draw_number(smallNum3, x_offset, y); break;
        case 4: draw_number(smallNum4, x_offset, y); break;
        case 5: draw_number(smallNum5, x_offset, y); break;
        case 6: draw_number(smallNum6, x_offset, y); break;
        case 7: draw_number(smallNum7, x_offset, y); break;
        case 8: draw_number(smallNum8, x_offset, y); break;
        case 9: draw_number(smallNum9, x_offset, y); break;
    }
    x_offset += smallNum0->width + spacing;

    switch (hours % 10) {
        case 0: draw_number(smallNum0, x_offset, y); break;
        case 1: draw_number(smallNum1, x_offset, y); break;
        case 2: draw_number(smallNum2, x_offset, y); break;
        case 3: draw_number(smallNum3, x_offset, y); break;
        case 4: draw_number(smallNum4, x_offset, y); break;
        case 5: draw_number(smallNum5, x_offset, y); break;
        case 6: draw_number(smallNum6, x_offset, y); break;
        case 7: draw_number(smallNum7, x_offset, y); break;
        case 8: draw_number(smallNum8, x_offset, y); break;
        case 9: draw_number(smallNum9, x_offset, y); break;
    }
    x_offset += smallNum0->width + spacing;

    draw_number(divisor, x_offset, y + (smallNum0->height - divisor->height) / 2);
    x_offset += divisor->width + spacing;

    switch (minutes / 10) {
        case 0: draw_number(smallNum0, x_offset, y); break;
        case 1: draw_number(smallNum1, x_offset, y); break;
        case 2: draw_number(smallNum2, x_offset, y); break;
        case 3: draw_number(smallNum3, x_offset, y); break;
        case 4: draw_number(smallNum4, x_offset, y); break;
        case 5: draw_number(smallNum5, x_offset, y); break;
        case 6: draw_number(smallNum6, x_offset, y); break;
        case 7: draw_number(smallNum7, x_offset, y); break;
        case 8: draw_number(smallNum8, x_offset, y); break;
        case 9: draw_number(smallNum9, x_offset, y); break;
    }
    x_offset += smallNum0->width + spacing;

    switch (minutes % 10) {
        case 0: draw_number(smallNum0, x_offset, y); break;
        case 1: draw_number(smallNum1, x_offset, y); break;
        case 2: draw_number(smallNum2, x_offset, y); break;
        case 3: draw_number(smallNum3, x_offset, y); break;
        case 4: draw_number(smallNum4, x_offset, y); break;
        case 5: draw_number(smallNum5, x_offset, y); break;
        case 6: draw_number(smallNum6, x_offset, y); break;
        case 7: draw_number(smallNum7, x_offset, y); break;
        case 8: draw_number(smallNum8, x_offset, y); break;
        case 9: draw_number(smallNum9, x_offset, y); break;
    }
}

/**
 * @brief Draws all the sprites corresponding to the waiting menu screen.
 */
/* Waiting menu */
void (draw_waiting)() {
  drawing_bg(menu_bg);
  drawing_sprite(waiting);
  drawing_sprite(wait);
  drawing_sprite(energy);
  update_flip_frames();
}

/**
 * @brief Draws all the sprites corresponding to the win screen, (including the time the player took to finish the level).
 * 
 * @param total_seconds Total seconds elapsed in the game.
 */
/* Win */
void (draw_win)(int total_seconds) {
  drawing_bg(menu_bg);
  drawing_sprite(win);
  draw_time(total_seconds, 300, 305);

  if (collision(cursor,quit)) drawing_sprite(hover_quit);
  else drawing_sprite(quit);

  drawing_sprite(cursor);
  update_flip_frames();
}

/**
 * @brief Draws all the sprites corresponding to the game over screen.
 */
/* Lose */
void (draw_lost)() {
  drawing_bg(menu_bg);
  drawing_sprite(game_over);
  
  if (collision(cursor,quit)) drawing_sprite(hover_quit);
  else drawing_sprite(quit);
  
  drawing_sprite(cursor);
  update_flip_frames();
}

