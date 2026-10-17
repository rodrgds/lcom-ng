#include "bullet.h"
#include "xpm/tower_orange/shot_orange.xpm"

double bullet_x; 
double bullet_y;
int bullet_enemy_id[6][3] = {{-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}};
int last_time_a_bullet_was_shot[6][3] = {{-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}};
bool is_bullet_on = false;
// int time_between_shots = 700;
// int bullet_speed = 5;
// int bullet_range = 100;

bool drawBullet(int tower_x, int tower_y, enemy current_enemy, int tower_id, int enemy_id, int bullet_speed, int bullet_range, int time_between_shots, int level){
  int enemy_x = current_enemy.x;
  int enemy_y = current_enemy.y;
  bool is_enemy_alive = (current_enemy.hp > 0);
  if(bullet_enemy_id[tower_id][level - 1] != -1 && 
     enemy_id != bullet_enemy_id[tower_id][level - 1]){
    return false;
  }

  int tower_x_center =  tower_x + 15;
  int tower_y_center = tower_y + 15;
  int enemy_x_center = enemy_x + 15;
  int enemy_y_center = enemy_y + 15;
  double y_difference = enemy_y_center - tower_y_center;
  double x_difference = enemy_x_center -  tower_x_center;

  if(sqrt(y_difference*y_difference + x_difference*x_difference) <= bullet_range){
    if(bullet_enemy_id[tower_id][level - 1] == -1 && 
      is_time_interval_elapsed_milliseconds(last_time_a_bullet_was_shot[tower_id][level - 1], time_between_shots) &&
      is_enemy_alive && current_enemy.x > 0 && current_enemy.y > 0) {
      bullet_enemy_id[tower_id][level - 1] = enemy_id;
      bullet_pos[tower_id][level - 1].x = tower_x_center; 
      bullet_pos[tower_id][level - 1].y = tower_y_center;
      last_time_a_bullet_was_shot[tower_id][level - 1] = get_time_counter();
    } 
  } 

  if(bullet_enemy_id[tower_id][level - 1] == enemy_id){
    if(bullet_pos[tower_id][level - 1].x < 0 || bullet_pos[tower_id][level - 1].x > 800 || 
       bullet_pos[tower_id][level - 1].y < 0 || bullet_pos[tower_id][level - 1].y > 600){
      bullet_enemy_id[tower_id][level - 1] = -1;
      bullet_pos[tower_id][level - 1].x = tower_x_center; 
      bullet_pos[tower_id][level - 1].y = tower_y_center;
      return false;
    } if(!is_enemy_alive || current_enemy.x < 0 || current_enemy.y < 0){
      bullet_enemy_id[tower_id][level - 1] = -1;
      bullet_pos[tower_id][level - 1].x = tower_x_center; 
      bullet_pos[tower_id][level - 1].y = tower_y_center;
      return false;
    }
    draw_xpm(bullet_pos[tower_id][level - 1].x, bullet_pos[tower_id][level - 1].y, shot_orange, 0xFFFFFF);
    // bullet center = bullet coordinates + 7.5
    double move_delta_x = (bullet_pos[tower_id][level - 1].x + 7.5 - enemy_x_center)/5;
    double move_delta_y = (bullet_pos[tower_id][level - 1].y + 7.5 - enemy_y_center)/5;

    double move_delta = sqrt(move_delta_x*move_delta_x + move_delta_y*move_delta_y);
    move_delta_x /= move_delta;
    move_delta_y /= move_delta;
    bullet_pos[tower_id][level - 1].x -= bullet_speed*move_delta_x;
    bullet_pos[tower_id][level - 1].y -= bullet_speed*move_delta_y;
  }
  
  if(bullet_pos[tower_id][level - 1].x + 7.5 >= enemy_x_center - 15 && 
     bullet_pos[tower_id][level - 1].x + 7.5 <= enemy_x_center + 15 &&
     bullet_pos[tower_id][level - 1].y + 7.5 >= enemy_y_center - 15 && 
     bullet_pos[tower_id][level - 1].y + 7.5 <= enemy_y_center + 15 &&
     is_enemy_alive) {
    bullet_enemy_id[tower_id][level - 1] = -1;
    bullet_pos[tower_id][level - 1].x = tower_x_center; 
    bullet_pos[tower_id][level - 1].y = tower_y_center;
    return true;
  }
  return false;
}
