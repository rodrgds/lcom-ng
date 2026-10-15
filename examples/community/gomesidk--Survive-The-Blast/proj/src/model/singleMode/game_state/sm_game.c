#include "sm_game.h"

#define GRID_WIDTH 17
#define GRID_HEIGHT 15

struct sm_game_imp {
  Player *player1;
  Bomb *bomb_matrix[GRID_WIDTH][GRID_HEIGHT]; 
  Wall *wall_matrix[GRID_WIDTH][GRID_HEIGHT];
  Explosion *explosion_matrix[GRID_WIDTH][GRID_HEIGHT]; 
  uint16_t grid_square_width; 
  uint16_t x_initial_grid; 
  uint16_t y_initial_grid; 
  Sprite *game_background; 
  Sprite *grid_background; 
  BombOptions *bomb_options; 
  BombType player1_bomb_option;
  Button* exit;
  Sprite* your_score;
  Score* score;
};

SmGame *create_sm_game(SpriteLoader *loader, Score* score) {
  SmGame *smg = (SmGame *) malloc(sizeof(SmGame));
  if (smg == NULL) {
    return NULL;
  }
  
  smg->x_initial_grid = 208; 
  smg->y_initial_grid = 52; 
  smg->grid_square_width = get_sprite_width(get_wall(loader));
  smg->grid_background = get_grid_background(loader); 
  smg->game_background = get_game_background(loader);
  smg->exit = create_button(0, 0, get_exit(loader));

  smg->player1 = create_player(1* smg->grid_square_width, 1 * smg->grid_square_width, get_player1_left(loader), get_player1_right(loader), get_player1_up(loader), get_player1_down(loader), get_player1_standing(loader));

  if (smg->player1 == NULL) {
    free(smg);
    return NULL;
  }

  for (int i = 0; i < GRID_HEIGHT; i++) {
    for (int j = 0; j < GRID_WIDTH; j++) {
      // Initialize walls, bombs, and explosions
      if (i == 0 || i == GRID_HEIGHT - 1 || j == 0 || j == GRID_WIDTH - 1) {
        smg->wall_matrix[j][i] = create_wall(j, i, get_solid_wall(loader), 0, 1);
      } else if (i % 2 == 0 && j % 2 == 0) {
        smg->wall_matrix[j][i] = create_wall(j, i, get_solid_wall(loader), 0, 1);
      }else if (i == 5 || j == 3) {
        smg->wall_matrix[j][i] = create_wall(j, i, get_wall(loader), 1, 1);
      }else {
        smg->wall_matrix[j][i] = create_wall(j, i, get_wall(loader), 1, 0);
      }
      smg->bomb_matrix[j][i] = create_bomb(j, i, get_bomb(loader), 0);
      smg->explosion_matrix[j][i] = create_explosion(j, i, get_explosion(loader));
    }
  }

  smg->bomb_options = create_bomb_options(get_bomb_options(loader), get_selected_options(loader));
  smg->your_score = get_your_score_text(loader);
  smg->player1_bomb_option = NORMAL;
  smg->score = score;
  return smg;
}

void destroy_sm_game(SmGame *smg) {
  if (smg == NULL) {
    return;
  }

  destroy_player(smg->player1);

  for (int i = 0; i < GRID_HEIGHT; i++) {
    for (int j = 0; j < GRID_WIDTH; j++) {
      destroy_wall(smg->wall_matrix[j][i]);
      destroy_bomb(smg->bomb_matrix[j][i]);
      destroy_explosion(smg->explosion_matrix[j][i]);
    }
  }

  destroy_bomb_options(smg->bomb_options);
  destroy_button(smg->exit);
  free(smg);
}

