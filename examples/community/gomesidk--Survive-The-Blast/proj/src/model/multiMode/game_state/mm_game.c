#include "mm_game.h"

#define BIT(n) (1 << (n))

#define GRID_WIDTH 17
#define GRID_HEIGHT 15
#define BOMB_TYPE_NONE        0x00
#define BOMB_TYPE_NORMAL      0x01
#define BOMB_TYPE_FULL_LINE   0x02
#define BOMB_TYPE_CONSTRUCTIVE 0x03

struct mm_game_imp {
  Player *other_player; 
  Player *my_player;

  int winner; // 0 - No winner, 1 - My wins, 2 - Other wins

  SpriteLoader *loader; // Pointer to the sprite loader
  Bomb *bomb_matrix[GRID_WIDTH][GRID_HEIGHT]; 
  Wall *wall_matrix[GRID_WIDTH][GRID_HEIGHT];
  Explosion *explosion_matrix[GRID_WIDTH][GRID_HEIGHT]; 
  uint16_t grid_square_width; 
  uint16_t x_initial_grid; 
  uint16_t y_initial_grid; 
  Sprite *game_background; 
  Sprite *grid_background; 
  BombOptions *bomb_options; 
  BombType player_bomb_option;
  Button* exit;

  uint8_t byte_to_send; // Byte to send over serial port
  // Bit 0 & 1 - 00 No bomb, 01 Normal, 10 Line, 11 Construction
  // Bit 2 - Move up
  // Bit 3 - Move down
  // Bit 4 - Move left
  // Bit 5 - Move right
  // Bit 6 - 
  // Bit 7 - Exit 
};

MmGame *create_mm_game(SpriteLoader *loader, uint8_t player_number) {
  MmGame *mm_game = (MmGame *) malloc(sizeof(MmGame));
  if (mm_game == NULL) {
    return NULL;
  }
  
  mm_game->loader = loader;
  mm_game->winner = 0; // No winner initially

  mm_game->x_initial_grid = 208; 
  mm_game->y_initial_grid = 52; 
  mm_game->grid_square_width = get_sprite_width(get_wall(loader));
  mm_game->grid_background = get_grid_background(loader); 
  mm_game->game_background = get_game_background(loader);
  mm_game->exit = create_button(0, 0, get_exit(loader));

  if (player_number == 1) {
    mm_game->my_player = create_player(1 * mm_game->grid_square_width, 1 * mm_game->grid_square_width, get_player1_left(loader), get_player1_right(loader), get_player1_up(loader), get_player1_down(loader), get_player1_standing(loader));
    mm_game->other_player = create_player(15 * mm_game->grid_square_width, 13 * mm_game->grid_square_width, get_player2_left(loader), get_player2_right(loader), get_player2_up(loader), get_player2_down(loader), get_player2_standing(loader));
  } else {
    mm_game->my_player = create_player(15 * mm_game->grid_square_width, 13 * mm_game->grid_square_width, get_player2_left(loader), get_player2_right(loader), get_player2_up(loader), get_player2_down(loader), get_player2_standing(loader));
    mm_game->other_player = create_player(1 * mm_game->grid_square_width, 1 * mm_game->grid_square_width, get_player1_left(loader), get_player1_right(loader), get_player1_up(loader), get_player1_down(loader), get_player1_standing(loader));
  }
  if (mm_game->my_player == NULL || mm_game->other_player == NULL) {
    free(mm_game);
    printf("Error creating players in mm_game\n");
    return NULL;
  }

  for (int i = 0; i < GRID_HEIGHT; i++) {
    for (int j = 0; j < GRID_WIDTH; j++) {
      // Initialize walls, bombs, and explosions
      if (i == 0 || i == GRID_HEIGHT - 1 || j == 0 || j == GRID_WIDTH - 1) {
        mm_game->wall_matrix[j][i] = create_wall(j, i, get_solid_wall(loader), 0, 1);
      } else if (i % 2 == 0 && j % 2 == 0) {
        mm_game->wall_matrix[j][i] = create_wall(j, i, get_solid_wall(loader), 0, 1);
      }else if (i == 5 || j == 3) {
        mm_game->wall_matrix[j][i] = create_wall(j, i, get_wall(loader), 1, 1);
      }else {
        mm_game->wall_matrix[j][i] = create_wall(j, i, get_wall(loader), 1, 0);
      }
      mm_game->bomb_matrix[j][i] = create_bomb(j, i, get_bomb(loader), 0);
      mm_game->explosion_matrix[j][i] = create_explosion(j, i, get_explosion(loader));
    }
  }

  mm_game->bomb_options = create_bomb_options(get_bomb_options(loader), get_selected_options(loader));
  mm_game->player_bomb_option = NORMAL;
  return mm_game;
}

