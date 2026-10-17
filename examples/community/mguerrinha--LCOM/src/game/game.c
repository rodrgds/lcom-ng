#include "game.h"

extern int timer_counter;
extern MenuState menuState;
extern Arena arena_matrix;
extern PlayerInfo player1_info;
extern PlayerInfo player2_info;
int p1_counter_territory;
int p2_counter_territory;

void (move_player)(PlayerInfo *player_info, enum direction *aux_dir) {
  int player_territory = (player_info->id == 1) ? 1 : 2;
  int player_trail = (player_info->id == 1) ? 3 : 4;
  if ((player_info->dir == UP || player_info->dir == DOWN || player_info->dir == STOP) && player_info->y % 20 == 0 && *aux_dir != 4 && (player_info->x > 0  || *aux_dir == RIGHT) && (player_info->x < 780 || *aux_dir == LEFT)) {
    player_info->dir = *aux_dir;
    *aux_dir = 4;
  }
  else if ((player_info->dir == LEFT || player_info->dir == RIGHT || player_info->dir == STOP) && player_info->x % 20 == 0 && *aux_dir != 4 && (player_info->y > 0 || *aux_dir == DOWN) && (player_info->y < 540 || *aux_dir == UP)) {
    player_info->dir = *aux_dir;
    *aux_dir = 4;
  }
  
  if (menuState == PLAY) {
    switch (player_info->dir)
    {
      case UP:
        move_player_up(player_info);
        if (arena_matrix.map[player_info->y+20][player_info->x] != player_territory) {
          player_info->out_of_territory = true;
          for (unsigned int i = player_info->x; i < player_info->x+20; i++) {
            arena_matrix.map[player_info->y+20][i] = player_trail;
          }
        }
        else if (player_info->out_of_territory == true) {
          player_info->out_of_territory = false;
          fill_territory(player_info);
          count_territory();
        }
        break;

      case LEFT:
        move_player_left(player_info);
        if (arena_matrix.map[player_info->y][player_info->x+20] != player_territory) {
          player_info->out_of_territory = true;
          for (unsigned int i = player_info->y; i < player_info->y+20; i++) {
            arena_matrix.map[i][player_info->x+20] = player_trail;
          }
        }
        else if (player_info->out_of_territory == true) {
          player_info->out_of_territory = false;
          fill_territory(player_info);
          count_territory();
        }
        break;

      case DOWN:
        move_player_down(player_info);        
        if (arena_matrix.map[player_info->y-1][player_info->x] != player_territory) {
          player_info->out_of_territory = true;
          for (unsigned int i = player_info->x; i < player_info->x+20; i++) {
            arena_matrix.map[player_info->y-1][i] = player_trail;
          }
        }
        else if (player_info->out_of_territory == true) {
          player_info->out_of_territory = false;
          fill_territory(player_info);
          count_territory();
        }
        break;
        
      case RIGHT:
        move_player_right(player_info);
        if (arena_matrix.map[player_info->y][player_info->x-1] != player_territory) {
          player_info->out_of_territory = true;
          for (unsigned int i = player_info->y; i < player_info->y+20; i++) {
            arena_matrix.map[i][player_info->x-1] = player_trail;
          }
        }
        else if (player_info->out_of_territory == true) {
          player_info->out_of_territory = false;
          fill_territory(player_info);
          count_territory();
        }
        break;
      default:
        break;
    }
  player_collisions(player_info);
  }
}

void (player_collisions)(PlayerInfo *player_info) {
  if ((player_info->id == 1 && arena_matrix.map[player_info->y][player_info->x] == 3)) {
    for (unsigned int h = 0; h < arena_matrix.height; h++) {
      for (unsigned int w = 0; w < arena_matrix.width; w++) {
        if ((arena_matrix.map[h][w] == 3) || (arena_matrix.map[h][w] == 1)) arena_matrix.map[h][w] = 0;
      }
    }
    player1_info.x = 200;
    player1_info.y = 300;
    for (int i = player1_info.y - 20; i < player1_info.y + 40; i++) {
      for (int j = player1_info.x - 20; j < player1_info.x + 40; j++) {
        arena_matrix.map[i][j] = 1;
      }
    }
    player1_info.dir = STOP;
  }
  if ((player_info->id == 2 && arena_matrix.map[player_info->y][player_info->x] == 4)) {
    for (unsigned int h = 0; h < arena_matrix.height; h++) {
      for (unsigned int w = 0; w < arena_matrix.width; w++) {
        if ((arena_matrix.map[h][w] == 4) || (arena_matrix.map[h][w] == 2)) arena_matrix.map[h][w] = 0;
      }
    }
    player2_info.x = 600;
    player2_info.y = 300;
    for (int i = player2_info.y - 20; i < player2_info.y + 40; i++) {
      for (int j = player2_info.x - 20; j < player2_info.x + 40; j++) {
        arena_matrix.map[i][j] = 2;
      }
    }
    player2_info.dir = STOP;
  }
  else if (player_info->id == 1 && arena_matrix.map[player_info->y][player_info->x] == 4) {
    for (unsigned int h = 0; h < arena_matrix.height; h++) {
      for (unsigned int w = 0; w < arena_matrix.width; w++) {
        if ((arena_matrix.map[h][w] == 4) || (arena_matrix.map[h][w] == 2)) arena_matrix.map[h][w] = 0;
      }
    }
    player2_info.x = 600;
    player2_info.y = 300;
    for (int i = player2_info.y - 20; i < player2_info.y + 40; i++) {
      for (int j = player2_info.x - 20; j < player2_info.x + 40; j++) {
        arena_matrix.map[i][j] = 2;
        }
    }
    player2_info.dir = STOP;
  }
  else if (player_info->id == 2 && arena_matrix.map[player_info->y][player_info->x] == 3) {
    for (unsigned int h = 0; h < arena_matrix.height; h++) {
      for (unsigned int w = 0; w < arena_matrix.width; w++) {
        if ((arena_matrix.map[h][w] == 3) || (arena_matrix.map[h][w] == 1)) arena_matrix.map[h][w] = 0;
        
      }
    }
    player1_info.x = 200;
    player1_info.y = 300;
    for (int i = player1_info.y - 20; i < player1_info.y + 40; i++) {
      for (int j = player1_info.x - 20; j < player1_info.x + 40; j++) {
        arena_matrix.map[i][j] = 1;
      }
    }
    player1_info.dir = STOP;
  }
}