void draw_sm_game(SmGame *smg) {
  if (smg == NULL) {
    return;
  }
  
  draw_sprite(smg->grid_background, smg->x_initial_grid + 33, smg->y_initial_grid + 50);

  for (int i = 0; i < GRID_HEIGHT; i++) {
    for (int j = 0; j < GRID_WIDTH; j++) {
      draw_wall(smg->wall_matrix[j][i], smg->x_initial_grid, smg->y_initial_grid, smg->grid_square_width);
      draw_bomb(smg->bomb_matrix[j][i], smg->x_initial_grid, smg->y_initial_grid, smg->grid_square_width);
      draw_explosion(smg->explosion_matrix[j][i], smg->x_initial_grid, smg->y_initial_grid, smg->grid_square_width);
    }

    if (get_player_Ypos(smg->player1) / smg->grid_square_width == i) {
      draw_player(smg->player1, smg->x_initial_grid, smg->y_initial_grid);
    }
  }
  draw_button(smg->exit);
  draw_bomb_options(smg->bomb_options, smg->player1_bomb_option);
  draw_sprite(smg->your_score, 1110- get_sprite_width(smg->your_score), 7);
  draw_score(smg->score, 1190, 5);
}


static bool check_wall_collision(SmGame *sm, uint16_t x, uint16_t y) {
  if (sm == NULL) {
    return true;
  }

  //calculate the grids positions of the 4 corners of the player
  uint16_t grid_x_top_left = x / sm->grid_square_width;
  uint16_t grid_y_top_left = y / sm->grid_square_width;
  
  uint16_t grid_x_bottom_right = (x + (sm->grid_square_width-1)) / sm->grid_square_width;
  uint16_t grid_y_bottom_right = (y + (sm->grid_square_width-1)) / sm->grid_square_width;

  uint16_t grid_x_top_right = (x + sm->grid_square_width -1) / sm->grid_square_width;
  uint16_t grid_y_top_right = y / sm->grid_square_width;

  uint16_t grid_x_bottom_left = x / sm->grid_square_width;
  uint16_t grid_y_bottom_left = (y + (sm->grid_square_width -1)) / sm->grid_square_width;

  if (grid_x_top_left < 0 || grid_x_bottom_right >= GRID_WIDTH || grid_y_top_left < 0 || grid_y_bottom_right >= GRID_HEIGHT) {
    return true; // Out of bounds
  }
  if (sm->wall_matrix[grid_x_top_left][grid_y_top_left] == NULL || is_wall_active(sm->wall_matrix[grid_x_top_left][grid_y_top_left])) {
    return true; // Wall at the position
  }
  if (sm->wall_matrix[grid_x_bottom_right][grid_y_bottom_right] == NULL || is_wall_active(sm->wall_matrix[grid_x_bottom_right][grid_y_bottom_right])) {
    return true; // Wall at the position
  }
  if (sm->wall_matrix[grid_x_top_right][grid_y_top_right] == NULL || is_wall_active(sm->wall_matrix[grid_x_top_right][grid_y_top_right])) {
    return true; // Wall at the position
  }
  if (sm->wall_matrix[grid_x_bottom_left][grid_y_bottom_left] == NULL || is_wall_active(sm->wall_matrix[grid_x_bottom_left][grid_y_bottom_left])) {
    return true; // Wall at the position
  }
  return false; // No wall collision
}


static int process_bomb_spawning(SmGame *sm) {
  if (sm == NULL) {
    return 1; // Go back to menu
  }

  decrease_decrease_spawn_rate_counter(sm->bomb_options); // Decrease the spawn rate counter
  // Check if the bomb options are available
  if (is_spawning(sm->bomb_options)) {
    uint16_t randomX = (rand() % 16) + 1; 
    uint16_t randomY = (rand() % 14) + 1; 
    while (is_wall_active(sm->wall_matrix[randomX][randomY]) || is_bomb_active(sm->bomb_matrix[randomX][randomY])) {
      randomX = (rand() % 16) + 1; // Generate a random X coordinate
      randomY = (rand() % 14) + 1; // Generate a random Y coordinate
    }
    set_bomb_active(sm->bomb_matrix[randomX][randomY], true, get_random_bomb_type()); // Activate the bomb
    reset_time_until_spawn(sm->bomb_options); // Reset the spawn rate
  }
  else {
    decrease_time_until_spawn(sm->bomb_options); // Decrease the time that is missing for the bomb to spanw
  }
  return 0; // Continue game
}