void destroy_mm_game(MmGame *mm_game) {
  if (mm_game == NULL) {
    return;
  }
  destroy_player(mm_game->my_player);
  destroy_player(mm_game->other_player);

  for (int i = 0; i < GRID_HEIGHT; i++) {
    for (int j = 0; j < GRID_WIDTH; j++) {
      destroy_wall(mm_game->wall_matrix[j][i]);
      destroy_bomb(mm_game->bomb_matrix[j][i]);
      destroy_explosion(mm_game->explosion_matrix[j][i]);
    }
  }

  destroy_button(mm_game->exit);
  destroy_bomb_options(mm_game->bomb_options);
  free(mm_game);
}

void draw_mm_game(MmGame *mm_game) {
  if (mm_game == NULL) {
    return;
  }
  draw_sprite(mm_game->grid_background, mm_game->x_initial_grid + 33, mm_game->y_initial_grid + 50);
  draw_button(mm_game->exit);

  for (int i = 0; i < GRID_HEIGHT; i++) {
    for (int j = 0; j < GRID_WIDTH; j++) {
      draw_wall(mm_game->wall_matrix[j][i], mm_game->x_initial_grid, mm_game->y_initial_grid, mm_game->grid_square_width);
      draw_bomb(mm_game->bomb_matrix[j][i], mm_game->x_initial_grid, mm_game->y_initial_grid, mm_game->grid_square_width);
      draw_explosion(mm_game->explosion_matrix[j][i], mm_game->x_initial_grid, mm_game->y_initial_grid, mm_game->grid_square_width);
    }

    if (get_player_Ypos(mm_game->my_player) / mm_game->grid_square_width == i) {
      draw_player(mm_game->my_player, mm_game->x_initial_grid, mm_game->y_initial_grid);
    }
    if (get_player_Ypos(mm_game->other_player) / mm_game->grid_square_width == i) {
      draw_player(mm_game->other_player, mm_game->x_initial_grid, mm_game->y_initial_grid);
    }
  }
  draw_bomb_options(mm_game->bomb_options, mm_game->player_bomb_option);
}


static bool check_wall_collision(MmGame *mm, uint16_t x, uint16_t y) {
  if (mm == NULL) {
    return true;
  }

  //calculate the grids positions of the 4 corners of the player
  uint16_t grid_x_top_left = x / mm->grid_square_width;
  uint16_t grid_y_top_left = y / mm->grid_square_width;
  
  uint16_t grid_x_bottom_right = (x + (mm->grid_square_width-1)) / mm->grid_square_width;
  uint16_t grid_y_bottom_right = (y + (mm->grid_square_width-1)) / mm->grid_square_width;

  uint16_t grid_x_top_right = (x + mm->grid_square_width -1) / mm->grid_square_width;
  uint16_t grid_y_top_right = y / mm->grid_square_width;

  uint16_t grid_x_bottom_left = x / mm->grid_square_width;
  uint16_t grid_y_bottom_left = (y + (mm->grid_square_width -1)) / mm->grid_square_width;

  if (grid_x_top_left < 0 || grid_x_bottom_right >= GRID_WIDTH || grid_y_top_left < 0 || grid_y_bottom_right >= GRID_HEIGHT) {
    return true; // Out of bounds
  }
  if (mm->wall_matrix[grid_x_top_left][grid_y_top_left] == NULL || is_wall_active(mm->wall_matrix[grid_x_top_left][grid_y_top_left])) {
    return true; // Wall at the position
  }
  if (mm->wall_matrix[grid_x_bottom_right][grid_y_bottom_right] == NULL || is_wall_active(mm->wall_matrix[grid_x_bottom_right][grid_y_bottom_right])) {
    return true; // Wall at the position
  }
  if (mm->wall_matrix[grid_x_top_right][grid_y_top_right] == NULL || is_wall_active(mm->wall_matrix[grid_x_top_right][grid_y_top_right])) {
    return true; // Wall at the position
  }
  if (mm->wall_matrix[grid_x_bottom_left][grid_y_bottom_left] == NULL || is_wall_active(mm->wall_matrix[grid_x_bottom_left][grid_y_bottom_left])) {
    return true; // Wall at the position
  }
  return false; // No wall collision
}

