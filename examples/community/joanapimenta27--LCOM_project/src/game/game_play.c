#include "game_play.h"

extern uint32_t *game_color_array;
extern uint32_t *background_color_array;
extern sprite_t *player;
extern sprite_t *level_1_background;
extern sprite_t *level_2_background;
extern sprite_t *level_3_background;
extern sprite_t *win_main_menu;
extern sprite_t *coin_1;
extern sprite_t *coin_2;
extern sprite_t *coin_3;


sprite_t *target_low;
sprite_t *target_mid;
sprite_t *target_high;
sprite_t *target_low_pop;
sprite_t *target_mid_pop;
sprite_t *target_high_pop;

extern game_state_e game_state;
extern int selected;
extern level_state_e level_state;
int f=0;

extern unsigned int timer_counter;
extern unsigned int countdown;
extern unsigned int target_lifetime;
extern unsigned int target_spawn_chance;

unsigned int points;
unsigned int prev_points;
unsigned int min_points;

target_t targets[MAX_TARGETS];

void(move_player)(position_t pos,int i){
  game_undraw_sprite(player, game_color_array);
  if(!verify_move(pos)){
     player->position=pos;  
  }

  player->i_frame=i;
  check_coin(pos);
  game_draw_sprite(player,game_color_array);
  
 
  
  
  

  if(pos.x==80){

    if(level_state==LEVEL_1){
      if(level_1_background->i_frame<2){
        level_1_background->i_frame++;

       
      }
      f=level_1_background->i_frame;
      game_undraw_sprite(player, game_color_array);
      game_draw_sprite(level_1_background, background_color_array);
      print_min_points(get_min_points(LEVEL_1), background_color_array);
      game_draw_sprite(pause_button, game_color_array);
      game_undraw_sprite(coin_1, game_color_array);
      game_undraw_sprite(coin_2, game_color_array);
      game_undraw_sprite(coin_3, game_color_array);

      player->position=(position_t) {0, pos.y};
      if(f==1){
        coin_1->position=(position_t){13,55};
        coin_2->position=(position_t){51,42};
        coin_3->position=(position_t){51,51};
      }
      if(f==2){
        coin_1->position=(position_t){2,36};
        coin_2->position=(position_t){51,36};
        coin_3->position=(position_t){49,55};
      }
      
      game_draw_sprite(player,game_color_array);
      game_draw_sprite(coin_1,game_color_array);
      game_draw_sprite(coin_2,game_color_array);
      game_draw_sprite(coin_3,game_color_array);

    }

    if(level_state==LEVEL_2){
      if(level_2_background->i_frame<2){
        level_2_background->i_frame++;
       
      }
      f=level_2_background->i_frame;
      game_undraw_sprite(player, game_color_array);
      game_draw_sprite(level_2_background, background_color_array);
      print_min_points(get_min_points(LEVEL_2), background_color_array);
      game_undraw_sprite(coin_1, game_color_array);
      game_undraw_sprite(coin_2, game_color_array);
      game_undraw_sprite(coin_3, game_color_array);

      player->position=(position_t) {0, pos.y};
        if(f==1){
        coin_1->position=(position_t){13,55};
        coin_2->position=(position_t){51,42};
        coin_3->position=(position_t){51,51};
      }
      if(f==2){
        coin_1->position=(position_t){12,36};
        coin_2->position=(position_t){28,42};
        coin_3->position=(position_t){49,53};
      }

      game_draw_sprite(player,game_color_array);
      game_draw_sprite(coin_1,game_color_array);
      game_draw_sprite(coin_2,game_color_array);
      game_draw_sprite(coin_3,game_color_array);
    }

    if(level_state==LEVEL_3){
      if(level_3_background->i_frame<2){
        level_3_background->i_frame++;
        
      }
      f=level_3_background->i_frame;
      game_undraw_sprite(player, game_color_array);
      game_draw_sprite(level_3_background, background_color_array);
      print_min_points(get_min_points(LEVEL_3), background_color_array);
      game_draw_sprite(pause_button, game_color_array);
      game_undraw_sprite(coin_1, game_color_array);
      game_undraw_sprite(coin_2, game_color_array);
      game_undraw_sprite(coin_3, game_color_array);

      player->position=(position_t) {0, pos.y}; 
           if(f==1){
        coin_1->position=(position_t){12,52};
        coin_2->position=(position_t){51,42};
        coin_3->position=(position_t){29,44};
      }
      if(f==2){
        coin_1->position=(position_t){2,57};
        coin_2->position=(position_t){6,39};
        coin_3->position=(position_t){49,56};
      }

      game_draw_sprite(player,game_color_array);
      game_draw_sprite(coin_1,game_color_array);
      game_draw_sprite(coin_2,game_color_array);
      game_draw_sprite(coin_3,game_color_array);
    } 
  }
 
  if(f==2 && pos.x==72){
    if(points>min_points){
        game_state=WIN;
        int l;
        if(level_state==LEVEL_1){
          l=1;
        }
        if(level_state==LEVEL_2){
          l=2;
        }
        if(level_state==LEVEL_3){
          l=3;
        }
        if(verify_if_go_to_scoreboard(l,points)){
          insert_score("d",l,points);
        }
        draw_win_menu_options(0,game_color_array);
    }
    else{
      kill_player("LOW SCORE");
    }
      
        level_1_background->i_frame=0;
        level_2_background->i_frame=0;
        level_3_background->i_frame=0;
        


      
  }
}