// 0 Continue game
// 1 Goes back to menu
static int check_bomb_exploded(SmGame *sm) {
  if (sm == NULL) {
    return -1;
  }

  for (int i = 1; i < GRID_HEIGHT-1; i++) {
    for (int j = 1; j < GRID_WIDTH-1; j++) {
      if (is_bomb_exploded(sm->bomb_matrix[j][i])) {
        uint16_t grid_x_top_left = get_player_Xpos(sm->player1) / sm->grid_square_width;
        uint16_t grid_y_top_left = get_player_Ypos(sm->player1) / sm->grid_square_width;
        
        uint16_t grid_x_bottom_right = (get_player_Xpos(sm->player1) + (sm->grid_square_width-1)) / sm->grid_square_width;
        uint16_t grid_y_bottom_right = (get_player_Ypos(sm->player1) + (sm->grid_square_width-1)) / sm->grid_square_width;

        BombType bomb_type = get_bomb_type(sm->bomb_matrix[j][i]);
        if (bomb_type == NORMAL) {
          if (grid_x_top_left == j && grid_y_top_left == i) {
            return 1; // Player is in the same position as the bomb, go to menu
          }else if( grid_x_top_left == j+1 && grid_y_top_left == i) {
            return 1; // In the right square
          }else if( grid_x_top_left == j && grid_y_top_left == i+1) {
            return 1; // In the bottom square
          }else if( grid_x_top_left == j-1 && grid_y_top_left == i) {
            return 1; // In the left square
          }else if( grid_x_top_left == j && grid_y_top_left == i-1) {
            return 1; // In the top square
          }else if( grid_x_bottom_right == j && grid_y_bottom_right == i) {
            return 1; // Player is in the same position as the bomb, exit game
          }else if( grid_x_bottom_right == j+1 && grid_y_bottom_right == i) {
            return 1; // In the right square
          }else if( grid_x_bottom_right == j && grid_y_bottom_right == i+1) {
            return 1; // In the bottom square
          }else if( grid_x_bottom_right == j-1 && grid_y_bottom_right == i) {
            return 1; // In the left square
          }else if( grid_x_bottom_right == j && grid_y_bottom_right == i-1) {
            return 1; // In the top square
          }
  
          if ( is_wall_destroyable(sm->wall_matrix[j+1][i]) && is_wall_active(sm->wall_matrix[j+1][i])) {
            set_wall_active(sm->wall_matrix[j+1][i], false); // Deactivate the wall, bomb destroyed it
            increment_score(sm->score);

          }
          if ( is_wall_destroyable(sm->wall_matrix[j][i+1]) && is_wall_active(sm->wall_matrix[j][i+1])) {
            set_wall_active(sm->wall_matrix[j][i+1], false); // Deactivate the wall, bomb destroyed it
            increment_score(sm->score);
          }
          if ( is_wall_destroyable(sm->wall_matrix[j-1][i]) && is_wall_active(sm->wall_matrix[j-1][i])) {
            set_wall_active(sm->wall_matrix[j-1][i], false); // Deactivate the wall, bomb destroyed it
            increment_score(sm->score);
          }
          if ( is_wall_destroyable(sm->wall_matrix[j][i-1]) && is_wall_active(sm->wall_matrix[j][i-1])) {
            set_wall_active(sm->wall_matrix[j][i-1], false); // Deactivate the wall, bomb destroyed it
            increment_score(sm->score);
          }
  
          set_explosion_active(sm->explosion_matrix[j][i], true); // Activate the explosion, in the bomb position
          if (is_wall_destroyable(sm->wall_matrix[j+1][i])) {
            set_explosion_active(sm->explosion_matrix[j+1][i], true); // Activate the explosion, if it is not a solid wall
          }
          if (is_wall_destroyable(sm->wall_matrix[j][i+1])) {
            set_explosion_active(sm->explosion_matrix[j][i+1], true); // Activate the explosion, if it is not a solid wall
          }
          if (is_wall_destroyable(sm->wall_matrix[j-1][i])) {
            set_explosion_active(sm->explosion_matrix[j-1][i], true); // Activate the explosion, if it is not a solid wall
          }
          if (is_wall_destroyable(sm->wall_matrix[j][i-1])) {
            set_explosion_active(sm->explosion_matrix[j][i-1], true); // Activate the explosion, if it is not a solid wall
          }
        }else if ( bomb_type == FULL_LINE) {
          bool up_collision = false;
          bool down_collision = false;
          bool left_collision = false;
          bool right_collision = false;
          // It is done in this way to check the closer positions to the explosion first
          for ( int k = 0; k < GRID_WIDTH - 2; k++) { // The -2 is to avoid the walls at the borders


            if ( j + k < GRID_WIDTH - 1 &&  !right_collision) { // Right direction
              if (grid_x_top_left == j + k && grid_y_top_left == i) {
                return 1; 
              }else if( grid_x_bottom_right == j + k && grid_y_bottom_right == i) {
                return 1;
              }

              if ( is_wall_destroyable(sm->wall_matrix[j + k][i])) {
                set_explosion_active(sm->explosion_matrix[j + k][i], true); // Activate the explosion
                if ( is_wall_active(sm->wall_matrix[j + k][i]) ){
                  set_wall_active(sm->wall_matrix[j + k][i], false); // Deactivate the wall, bomb destroyed
                  increment_score(sm->score);
                  right_collision = true;
                }
              }else {
                right_collision = true; // It is a solid wall, stop the explosion in this direction
              }

            }

            if ( j - k > 0 && !left_collision) { // Left direction
              if (grid_x_top_left == j - k && grid_y_top_left == i) {
                return 1; 
              }else if( grid_x_bottom_right == j - k && grid_y_bottom_right == i) {
                return 1;
              }

              if ( is_wall_destroyable(sm->wall_matrix[j - k][i])) {
                set_explosion_active(sm->explosion_matrix[j - k][i], true); // Activate the explosion
                if ( is_wall_active(sm->wall_matrix[j - k][i]) ){
                  set_wall_active(sm->wall_matrix[j - k][i], false); // Deactivate the wall, bomb destroyed
                  increment_score(sm->score);
                  left_collision = true;
                }
              }else {
                left_collision = true; // It is a solid wall, stop the explosion in this direction
              }
            }

            if ( i + k < GRID_HEIGHT - 1 && !down_collision) { // Down direction
              if (grid_x_top_left == j && grid_y_top_left == i + k) {
                return 1; 
              }else if( grid_x_bottom_right == j && grid_y_bottom_right == i + k) {
                return 1;
              }

              if ( is_wall_destroyable(sm->wall_matrix[j][i + k])) {
                set_explosion_active(sm->explosion_matrix[j][i + k], true); // Activate the explosion
                if ( is_wall_active(sm->wall_matrix[j][i + k]) ){
                  set_wall_active(sm->wall_matrix[j][i + k], false); // Deactivate the wall, bomb destroyed
                  increment_score(sm->score);
                  down_collision = true;
                }
              }else {
                down_collision = true; // It is a solid wall, stop the explosion in this direction
              }
            }

            if ( i - k > 0 && !up_collision) { // Up direction
              if (grid_x_top_left == j && grid_y_top_left == i - k) {
                return 1; 
              }else if( grid_x_bottom_right == j && grid_y_bottom_right == i - k) {
                return 1;
              }

              if ( is_wall_destroyable(sm->wall_matrix[j][i - k])) {
                set_explosion_active(sm->explosion_matrix[j][i - k], true); // Activate the explosion
                if ( is_wall_active(sm->wall_matrix[j][i - k]) ){
                  set_wall_active(sm->wall_matrix[j][i - k], false); // Deactivate the wall, bomb destroyed
                  increment_score(sm->score);
                  up_collision = true;
                }
              }else {
                up_collision = true; // It is a solid wall, stop the explosion in this direction
              }
            }

            if ( up_collision && down_collision && left_collision && right_collision) {
              break;
            }
          }

        }else if ( bomb_type == CONSTRUCTIVE) {

          if (grid_x_top_left == j && grid_y_top_left == i) {
            return 1; // Player is in the same position as the bomb, go to menu
          }else if( grid_x_top_left == j+1 && grid_y_top_left == i) {
            return 1; // In the right square
          }else if( grid_x_top_left == j && grid_y_top_left == i+1) {
            return 1; // In the bottom square
          }else if( grid_x_top_left == j-1 && grid_y_top_left == i) {
            return 1; // In the left square
          }else if( grid_x_top_left == j && grid_y_top_left == i-1) {
            return 1; // In the top square
          }else if( grid_x_bottom_right == j && grid_y_bottom_right == i) {
            return 1; // Player is in the same position as the bomb, exit game
          }else if( grid_x_bottom_right == j+1 && grid_y_bottom_right == i) {
            return 1; // In the right square
          }else if( grid_x_bottom_right == j && grid_y_bottom_right == i+1) {
            return 1; // In the bottom square
          }else if( grid_x_bottom_right == j-1 && grid_y_bottom_right == i) {
            return 1; // In the left square
          }else if( grid_x_bottom_right == j && grid_y_bottom_right == i-1) {
            return 1; // In the top square
          }
          
          if ( is_wall_destroyable(sm->wall_matrix[j][i]) && !is_wall_active(sm->wall_matrix[j][i])) {
            set_wall_active(sm->wall_matrix[j][i], true); // Activate the wall
            set_explosion_active(sm->explosion_matrix[j][i], true); // Activate the explosion,
          }
          if ( is_wall_destroyable(sm->wall_matrix[j+1][i]) && !is_wall_active(sm->wall_matrix[j+1][i])) {
            set_wall_active(sm->wall_matrix[j+1][i], true); // Activate the wall
            set_explosion_active(sm->explosion_matrix[j+1][i], true); // Activate the explosion
          }
          if ( is_wall_destroyable(sm->wall_matrix[j][i+1]) && !is_wall_active(sm->wall_matrix[j][i+1])) {
            set_wall_active(sm->wall_matrix[j][i+1], true); // Activate the wall
            set_explosion_active(sm->explosion_matrix[j][i+1], true); // Activate the explosion
          }
          if ( is_wall_destroyable(sm->wall_matrix[j-1][i]) && !is_wall_active(sm->wall_matrix[j-1][i])) {
            set_wall_active(sm->wall_matrix[j-1][i], true); // ACtivate the wall
            set_explosion_active(sm->explosion_matrix[j-1][i], true); // Activate the explosion
          }
          if ( is_wall_destroyable(sm->wall_matrix[j][i-1]) && !is_wall_active(sm->wall_matrix[j][i-1])) {
            set_wall_active(sm->wall_matrix[j][i-1], true); // Activate the wall
            set_explosion_active(sm->explosion_matrix[j][i-1], true); // Activate the explosion
          }
        }    
      }
    }
  }
  return 0;
}