// 0 Continue game
// 1 Goes back to menu
static int check_bomb_exploded(MmGame *mm) {
  if (mm == NULL) {
    return -1;
  }

  for (int i = 1; i < GRID_HEIGHT-1; i++) {
    for (int j = 1; j < GRID_WIDTH-1; j++) {
      if (is_bomb_exploded(mm->bomb_matrix[j][i])) {
        uint16_t grid_x_top_left_my = get_player_Xpos(mm->my_player) / mm->grid_square_width;
        uint16_t grid_y_top_left_my = get_player_Ypos(mm->my_player) / mm->grid_square_width;

        uint16_t grid_x_bottom_right_my = (get_player_Xpos(mm->my_player) + (mm->grid_square_width-1)) / mm->grid_square_width;
        uint16_t grid_y_bottom_right_my = (get_player_Ypos(mm->my_player) + (mm->grid_square_width-1)) / mm->grid_square_width;

        uint16_t grid_x_top_left_other = get_player_Xpos(mm->other_player) / mm->grid_square_width;
        uint16_t grid_y_top_left_other = get_player_Ypos(mm->other_player) / mm->grid_square_width;

        uint16_t grid_x_bottom_right_other = (get_player_Xpos(mm->other_player) + (mm->grid_square_width-1)) / mm->grid_square_width;
        uint16_t grid_y_bottom_right_other = (get_player_Ypos(mm->other_player) + (mm->grid_square_width-1)) / mm->grid_square_width;

        BombType bomb_type = get_bomb_type(mm->bomb_matrix[j][i]);
        if (bomb_type == NORMAL) {

          bool my_player_in_bomb = false;
          bool other_player_in_bomb = false;

          if (grid_x_top_left_my == j && grid_y_top_left_my == i) my_player_in_bomb = true;
          else if (grid_x_bottom_right_my == j && grid_y_bottom_right_my == i) my_player_in_bomb = true;

          if (grid_x_top_left_other == j && grid_y_top_left_other == i) other_player_in_bomb = true;
          else if (grid_x_bottom_right_other == j && grid_y_bottom_right_other == i) other_player_in_bomb = true;

          if ( my_player_in_bomb && other_player_in_bomb) {
            mm->winner = 0; // Draw, both players are in the same position as the bomb
            return 1; 
          }else if ( my_player_in_bomb ) {
            mm->winner = 2; // My player wins, The other player is in the same position as the bomb
            return 1;
          }else if ( other_player_in_bomb ) {
            mm->winner = 1; // Other player wins, I am in the same position as the bomb
            return 1;
          }

          if (grid_x_top_left_my == j +1 && grid_y_top_left_my == i) my_player_in_bomb = true; 
          else if (grid_x_top_left_my == j && grid_y_top_left_my == i + 1) my_player_in_bomb = true; 
          else if (grid_x_top_left_my == j - 1 && grid_y_top_left_my == i) my_player_in_bomb = true; 
          else if (grid_x_top_left_my == j && grid_y_top_left_my == i - 1) my_player_in_bomb = true; 
          else if (grid_x_bottom_right_my == j + 1 && grid_y_bottom_right_my == i) my_player_in_bomb = true; 
          else if (grid_x_bottom_right_my == j && grid_y_bottom_right_my == i + 1) my_player_in_bomb = true; 
          else if (grid_x_bottom_right_my == j - 1 && grid_y_bottom_right_my == i) my_player_in_bomb = true; 
          else if (grid_x_bottom_right_my == j && grid_y_bottom_right_my == i - 1) my_player_in_bomb = true; 

          if (grid_x_top_left_other == j +1 && grid_y_top_left_other == i) other_player_in_bomb = true; 
          else if (grid_x_top_left_other == j && grid_y_top_left_other == i + 1) other_player_in_bomb = true; 
          else if (grid_x_top_left_other == j - 1 && grid_y_top_left_other == i) other_player_in_bomb = true; 
          else if (grid_x_top_left_other == j && grid_y_top_left_other == i - 1) other_player_in_bomb = true; 
          else if (grid_x_bottom_right_other == j + 1 && grid_y_bottom_right_other == i) other_player_in_bomb = true; 
          else if (grid_x_bottom_right_other == j && grid_y_bottom_right_other == i + 1) other_player_in_bomb = true; 
          else if (grid_x_bottom_right_other == j - 1 && grid_y_bottom_right_other == i) other_player_in_bomb = true; 
          else if (grid_x_bottom_right_other == j && grid_y_bottom_right_other == i - 1) other_player_in_bomb = true;

          if ( my_player_in_bomb && other_player_in_bomb) {
            mm->winner = 0; // Draw, both players are at 1 square from the bomb
            return 1;
          }else if ( my_player_in_bomb ) {
            mm->winner = 2; // My player wins, The other player exploded with the bomb
            return 1;
          }else if ( other_player_in_bomb ) {
            mm->winner = 1; // Other player wins, I exploded with the bomb
            return 1;
          }
  
          if ( is_wall_destroyable(mm->wall_matrix[j+1][i]) && is_wall_active(mm->wall_matrix[j+1][i])) {
            set_wall_active(mm->wall_matrix[j+1][i], false); // Deactivate the wall, bomb destroyed it
          }
          if ( is_wall_destroyable(mm->wall_matrix[j][i+1]) && is_wall_active(mm->wall_matrix[j][i+1])) {
            set_wall_active(mm->wall_matrix[j][i+1], false); // Deactivate the wall, bomb destroyed it
          }
          if ( is_wall_destroyable(mm->wall_matrix[j-1][i]) && is_wall_active(mm->wall_matrix[j-1][i])) {
            set_wall_active(mm->wall_matrix[j-1][i], false); // Deactivate the wall, bomb destroyed it
          }
          if ( is_wall_destroyable(mm->wall_matrix[j][i-1]) && is_wall_active(mm->wall_matrix[j][i-1])) {
            set_wall_active(mm->wall_matrix[j][i-1], false); // Deactivate the wall, bomb destroyed it
          }
  
          set_explosion_active(mm->explosion_matrix[j][i], true); // Activate the explosion, in the bomb position
          if (is_wall_destroyable(mm->wall_matrix[j+1][i])) {
            set_explosion_active(mm->explosion_matrix[j+1][i], true); // Activate the explosion, if it is not a solid wall
          }
          if (is_wall_destroyable(mm->wall_matrix[j][i+1])) {
            set_explosion_active(mm->explosion_matrix[j][i+1], true); // Activate the explosion, if it is not a solid wall
          }
          if (is_wall_destroyable(mm->wall_matrix[j-1][i])) {
            set_explosion_active(mm->explosion_matrix[j-1][i], true); // Activate the explosion, if it is not a solid wall
          }
          if (is_wall_destroyable(mm->wall_matrix[j][i-1])) {
            set_explosion_active(mm->explosion_matrix[j][i-1], true); // Activate the explosion, if it is not a solid wall
          }

        }else if ( bomb_type == FULL_LINE) {
          bool up_collision = false;
          bool down_collision = false;
          bool left_collision = false;
          bool right_collision = false;

          // It is done in this way to check the closer positions to the explosion first
          for ( int k = 0; k < GRID_WIDTH - 2; k++) { // The -2 is to avoid the walls at the borders


            if ( j + k < GRID_WIDTH - 1 &&  !right_collision) { // Right direction
              bool is_my_player_in_bomb = false;
              bool is_other_player_in_bomb = false;

              if (grid_x_top_left_my == j + k && grid_y_top_left_my == i) {
                is_my_player_in_bomb = true;
              }else if( grid_x_bottom_right_my == j + k && grid_y_bottom_right_my == i) {
                is_my_player_in_bomb = true;
              }

              if (grid_x_top_left_other == j + k && grid_y_top_left_other == i) {
                is_other_player_in_bomb = true;
              }else if( grid_x_bottom_right_other == j + k && grid_y_bottom_right_other == i) {
                is_other_player_in_bomb = true;
              }

              if (is_my_player_in_bomb && is_other_player_in_bomb) {
                mm->winner = 0; // Draw, both players are at the same distance from the bomb
                return 1;
              }else if (is_my_player_in_bomb) {
                mm->winner = 2; // Other wins, i died
                return 1;
              }else if (is_other_player_in_bomb) {
                mm->winner = 1; // I win, other player died
                return 1;
              }

              if ( is_wall_destroyable(mm->wall_matrix[j + k][i])) {
                set_explosion_active(mm->explosion_matrix[j + k][i], true); // Activate the explosion
                if ( is_wall_active(mm->wall_matrix[j + k][i]) ){
                  set_wall_active(mm->wall_matrix[j + k][i], false); // Deactivate the wall, bomb destroyed
                  right_collision = true;
                }
              }else {
                right_collision = true; // It is a solid wall, stop the explosion in this direction
              }

            }

            if ( j - k > 0 && !left_collision) { // Left direction
              bool is_my_player_in_bomb = false;
              bool is_other_player_in_bomb = false;

              if (grid_x_top_left_my == j - k && grid_y_top_left_my == i) {
                is_my_player_in_bomb = true;
              }else if( grid_x_bottom_right_my == j - k && grid_y_bottom_right_my == i) {
                is_my_player_in_bomb = true;
              }

              if (grid_x_top_left_other == j - k && grid_y_top_left_other == i) {
                is_other_player_in_bomb = true;
              }else if( grid_x_bottom_right_other == j - k && grid_y_bottom_right_other == i) {
                is_other_player_in_bomb = true;
              }

              if (is_my_player_in_bomb && is_other_player_in_bomb) {
                mm->winner = 0; // Draw, both players are at the same distance from the bomb
                return 1;
              }else if (is_my_player_in_bomb) {
                mm->winner = 2; // Other wins, i died
                return 1;
              }else if (is_other_player_in_bomb) {
                mm->winner = 1; // I win, other player died
                return 1;
              }

              if ( is_wall_destroyable(mm->wall_matrix[j - k][i])) {
                set_explosion_active(mm->explosion_matrix[j - k][i], true); // Activate the explosion
                if ( is_wall_active(mm->wall_matrix[j - k][i]) ){
                  set_wall_active(mm->wall_matrix[j - k][i], false); // Deactivate the wall, bomb destroyed
                  left_collision = true;
                }
              }else {
                left_collision = true; // It is a solid wall, stop the explosion in this direction
              }
            }

            if ( i + k < GRID_HEIGHT - 1 && !down_collision) { // Down direction
              bool is_my_player_in_bomb = false;
              bool is_other_player_in_bomb = false;

              if (grid_x_top_left_my == j && grid_y_top_left_my == i + k) {
                is_my_player_in_bomb = true;
              }else if( grid_x_bottom_right_my == j && grid_y_bottom_right_my == i + k) {
                is_my_player_in_bomb = true;
              }

              if (grid_x_top_left_other == j && grid_y_top_left_other == i + k) {
                is_other_player_in_bomb = true;
              }else if( grid_x_bottom_right_other == j && grid_y_bottom_right_other == i + k) {
                is_other_player_in_bomb = true;
              }

              if (is_my_player_in_bomb && is_other_player_in_bomb) {
                mm->winner = 0; // Draw, both players are at the same distance from the bomb
                return 1;
              }else if (is_my_player_in_bomb) {
                mm->winner = 2; // Other wins, i died
                return 1;
              }else if (is_other_player_in_bomb) {
                mm->winner = 1; // I win, other player died
                return 1;
              }

              if ( is_wall_destroyable(mm->wall_matrix[j][i + k])) {
                set_explosion_active(mm->explosion_matrix[j][i + k], true); // Activate the explosion
                if ( is_wall_active(mm->wall_matrix[j][i + k]) ){
                  set_wall_active(mm->wall_matrix[j][i + k], false); // Deactivate the wall, bomb destroyed
                  down_collision = true;
                }
              }else {
                down_collision = true; // It is a solid wall, stop the explosion in this direction
              }
            }

            if ( i - k > 0 && !up_collision) { // Up direction
              bool is_my_player_in_bomb = false;
              bool is_other_player_in_bomb = false;

              if (grid_x_top_left_my == j && grid_y_top_left_my == i - k) {
                is_my_player_in_bomb = true;
              }else if( grid_x_bottom_right_my == j && grid_y_bottom_right_my == i - k) {
                is_my_player_in_bomb = true;
              }

              if (grid_x_top_left_other == j && grid_y_top_left_other == i - k) {
                is_other_player_in_bomb = true;
              }else if( grid_x_bottom_right_other == j && grid_y_bottom_right_other == i - k) {
                is_other_player_in_bomb = true;
              }

              if (is_my_player_in_bomb && is_other_player_in_bomb) {
                mm->winner = 0; // Draw, both players are at the same distance from the bomb
                return 1;
              }else if (is_my_player_in_bomb) {
                mm->winner = 2; // Other wins, i died
                return 1;
              }else if (is_other_player_in_bomb) {
                mm->winner = 1; // I win, other player died
                return 1;
              }

              if ( is_wall_destroyable(mm->wall_matrix[j][i - k])) {
                set_explosion_active(mm->explosion_matrix[j][i - k], true); // Activate the explosion
                if ( is_wall_active(mm->wall_matrix[j][i - k]) ){
                  set_wall_active(mm->wall_matrix[j][i - k], false); // Deactivate the wall, bomb destroyed
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

          bool my_player_in_bomb = false;
          bool other_player_in_bomb = false;

          if (grid_x_top_left_my == j && grid_y_top_left_my == i) my_player_in_bomb = true;
          else if (grid_x_bottom_right_my == j && grid_y_bottom_right_my == i) my_player_in_bomb = true;

          if (grid_x_top_left_other == j && grid_y_top_left_other == i) other_player_in_bomb = true;
          else if (grid_x_bottom_right_other == j && grid_y_bottom_right_other == i) other_player_in_bomb = true;

          if ( my_player_in_bomb && other_player_in_bomb) {
            mm->winner = 0; // Draw, both players are in the same position as the bomb
            return 1; 
          }else if ( my_player_in_bomb ) {
            mm->winner = 2; // My player wins, The other player is in the same position as the bomb
            return 1;
          }else if ( other_player_in_bomb ) {
            mm->winner = 1; // Other player wins, I am in the same position as the bomb
            return 1;
          }

          if (grid_x_top_left_my == j +1 && grid_y_top_left_my == i) my_player_in_bomb = true; 
          else if (grid_x_top_left_my == j && grid_y_top_left_my == i + 1) my_player_in_bomb = true; 
          else if (grid_x_top_left_my == j - 1 && grid_y_top_left_my == i) my_player_in_bomb = true; 
          else if (grid_x_top_left_my == j && grid_y_top_left_my == i - 1) my_player_in_bomb = true; 
          else if (grid_x_bottom_right_my == j + 1 && grid_y_bottom_right_my == i) my_player_in_bomb = true; 
          else if (grid_x_bottom_right_my == j && grid_y_bottom_right_my == i + 1) my_player_in_bomb = true; 
          else if (grid_x_bottom_right_my == j - 1 && grid_y_bottom_right_my == i) my_player_in_bomb = true; 
          else if (grid_x_bottom_right_my == j && grid_y_bottom_right_my == i - 1) my_player_in_bomb = true; 

          if (grid_x_top_left_other == j +1 && grid_y_top_left_other == i) other_player_in_bomb = true; 
          else if (grid_x_top_left_other == j && grid_y_top_left_other == i + 1) other_player_in_bomb = true; 
          else if (grid_x_top_left_other == j - 1 && grid_y_top_left_other == i) other_player_in_bomb = true; 
          else if (grid_x_top_left_other == j && grid_y_top_left_other == i - 1) other_player_in_bomb = true; 
          else if (grid_x_bottom_right_other == j + 1 && grid_y_bottom_right_other == i) other_player_in_bomb = true; 
          else if (grid_x_bottom_right_other == j && grid_y_bottom_right_other == i + 1) other_player_in_bomb = true; 
          else if (grid_x_bottom_right_other == j - 1 && grid_y_bottom_right_other == i) other_player_in_bomb = true; 
          else if (grid_x_bottom_right_other == j && grid_y_bottom_right_other == i - 1) other_player_in_bomb = true;

          if ( my_player_in_bomb && other_player_in_bomb) {
            mm->winner = 0; // Draw, both players are at 1 square from the bomb
            return 1;
          }else if ( my_player_in_bomb ) {
            mm->winner = 2; // My player wins, The other player exploded with the bomb
            return 1;
          }else if ( other_player_in_bomb ) {
            mm->winner = 1; // Other player wins, I exploded with the bomb
            return 1;
          }
          
          if ( is_wall_destroyable(mm->wall_matrix[j][i]) && !is_wall_active(mm->wall_matrix[j][i])) {
            set_wall_active(mm->wall_matrix[j][i], true); // Activate the wall
            set_explosion_active(mm->explosion_matrix[j][i], true); // Activate the explosion,
          }
          if ( is_wall_destroyable(mm->wall_matrix[j+1][i]) && !is_wall_active(mm->wall_matrix[j+1][i])) {
            set_wall_active(mm->wall_matrix[j+1][i], true); // Activate the wall
            set_explosion_active(mm->explosion_matrix[j+1][i], true); // Activate the explosion
          }
          if ( is_wall_destroyable(mm->wall_matrix[j][i+1]) && !is_wall_active(mm->wall_matrix[j][i+1])) {
            set_wall_active(mm->wall_matrix[j][i+1], true); // Activate the wall
            set_explosion_active(mm->explosion_matrix[j][i+1], true); // Activate the explosion
          }
          if ( is_wall_destroyable(mm->wall_matrix[j-1][i]) && !is_wall_active(mm->wall_matrix[j-1][i])) {
            set_wall_active(mm->wall_matrix[j-1][i], true); // ACtivate the wall
            set_explosion_active(mm->explosion_matrix[j-1][i], true); // Activate the explosion
          }
          if ( is_wall_destroyable(mm->wall_matrix[j][i-1]) && !is_wall_active(mm->wall_matrix[j][i-1])) {
            set_wall_active(mm->wall_matrix[j][i-1], true); // Activate the wall
            set_explosion_active(mm->explosion_matrix[j][i-1], true); // Activate the explosion
          }
        }    
      }
    }
  }
  return 0;
}

int process_mm_game_mouse(MmGame *mm_game, Cursor *c) {
  if (mm_game == NULL || c == NULL) {
    return -1; // Error
  }

  // Rigth mouse button pressed
  if (get_cursor_button_pressed(c, 2)) {
    
    uint16_t grid_x =  (get_player_Xpos(mm_game->my_player)+30) / mm_game->grid_square_width;
    uint16_t grid_y =  (get_player_Ypos(mm_game->my_player)+30) / mm_game->grid_square_width;

    if (!is_bomb_active(mm_game->bomb_matrix[grid_x][grid_y])) {
      if (!is_bomb_off_cooldown(mm_game->bomb_options, mm_game->player_bomb_option)) {
        return 0;
      }
      else {
        switch (mm_game->player_bomb_option) {
          case NORMAL:
            mm_game->byte_to_send |= (BOMB_TYPE_NORMAL);
            break;
          case FULL_LINE:
            mm_game->byte_to_send |= (BOMB_TYPE_FULL_LINE);
            break;
          case CONSTRUCTIVE:
            mm_game->byte_to_send |= (BOMB_TYPE_CONSTRUCTIVE);
            break;
          default:
            break;
        }
        set_bomb_active(mm_game->bomb_matrix[grid_x][grid_y], true, mm_game->player_bomb_option);
        set_bomb_on_cooldown(mm_game->bomb_options, mm_game->player_bomb_option); 
      }                                
    }
  }

  // Left mouse button pressed
  if (get_cursor_button_pressed(c, 0)) {

    if (is_button_clicked(mm_game->exit, c)){
      mm_game->byte_to_send |= (1 << 7); // Set bit 7 for exit
      send_byte(mm_game->byte_to_send);
      return 1; // Go back to menu
    }

    // --- Bomb option buttons ---
    int x_min = get_options_x_initial(mm_game->bomb_options);
    int x_max = get_options_x_final(mm_game->bomb_options);

    if (get_cursor_Xpos(c) > x_min && get_cursor_Xpos(c) < x_max) {
      for (int i = 0; i < 3; ++i) {
        int y_min = get_options_y_initial(mm_game->bomb_options, i);
        int y_max = get_options_y_final(mm_game->bomb_options, i);

        if (get_cursor_Ypos(c) > y_min && get_cursor_Ypos(c) < y_max) {
          mm_game->player_bomb_option = (BombType)i;
          break;
        }
      }
    }
  }
  return 0; // Continue game
}


int process_mm_game_kbd(MmGame *mm_game, KeyPressed *key) {
  if (mm_game == NULL || key == NULL) {
    return -1; // Error
  }

  mm_game->byte_to_send &= ~(BIT(2) | BIT(3) | BIT(4) | BIT(5)); // Clear movement bits
  bool is_some_key_pressed = false;

  if ( is_up_pressed(key) ){ // Up
    mm_game->byte_to_send |= BIT(2); // Set bit 2 for up movement
    is_some_key_pressed = true;
    for (int i = 0; i < get_player_speed(mm_game->my_player); i++) {
      if (!check_wall_collision(mm_game, get_player_Xpos(mm_game->my_player), get_player_Ypos(mm_game->my_player) - 1)){
        player_move_up(mm_game->my_player);
      }else{
        break;
      }
    }
  }

  if ( is_down_pressed(key) ){ // Down
    mm_game->byte_to_send |= BIT(3); // Set bit 3 for down movement
    is_some_key_pressed = true;
    for (int i = 0; i < get_player_speed(mm_game->my_player); i++) {
      if (!check_wall_collision(mm_game, get_player_Xpos(mm_game->my_player), get_player_Ypos(mm_game->my_player) + 1)){
        player_move_down(mm_game->my_player);
      }else{
        break;
      }
    }
  }

  if ( is_left_pressed(key) ){ // Left
    mm_game->byte_to_send |= BIT(4); // Set bit 4 for left movement
    is_some_key_pressed = true;
    for (int i = 0; i < get_player_speed(mm_game->my_player); i++) {
      if (!check_wall_collision(mm_game, get_player_Xpos(mm_game->my_player) - 1, get_player_Ypos(mm_game->my_player))){
        player_move_left(mm_game->my_player);
      }else{
        break;
      }
    }
  }

  if ( is_right_pressed(key)){ // Right
    mm_game->byte_to_send |= BIT(5); // Set bit 5 for right movement
    is_some_key_pressed = true;
    for (int i = 0; i < get_player_speed(mm_game->my_player); i++) {
      if (!check_wall_collision(mm_game, get_player_Xpos(mm_game->my_player) + 1, get_player_Ypos(mm_game->my_player))){
        player_move_right(mm_game->my_player);
      }else{
        break;
      }
    }
  }
  if ( !is_some_key_pressed ) { 
    player_stand(mm_game->my_player);
  } 
  if ( is_esc_pressed(key) ) { 
    mm_game->byte_to_send |= BIT(7); // Set bit 7 for exit
    send_byte(mm_game->byte_to_send);
    mm_game->byte_to_send = 0; // Reset byte to send
    return 1; 
  }
  return 0; // Continue game
}

int process_mm_game_timer(MmGame *mm_game) {
  if (mm_game == NULL) {
    return -1; // Error
  }

  send_byte(mm_game->byte_to_send); 
  mm_game->byte_to_send = 0x00; // Reset byte to send

  int ret = check_bomb_exploded(mm_game);
  if (ret == 1) {
    return 2; // Go to winner state
  }else if (ret == -1) {
    return 1; // Go back to menu
  }

  decrease_cooldown_time(mm_game->bomb_options);
  return 0; // Continue game
}

int process_mm_game_sp(MmGame *mm_game, uint8_t byte) {
  if (mm_game == NULL) {
    return 1; // Menu
  }

  if (byte & BIT(7)) { // Exit bit
    return 1; // Go back to menu
  }
  
  bool is_some_key_pressed = false; 
  if (byte & BIT(2)) { // Up movement bit
    is_some_key_pressed = true;
    for (int i = 0; i < get_player_speed(mm_game->other_player); i++) {
      if (!check_wall_collision(mm_game, get_player_Xpos(mm_game->other_player), get_player_Ypos(mm_game->other_player) - 1)) {
        player_move_up(mm_game->other_player);
      } else {
        break;
      }
    }
  }
  if (byte & BIT(3)) { // Down movement bit
    is_some_key_pressed = true;
    for (int i = 0; i < get_player_speed(mm_game->other_player); i++) {
      if (!check_wall_collision(mm_game, get_player_Xpos(mm_game->other_player), get_player_Ypos(mm_game->other_player) + 1)) {
        player_move_down(mm_game->other_player);
      } else {
        break;
      }
    }
  }
  if (byte & BIT(4)) { // Left movement bit
    is_some_key_pressed = true;
    for (int i = 0; i < get_player_speed(mm_game->other_player); i++) {
      if (!check_wall_collision(mm_game, get_player_Xpos(mm_game->other_player) - 1, get_player_Ypos(mm_game->other_player))) {
        player_move_left(mm_game->other_player);
      } else {
        break;
      }
    }
  }
  if (byte & BIT(5)) { // Right movement bit
    is_some_key_pressed = true;
    for (int i = 0; i < get_player_speed(mm_game->other_player); i++) {
      if (!check_wall_collision(mm_game, get_player_Xpos(mm_game->other_player) + 1, get_player_Ypos(mm_game->other_player))) {
        player_move_right(mm_game->other_player);
      } else {
        break;
      }
    }
  }
  if (!is_some_key_pressed) { 
    player_stand(mm_game->other_player);
  }

  uint8_t bomb_type_bits = (byte & (BIT(0) | BIT(1))); // Get the bomb type bits
  BombType bomb_type;
  switch (bomb_type_bits) {
    case BOMB_TYPE_NORMAL: bomb_type = NORMAL; break;
    case BOMB_TYPE_FULL_LINE: bomb_type = FULL_LINE; break;
    case BOMB_TYPE_CONSTRUCTIVE: bomb_type = CONSTRUCTIVE; break;
    default: bomb_type = NONE; break;
  }

  if (is_bomb_off_cooldown(mm_game->bomb_options, bomb_type)) {
    uint16_t grid_x = (get_player_Xpos(mm_game->other_player) + 30) / mm_game->grid_square_width;
    uint16_t grid_y = (get_player_Ypos(mm_game->other_player) + 30) / mm_game->grid_square_width;

    if (!is_bomb_active(mm_game->bomb_matrix[grid_x][grid_y])) {
      set_bomb_active(mm_game->bomb_matrix[grid_x][grid_y], true, bomb_type);
      set_bomb_on_cooldown(mm_game->bomb_options, bomb_type);
    }
  }
  
  return 0; // Continue game
}

int get_winner(MmGame *mm_game) {
  if (mm_game == NULL) {
    return -1;
  }
  
  return mm_game->winner;
}