void (fill_territory)(PlayerInfo *player_info) {
  int player_territory = (player_info->id == 1) ? 1 : 2;
  int player_trail = (player_info->id == 1) ? 3 : 4;
  bool down = false;
  bool left = false;
  bool right = false;
  bool up = false;

  int enemie_counter = 0;
  unsigned int ymin = UINT_MAX;
  unsigned int ymax = 0;
  unsigned int xmin = UINT_MAX;
  unsigned int xmax = 0;

  for (unsigned int h = 0; h < arena_matrix.height; h++) {
    for (unsigned int w = 0; w < arena_matrix.width; w++) {
      if(arena_matrix.map[h][w] == player_trail) arena_matrix.map[h][w] = player_territory;
      if((arena_matrix.map[h][w] == player_territory) || (arena_matrix.map[h][w] == player_trail)){
        if(h<ymin) ymin = h;
        if(h>ymax) ymax = h;
        if(w<xmin) xmin = w;
        if(w>xmax) xmax = w;
      }
    }
  }  


  for (unsigned int h = ymin; h < ymax+1; h++) {
    for (unsigned int w = xmin; w < xmax+1; w++) {
        for(unsigned int i=ymin; i<=h; i++){
          if((arena_matrix.map[i][w] == player_territory) || (arena_matrix.map[i][w] == player_trail)){
            down = true;
            break;
          }
        }
        for(unsigned int i=xmin; i<w; i++){
          if((arena_matrix.map[h][i] == player_territory) || (arena_matrix.map[h][i] == player_trail)){
            left = true;
            break;
          }
        }
        for(unsigned int i=w; i<xmax+1; i++){
          if((arena_matrix.map[h][i] == player_territory) || (arena_matrix.map[h][i] == player_trail)){
            right = true;
            break;
          }
        }
        for(unsigned int i=h; i<ymax+1; i++){
          if((arena_matrix.map[i][w] == player_territory) || (arena_matrix.map[i][w] == player_trail)){
            up = true;
            break;
          }
        }

        if(down && left && right && up){
          arena_matrix.map[h][w] = player_territory;
        }



        down = false;
        left = false;
        right = false;
        up = false; 
      
    }

  }
  for (unsigned int h = 0; h < arena_matrix.height; h++) {
    for (unsigned int w = 0; w < arena_matrix.width; w++) {
      if(player_info->id==1){
        if(arena_matrix.map[h][w] == 2) enemie_counter++;
      }
      else{
        if(arena_matrix.map[h][w] == 1) enemie_counter++;
      }
    }
  }
  if(enemie_counter==0){
    int player_x = player_info->x;
    int player_y = player_info->y;
    if(player_info->id == 1){
      player_info->x = 600;
      player_info->y = 300;
      arena_matrix.map[player_info->y][player_info->x] = 4;
    }
    else{
      player_info->x = 200;
      player_info->y = 300;
      arena_matrix.map[player_info->y][player_info->x] = 3;
    }
    player_collisions(player_info);
    player_info->x = player_x;
    player_info->y = player_y;
  }  

}

void (count_territory)() {
  int total_territory = 800 * 560;
  p1_counter_territory = 0;
  p2_counter_territory = 0;
  for (unsigned int h = 0; h < arena_matrix.height; h++) {
    for (unsigned int w = 0; w < arena_matrix.width; w++) {
      if (arena_matrix.map[h][w] == 1) {
        p1_counter_territory++;
      }
      else if (arena_matrix.map[h][w] == 2) {
        p2_counter_territory++;
      }
    }
  }
  p1_counter_territory = ((float)p1_counter_territory / total_territory) * 100;
  p2_counter_territory = ((float)p2_counter_territory / total_territory) * 100;
}