// 0 Continue game
// 1 Goes back to menu
int process_sm_game_kbd(SmGame *sm, KeyPressed * key) {
  if (sm == NULL) {
    return 1; 
  }

  bool is_some_key_pressed = false;
  if ( is_up_pressed(key) ){ // Up
    is_some_key_pressed = true;
    for (int i = 0; i < get_player_speed(sm->player1); i++) {
      if (!check_wall_collision(sm, get_player_Xpos(sm->player1), get_player_Ypos(sm->player1) - 1)){
        player_move_up(sm->player1);
      }else{
        break;
      }
    }
  }
  if ( is_down_pressed(key) ){ // Down
    is_some_key_pressed = true;
    for (int i = 0; i < get_player_speed(sm->player1); i++) {
      if (!check_wall_collision(sm, get_player_Xpos(sm->player1), get_player_Ypos(sm->player1) + 1)){
        player_move_down(sm->player1);
      }else{
        break;
      }
    }
  }
  if ( is_left_pressed(key) ){ // Left
    is_some_key_pressed = true;
    for (int i = 0; i < get_player_speed(sm->player1); i++) {
      if (!check_wall_collision(sm, get_player_Xpos(sm->player1) - 1, get_player_Ypos(sm->player1))){
        player_move_left(sm->player1);
      }else{
        break;
      }
    }
  }
  if ( is_right_pressed(key)){ // Right
    is_some_key_pressed = true;
    for (int i = 0; i < get_player_speed(sm->player1); i++) {
      if (!check_wall_collision(sm, get_player_Xpos(sm->player1) + 1, get_player_Ypos(sm->player1))){
        player_move_right(sm->player1);
      }else{
        break;
      }
    }
  }
  if ( !is_some_key_pressed ) { 
    player_stand(sm->player1);
  } 
  if ( is_esc_pressed(key) ) { 
    return 1; 
  }
  return 0; 
}