void(check_coin)(position_t pos){
  bool top_1= (game_color_array[pos.y*H_PSEUDO_RES+pos.x]==0xFFB900 );
  bool top_2= (game_color_array[pos.y*H_PSEUDO_RES+pos.x+1]==0xFFB900 );
  bool top_3= (game_color_array[pos.y*H_PSEUDO_RES+pos.x+2]==0xFFB900 );

  bool middle_1=(game_color_array[(pos.y+1)*H_PSEUDO_RES+pos.x]==0xFFB900 );
  bool middle_2=(game_color_array[(pos.y+1)*H_PSEUDO_RES+pos.x+2]==0xFFB900);

  bool bottom_1= (game_color_array[(pos.y+2)*H_PSEUDO_RES+pos.x]==0xFFB900 );
  bool bottom_2= (game_color_array[(pos.y+2)*H_PSEUDO_RES+pos.x+1]==0xFFB900 );
  bool bottom_3= (game_color_array[(pos.y+2)*H_PSEUDO_RES+pos.x+2]==0xFFB900 );

  bool valid=top_1||top_2||top_3||middle_1||middle_2||bottom_1||bottom_2||bottom_3;
  
  if(valid){
    if(coin_1->position.x>pos.x-3 && coin_1->position.x<pos.x+3 &&coin_1->position.y>pos.y-3 && coin_1->position.y<pos.y+3){
      game_undraw_sprite(coin_1,game_color_array);
      add_points(3);
    }
    if(coin_2->position.x>pos.x-3 && coin_2->position.x<pos.x+3 &&coin_2->position.y>pos.y-3 && coin_2->position.y<pos.y+3){
    
      game_undraw_sprite(coin_2,game_color_array);
      add_points(3);
    }
   
     if(coin_3->position.x>pos.x-3 && coin_3->position.x<pos.x+3 &&coin_3->position.y>pos.y-3 && coin_3->position.y<pos.y+3){
      
      game_undraw_sprite(coin_3,game_color_array);
      add_points(3);
    }

  }

}

void (reset_player)(){
  game_undraw_sprite(player, game_color_array);
  game_undraw_sprite(coin_1,game_color_array);
  game_undraw_sprite(coin_2,game_color_array);
  game_undraw_sprite(coin_3,game_color_array);

  player->position=(position_t) {2, 34};
  player->i_frame=0;
  game_draw_sprite(player,game_color_array);
  game_draw_sprite(coin_1,game_color_array);
  game_draw_sprite(coin_2,game_color_array);
  game_draw_sprite(coin_3,game_color_array);
}

bool(verify_move)(position_t pos){
  bool area = pos.x<0 || pos.x>80 ||pos.y<=33 || pos.y>=58;
  bool top_1= (background_color_array[pos.y*H_PSEUDO_RES+pos.x]==0x734F14 );
  bool top_2= (background_color_array[pos.y*H_PSEUDO_RES+pos.x+1]==0x734F14 )&& pos.x<79;
  bool top_3= (background_color_array[pos.y*H_PSEUDO_RES+pos.x+2]==0x734F14 )&& pos.x<78;

  bool middle_1=(background_color_array[(pos.y+1)*H_PSEUDO_RES+pos.x]==0x734F14 )&& pos.x<79;
  bool middle_2=(background_color_array[(pos.y+1)*H_PSEUDO_RES+pos.x+2]==0x734F14) && pos.x<78;

  bool bottom_1= (background_color_array[(pos.y+2)*H_PSEUDO_RES+pos.x]==0x734F14 );
  bool bottom_2= (background_color_array[(pos.y+2)*H_PSEUDO_RES+pos.x+1]==0x734F14 )&& pos.x<79;
  bool bottom_3= (background_color_array[(pos.y+2)*H_PSEUDO_RES+pos.x+2]==0x734F14 )&& pos.x<78;


  return area ||top_1||top_2||top_3||middle_1||middle_2||bottom_1||bottom_2||bottom_3;
}