// 0 Continue game
// 1 Goes back to menu
int process_sm_game_mouse(SmGame *sm, Cursor *c) {
  if (sm == NULL || c == NULL) {
    return 1; 
  }
  
  // Rigth mouse button pressed
  if (get_cursor_button_pressed(c, 2)) {
    
    uint16_t grid_x =  (get_player_Xpos(sm->player1)+30) / sm->grid_square_width;
    uint16_t grid_y =  (get_player_Ypos(sm->player1)+30) / sm->grid_square_width;

    if (!is_bomb_active(sm->bomb_matrix[grid_x][grid_y])) {
      if (!is_bomb_off_cooldown(sm->bomb_options, sm->player1_bomb_option)) {
        return 0;
      }
      else {
        set_bomb_active(sm->bomb_matrix[grid_x][grid_y], true, sm->player1_bomb_option);
        set_bomb_on_cooldown(sm->bomb_options, sm->player1_bomb_option); 
      }                                
    }
  }

  // Left mouse button pressed
  if (get_cursor_button_pressed(c, 0)) {

    if (is_button_clicked(sm->exit, c)) {
      return 1; // Go back to menu
    }

    // --- Bomb option buttons ---
    int x_min = get_options_x_initial(sm->bomb_options);
    int x_max = get_options_x_final(sm->bomb_options);

    if (get_cursor_Xpos(c) > x_min && get_cursor_Xpos(c) < x_max) {
      for (int i = 0; i < 3; ++i) {
        int y_min = get_options_y_initial(sm->bomb_options, i);
        int y_max = get_options_y_final(sm->bomb_options, i);

        if (get_cursor_Ypos(c) > y_min && get_cursor_Ypos(c) < y_max) {
          sm->player1_bomb_option = (BombType)i;
          break;
        }
      }
    }
  }

  return 0; // Continue game
  
}

// 0 Continue game
// 1 Go back to menu
// 2 Go to died state
int process_sm_game_timer(SmGame *sm) {
  if (sm == NULL) {
    return 1; // Go back to menu
  }

  if (process_bomb_spawning(sm) == 1) {
    return 1; // Go back to menu
  }

  int ret = check_bomb_exploded(sm);
  if (ret == 1) {
    return 2; // Go to died state
  }else if (ret == -1) {
    return 1; // Go back to menu
  }

  increment_score(sm->score);
  decrease_cooldown_time(sm->bomb_options);
  return 0; // Continue game
}