void (kill_player)(const char *death_str){
  increase_redness(game_color_array, 70);
  increase_redness(background_color_array, 70);
  game_draw_color_array(background_color_array);
  game_draw_color_array(game_color_array);
  game_undraw_sprite(player, game_color_array);
  player->i_frame=4;
  game_draw_sprite(player, game_color_array);
  game_state = DEAD;
  usleep(400000);
  game_undraw_sprite(player, game_color_array);
  player->i_frame=5;
  game_draw_sprite(player, game_color_array);
  draw_dead_text(death_str, game_color_array);
  draw_dead_options(selected, game_color_array);
}

void(handle_countdown)(){
  timer_counter = (timer_counter+1) % GAME_FREQUENCY;
  
  if(timer_counter%GAME_FREQUENCY==0) {
    print_points(points, prev_points, game_color_array);
    if (countdown > 0){
      countdown--;
      print_countdown(countdown, game_color_array);
    } else {
      kill_player("time is up");
    }
  }
  return;
}

void(drain_target_time)(target_t *target) {
  target->remaining_time = 0;
}

int(unspawn_targets_if_applicable)() {
  for (int i = 0; i < MAX_TARGETS; i++) {
    if (targets[i].active) {
      targets[i].remaining_time--;
      if (targets[i].remaining_time == 0 && !targets[i].popped) {
        if(game_undraw_sprite(&targets[i].sprite, game_color_array)) {
          return 1;
        } 

        targets[i].popped = true;
        targets[i].sprite.i_frame = 1;
        if(game_draw_sprite(&targets[i].sprite, game_color_array)) {
          return 1;
        } 
        
        return 0;
      }
      else if (targets[i].popped) {
        
        if (game_undraw_sprite(&targets[i].sprite, game_color_array)) {
          return 1;
        }

        targets[i].sprite.i_frame = 0;
        targets[i].active = false;
        targets[i].popped = false;
      }
    }
  }
  return 0;
}

void(prepare_level_values)(level_state_e level_state) {
  points = 0; prev_points = 0;
  countdown = LEVEL_DURATION;
  timer_counter = 0;
  switch(level_state) {
    case LEVEL_1:
      target_spawn_chance = 6;
      target_lifetime = 120;
      min_points = LEVEL_1_MIN_POINTS;
      break;
    case LEVEL_2:
      target_spawn_chance = 3;
      target_lifetime = 60;
      min_points = LEVEL_2_MIN_POINTS;
      break;
    case LEVEL_3:
      target_spawn_chance = 1;
      target_lifetime = 30;
      min_points = LEVEL_3_MIN_POINTS;
      break;
    case NON:
      break;
    default:
      break;
  }
}

void(clear_targets_array)() {
  for (int i = 0; i < MAX_TARGETS; i++) {
    targets[i].active = false;
    targets[i].remaining_time = target_lifetime;
    memset(&targets[i].sprite, 0, sizeof(sprite_t));
  }
}

int(spawn_target)(sprite_t* spr, position_t pos, int target_index) {


  memcpy(&targets[target_index].sprite, spr, sizeof(sprite_t));


  targets[target_index].sprite.position = pos;


  targets[target_index].remaining_time = target_lifetime + RANDINT(120);


  targets[target_index].active = true;


  if(game_draw_sprite(&targets[target_index].sprite, game_color_array)) {

    return 1;
  }
  return 0;
}

int (target_spawner)() {


  if(unspawn_targets_if_applicable()) {

    return 1;
  } 

  unsigned int spawn_chance = RANDINT(GAME_RAND_MAX);

  
  if (spawn_chance <= target_spawn_chance) {
    int target_index = -1;
    for (int i = 0; i < MAX_TARGETS; i++) {
        if (!targets[i].active) {
            target_index = i;
         
            break;
        }
    }

    if (target_index != -1) {
      unsigned int target_chance = RANDINT(TARGET_CHANCE_MAX);


      sprite_t* chosen_target = NULL;

      if (target_chance <= TARGET_HIGH_CHANCE) {
        chosen_target = target_high;

      } else if (target_chance <= TARGET_MID_CHANCE) {
        chosen_target = target_mid;

      } else {
        chosen_target = target_low;

      }

      int x = RANDINT(77);
      int y = 7 + RANDINT(24);
      position_t pos = {x, y};


      if (spawn_target(chosen_target, pos, target_index)) {
      return 1;
      }
    }
    }
  return 0;
}

target_t get_target(int i){
  return targets[i];
}

void (add_points)(unsigned int plus_points) {
  prev_points = points;
  if (points + plus_points >= 999) {
    points = 999;
  } else {
    points += plus_points;
  }
}

int (get_min_points)(level_state_e level_state) {
  switch(level_state) {
    case LEVEL_1:
      min_points = LEVEL_1_MIN_POINTS;
      break;
    case LEVEL_2:
      min_points = LEVEL_2_MIN_POINTS;
      break;
    case LEVEL_3:
      min_points = LEVEL_3_MIN_POINTS;
      break;
    case NON:
      min_points = -1;
      break;
    default:
      min_points = -1;
      break;
  }
  return min_points;
}
