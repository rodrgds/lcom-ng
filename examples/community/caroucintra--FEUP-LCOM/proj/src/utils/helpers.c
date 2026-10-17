
#include "helpers.h"
#include "i8042.h"
#include "game.h"
#include "xpms.h"
#include "video.h"
#include "rtc.h"

extern Game game;

Sprite *horizontal_arrow;
Sprite *background;
Sprite *background_esc;
Sprite *button_food;
Sprite *start_button;
Sprite *exit_button;
Sprite *button_bath;
Sprite *button_fun;
Sprite *button_love;
Sprite *button_sleep;
Sprite *arrow;
Sprite *hour_unit;
Sprite *hour_ten;
Sprite *min_unit;
Sprite *min_ten;
Sprite *dois_pontos;
Sprite *mouse;
Sprite *background_gameover;

Needs colored_buttons;
static Date date;

//Timer (needs counters)
int hunger_counter = 0;
int to_sleep_counter = 0;
int hygiene_counter = 0;
int love_counter = 0;
int play_counter = 0;
int minute_counter = 0;
int global_counter = 0;

int petting_counter = 0;
uint32_t movement = 0;

Sprite get_horizontal_arrow() { return *horizontal_arrow;}
Sprite get_background() { return *background;}
Sprite get_background_esc() { return *background_esc;}
Sprite get_button_food() { return *button_food;}
Sprite get_start_button() { return *start_button;}
Sprite get_exit_button() { return *exit_button;}
Sprite get_button_bath() { return *button_bath;}
Sprite get_button_fun() { return *button_fun;}
Sprite get_button_love() { return *button_love;}
Sprite get_button_sleep() { return *button_sleep;}
Sprite get_arrow() { return *arrow;}
Sprite get_hour_unit() { return *hour_unit;}
Sprite get_hour_ten() { return *hour_ten;}
Sprite get_min_unit() { return *min_unit;}
Sprite get_min_ten() { return *min_ten;}
Sprite get_dois_pontos() {return *dois_pontos;}
Sprite get_mouse() {return *mouse;}
Sprite get_violetchi() { return *game.violetchi.sprite;}

void get_key(uint8_t scancode[], int size){
    uint8_t key;    

    if (size == 1){
        key = scancode[0];
    }
    else{
        key = scancode[1];
    }
    if (game.state == GAME && !game.action){
        switch (key)
        {
        case ARROW_RIGHT:
            if (game.button == HAVE_FUN){
                game.button = FOOD;
            }
            else{
                game.button++;
            }

            break;
        case ARROW_LEFT:
            if (game.button == FOOD){
                game.button = HAVE_FUN;
            }
            else{
                game.button--;
            }
            break;
        case SPACE:
            game.action = true;
            needs_button_pressed(game.button);
            break;
        
        default:
            break;
        }
    }

    else if (game.state == MENU){
        switch (key)
        {
        case ARROW_UP:
            if (game.button == EXIT){
                game.button = PLAY_GAME;
            }
            break;

        case ARROW_DOWN:
            if (game.button == PLAY_GAME){
                game.button = EXIT;
            }
            break;

        case SPACE:
            game.action = true;
            break;
        
        default:
            break;
        }

    }
}

void needs_button_pressed(onButton game_onButton)
{
    global_counter = 0;
    switch (game_onButton)
    {
    case FOOD: reset_hunger_counter(); break;
    case ENERGY: reset_to_sleep_counter(); break;
    case BATH: reset_hygiene_counter(); break;
    case LOVE: 
      reset_petting_counter();
      break;
    case HAVE_FUN: reset_play_counter(); break;

    default: break;
    }

    do_animation();
}

void reset_petting_counter(){
    petting_counter = 0;
}

bool check_end_love(){
  if(petting_counter == PETTING_TIME && !game.mouse.action_completed){
    reset_petting_counter();
    game.action = false;
    if (update_xpm(mouse, mouse_xpm)) return 1;
    //movement = 0;
    game.mouse.in_action = false;
    return true;
  }

  else if(game.mouse.action_completed){
    if (update_xpm(mouse, mouse_xpm)) return 1;
    reset_petting_counter();
    reset_love_counter();
    return true;
  }

  return false;
}

void reduce_hunger_scale(){

    switch (game.violetchi.needs.hunger)
    {
    case HIGH:
        game.violetchi.needs.hunger = MEDIUM;
        break;

    case MEDIUM:
        game.violetchi.needs.hunger = LOW;
        break;
    
    default:
        break;
    }
    
}

void reset_hunger_counter(){
    hunger_counter = 0;
    game.violetchi.needs.hunger = HIGH;
}

void reduce_to_sleep_scale(){
    
    switch (game.violetchi.needs.to_sleep)
    {
    case HIGH:
        game.violetchi.needs.to_sleep = MEDIUM;
        break;

    case MEDIUM:
        game.violetchi.needs.to_sleep = LOW;
        break;
    
    default:
        break;
    }
    
}

void reset_to_sleep_counter(){
    to_sleep_counter = 0;
    game.violetchi.needs.to_sleep = HIGH;
}

void reduce_hygiene_scale(){

    switch (game.violetchi.needs.hygiene)
    {
    case HIGH:
        game.violetchi.needs.hygiene = MEDIUM;
        break;

    case MEDIUM:
        game.violetchi.needs.hygiene = LOW; 
        break;
    
    default:
        break;
    }                                       
    
}

void reset_hygiene_counter(){
    hygiene_counter = 0;
    game.violetchi.needs.hygiene = HIGH;
}

void reduce_love_scale(){
 
    switch (game.violetchi.needs.love)
    {
    case HIGH:
        game.violetchi.needs.love = MEDIUM;
        break;

    case MEDIUM:
        game.violetchi.needs.love = LOW;
    
        break;
    
    default:
        break;
    }   
    
}

void reset_love_counter(){
    love_counter = 0;
    game.violetchi.needs.love = HIGH;
}

void reduce_play_scale(){
    
    switch (game.violetchi.needs.play)
    {
    case HIGH:
        game.violetchi.needs.play = MEDIUM;
        break;

    case MEDIUM:
        game.violetchi.needs.play = LOW;
    
        break;
    
    default:
        break;
    }
    
}

void reset_play_counter(){
    play_counter = 0;
    game.violetchi.needs.play = HIGH;
}

bool verify_timer_events()
{
    bool event_occured = false;
    if(hunger_counter == HUNGER_TIME){
        reduce_hunger_scale();
        hunger_counter = 0;
        event_occured = true;
    }

    if(to_sleep_counter == SLEEP_TIME){
        reduce_to_sleep_scale();
        to_sleep_counter = 0;
        event_occured = true;
    }

    if(hygiene_counter == BATH_TIME){
        reduce_hygiene_scale();
        hygiene_counter = 0;
        event_occured = true;
    }

    if(love_counter == LOVE_TIME){
        reduce_love_scale();
        love_counter = 0;
        event_occured = true;
    }

    if(play_counter == PLAY_TIME){
        reduce_play_scale();
        play_counter = 0;
        event_occured = true;
    }

    if(minute_counter == MINUTE){
      minute_counter = 0;
      event_occured = true;
    }

    //update_buttons(game.violetchi.needs);
    if(event_occured){
      set_violetchi_mood(&(game.violetchi));
      if (!game.action) update_violetchi(&(game.violetchi));
    }

    return event_occured;
}

void (set_clock_min_unit)(uint8_t min){
    switch (min % 10)
    {
    case 0: update_xpm(min_unit, n0_xpm); break;
    case 1: update_xpm(min_unit, n1_xpm); break;
    case 2: update_xpm(min_unit, n2_xpm); break;
    case 3: update_xpm(min_unit, n3_xpm); break;
    case 4: update_xpm(min_unit, n4_xpm); break;
    case 5: update_xpm(min_unit, n5_xpm); break;
    case 6: update_xpm(min_unit, n6_xpm); break;
    case 7: update_xpm(min_unit, n7_xpm); break;
    case 8: update_xpm(min_unit, n8_xpm); break;
    case 9: update_xpm(min_unit, n9_xpm); break;

    default:
      break;
    }
}

void (set_clock_min_ten)(uint8_t min){
    switch ((int)(min / 10))
    {
    case 0: update_xpm(min_ten, n0_xpm); break;
    case 1: update_xpm(min_ten, n1_xpm); break;
    case 2: update_xpm(min_ten, n2_xpm); break;
    case 3: update_xpm(min_ten, n3_xpm); break;
    case 4: update_xpm(min_ten, n4_xpm); break;
    case 5: update_xpm(min_ten, n5_xpm); break;
    case 6: update_xpm(min_ten, n6_xpm); break;
    case 7: update_xpm(min_ten, n7_xpm); break;
    case 8: update_xpm(min_ten, n8_xpm); break;
    case 9: update_xpm(min_ten, n9_xpm); break;

    default:
      break;
    }
}

void (set_clock_hour_unit)(uint8_t hour){
    switch (hour % 10)
    {
    case 0: update_xpm(hour_unit, n0_xpm); break;
    case 1: update_xpm(hour_unit, n1_xpm); break;
    case 2: update_xpm(hour_unit, n2_xpm); break;
    case 3: update_xpm(hour_unit, n3_xpm); break;
    case 4: update_xpm(hour_unit, n4_xpm); break;
    case 5: update_xpm(hour_unit, n5_xpm); break;
    case 6: update_xpm(hour_unit, n6_xpm); break;
    case 7: update_xpm(hour_unit, n7_xpm); break;
    case 8: update_xpm(hour_unit, n8_xpm); break;
    case 9: update_xpm(hour_unit, n9_xpm); break;

    default:
      break;
    }
}

void (set_clock_hour_ten)(uint8_t hour){
    switch ((int)(hour / 10))
    {
    case 0: update_xpm(hour_ten, n0_xpm); break;
    case 1: update_xpm(hour_ten, n1_xpm); break;
    case 2: update_xpm(hour_ten, n2_xpm); break;
    case 3: update_xpm(hour_ten, n3_xpm); break;
    case 4: update_xpm(hour_ten, n4_xpm); break;
    case 5: update_xpm(hour_ten, n5_xpm); break;
    case 6: update_xpm(hour_ten, n6_xpm); break;
    case 7: update_xpm(hour_ten, n7_xpm); break;
    case 8: update_xpm(hour_ten, n8_xpm); break;
    case 9: update_xpm(hour_ten, n9_xpm); break;

    default:
      break;
    }
}

void (update_clock)(){
  date = get_date();
  set_clock_hour_ten(date.hour);
  set_clock_hour_unit(date.hour);
  set_clock_min_ten(date.minute);
  set_clock_min_unit(date.minute);
}

int (draw_clock)(){
  update_clock();
  if(draw_sprite(hour_ten)) return 1;
  if(draw_sprite(hour_unit)) return 1;
  if(draw_sprite(min_ten)) return 1;
  if(draw_sprite(min_unit)) return 1;
  if(draw_sprite(dois_pontos)) return 1;
  
  return 0;
}

void (initialize_scene)(){

  bool night = false;
  if (get_hour() >= 19) night = true;

  //menu
  if (night) background = create_sprite(bg2_night2, 0, 0, 0, 0);
  else background = create_sprite(bg2, 0, 0, 0, 0);
  horizontal_arrow = create_sprite(h_arrow, 200, 125, 0, 230);
  start_button = create_sprite(start_button_xpm, 320, 100, 0, 0);
  exit_button = create_sprite(exit_button_xpm, 320, 300, 0, 0);

  //game
  if (night) background_esc = create_sprite(bg2_night2_esc, 0, 0, 0, 0);
  else background_esc = create_sprite(bg2_esc, 0, 0, 0, 0);
  button_food = create_sprite(food_button_green, 100, 620, 0, 0);
  button_sleep = create_sprite(sleep_button_green, 249, 620, 0, 0);
  button_bath = create_sprite(soap_button_green, 398, 620, 0, 0);
  button_love = create_sprite(love_button_green, 547, 620, 0, 0);
  button_fun = create_sprite(play_button_green, 696, 620, 0, 0);
  arrow = create_sprite(arrow_xpm, 124, 750, 0, 0);

  //clock
  init_mode();
  min_unit = create_sprite(n0_xpm, get_hres()/2 + 40, 20, 0, 0);
  min_ten = create_sprite(n0_xpm, min_unit->x - 50, 20, 0, 0);
  hour_unit = create_sprite(n0_xpm, min_ten->x - 70, 20, 0, 0);
  hour_ten = create_sprite(n0_xpm, hour_unit->x - 50, 20, 0, 0);
  dois_pontos = create_sprite(dois_pontos_xpm, min_ten->x - 68, 0, 0, 0);
  mouse = create_sprite(mouse_xpm, game.mouse.x, game.mouse.y, 0, 0);
  colored_buttons.hunger = HIGH;
  colored_buttons.to_sleep = HIGH;
  colored_buttons.hygiene = HIGH;
  colored_buttons.love = HIGH;
  colored_buttons.play = HIGH;

  //end
  background_gameover = create_sprite(bg2_game_over, 0, 0, 0, 0);
}


int (draw_menu)(){
  if (draw_sprite(background)) return 1;
  if (draw_sprite(horizontal_arrow)) return 1;
  if (draw_sprite(start_button)) return 1;
  if (draw_sprite(exit_button)) return 1;
  
  return 0;
}


int (draw_bg)(){
  if (draw_sprite(background_esc)) return 1;
  if (draw_buttons()) return 1;
  if (draw_sprite(arrow)) return 1;
  
  return 0;
}

int (draw_game_over)(){
  if (draw_sprite(background_gameover)) return 1;
  
  return 0;
}

int (draw_buttons)(){
  update_buttons(game.violetchi.needs);
  if (draw_sprite(button_food)) return 1;
  if (draw_sprite(button_sleep)) return 1;
  if (draw_sprite(button_bath)) return 1;
  if (draw_sprite(button_love)) return 1;
  if (draw_sprite(button_fun)) return 1;
  
  return 0;
}

int (draw_mouse)(){
  update_mouse();
  if (draw_sprite(mouse)) return 1;
  
  return 0;
}

void (update_mouse)(){
  update_position(mouse, game.mouse.x, game.mouse.y);
}

void (update_arrow)(){
    if(game.state == GAME){
        update_position(arrow, 149*(game.button+1)-25, arrow->y);
    }

    else if(game.state == MENU){
        if(game.button == PLAY_GAME){
            update_position(horizontal_arrow, horizontal_arrow->x, 125);
        }
        else if(game.button == EXIT){
            update_position(horizontal_arrow, horizontal_arrow->x, 325);
        }
    }
}

void (update_buttons)(Needs violet_needs){
  if (violet_needs.hunger != colored_buttons.hunger){
    switch (violet_needs.hunger){
    case HIGH:
      update_xpm(button_food, food_button_green);
      colored_buttons.hunger = HIGH;
      break;
    case MEDIUM:
      update_xpm(button_food, food_button_yellow);
      colored_buttons.hunger = MEDIUM;
      break;
    case LOW:
      update_xpm(button_food, food_button_red);
      colored_buttons.hunger = LOW;
      break;
    default:
      break;
    }
  }
  if (violet_needs.to_sleep != colored_buttons.to_sleep){
    switch (violet_needs.to_sleep){
    case HIGH:
      update_xpm(button_sleep, sleep_button_green);
      colored_buttons.to_sleep = HIGH;
      break;
    case MEDIUM:
      update_xpm(button_sleep, sleep_button_yellow);
      colored_buttons.to_sleep = MEDIUM;
      break;
    case LOW:
      update_xpm(button_sleep, sleep_button_red);
      colored_buttons.to_sleep = LOW;
      break;
    default:
      break;
    }
  }
  if (violet_needs.hygiene != colored_buttons.hygiene){
    switch (violet_needs.hygiene){
    case HIGH:
      update_xpm(button_bath, soap_button_green);
      colored_buttons.hygiene = HIGH;
      break;
    case MEDIUM:
      update_xpm(button_bath, soap_button_yellow);
      colored_buttons.hygiene = MEDIUM;
      break;
    case LOW:
      update_xpm(button_bath, soap_button_red);
      colored_buttons.hygiene = LOW;
      break;
    default:
      break;
    }
  }
  if (violet_needs.love != colored_buttons.love){
    switch (violet_needs.love){
    case HIGH:
      update_xpm(button_love, love_button_green);
      colored_buttons.love = HIGH;
      break;
    case MEDIUM:
      update_xpm(button_love, love_button_yellow);
      colored_buttons.love = MEDIUM;
      break;
    case LOW:
      update_xpm(button_love, love_button_red);
      colored_buttons.love = LOW;
      break;
    default:
      break;
    }
  }
  if (violet_needs.play != colored_buttons.play){
    switch (violet_needs.play){
    case HIGH:
      update_xpm(button_fun, play_button_green);
      colored_buttons.play = HIGH;
      break;
    case MEDIUM:
      update_xpm(button_fun, play_button_yellow);
      colored_buttons.play = MEDIUM;
      break;
    case LOW:
      update_xpm(button_fun, play_button_red);
      colored_buttons.play = LOW;
      break;
    default:
      break;
    }
  }
}

int change_animation(){
  switch (game.button)
  {
  case FOOD:
    if (update_xpm(game.violetchi.sprite, violetchi4_eating2)) return 1;
    if (draw_bg()) return 1;
    if (draw_sprite(game.violetchi.sprite)) return 1;
    break;
  case HAVE_FUN:
    if (update_xpm(game.violetchi.sprite, violetchi4_play2)) return 1;
    if (draw_bg()) return 1;
    if (draw_sprite(game.violetchi.sprite)) return 1;
    break;
  default:
    break;
  }
  
  return 0;
}

int do_animation(){
  switch (game.button)
  {
  case FOOD:
    if (update_xpm(game.violetchi.sprite, violetchi4_eating)) return 1;
    if (draw_bg()) return 1;
    if (draw_sprite(game.violetchi.sprite)) return 1;
    break;
  case ENERGY:
    if (update_xpm(game.violetchi.sprite, violetchi4_sleeping)) return 1;
    if (draw_sprite(game.violetchi.sprite)) return 1;
    break;
  case BATH:
    if (update_xpm(game.violetchi.sprite, violetchi4_bath)) return 1;
    if (draw_sprite(game.violetchi.sprite)) return 1;
    break;
  case HAVE_FUN:
    if (update_xpm(game.violetchi.sprite, violetchi4_play)) return 1;
    if (draw_bg()) return 1;
    if (draw_sprite(game.violetchi.sprite)) return 1;
    break;
  case LOVE:
    if (update_xpm(mouse, hand)) return 1;
  default:
    break;
  }
  
  return 0;

}

void reset_violetchi(){

  set_violetchi_mood(&(game.violetchi));
  update_violetchi(&(game.violetchi));
  draw_sprite(game.violetchi.sprite);

  if (game.button != LOVE) game.action = false;
}

int love_animation(){
  draw_mouse();
  if (update_xpm(game.violetchi.sprite, violetchi4_love)) return 1;
  if (draw_sprite(game.violetchi.sprite)) return 1;
  
  //tickdelay(micros_to_ticks(DELAY));
  //set_violetchi_mood(&(game.violetchi));
  update_violetchi(&(game.violetchi));
  return 0;
}

void reset_timer_counters(){
  reset_hunger_counter();
  reset_hygiene_counter();
  reset_love_counter();
  reset_play_counter();
  reset_to_sleep_counter();
  date = get_date();
  minute_counter = date.second * 60;
}


int start_scene(Game game){
  if (game.state == GAME){

    if (draw_bg()) return 1;
    if (draw_clock()) return 1;
    if (draw_sprite(game.violetchi.sprite)) return 1;
    if (draw_mouse()) return 1;
  }

  else if(game.state == MENU){
    if (draw_menu()) return 1;
  }

  else if (game.state == END){
    if (draw_game_over()) return 1;
  }
  
  return 0;
}


void mouse_handle_pos(){
  if ((game.mouse.x + game.mouse.packet->delta_x < (int)get_hres()) && (game.mouse.x + game.mouse.packet->delta_x > 0) 
  &&  !(game.mouse.packet->x_ov )) {

    game.mouse.x += game.mouse.packet->delta_x;
  }
  else game.mouse.packet->delta_x = 0;
  if ((game.mouse.y - game.mouse.packet->delta_y <  (int)get_vres()) && (game.mouse.y - game.mouse.packet->delta_y > 0) 
    && !(game.mouse.packet->y_ov ) ) {

      game.mouse.y -= game.mouse.packet->delta_y;
  }
  else game.mouse.packet->delta_y = 0;
}

void check_action(){
  if (game.state == GAME){
    if (game.button == LOVE && game.action ){
      if (game.mouse.x >= game.violetchi.sprite->x && 
      game.mouse.x <=  game.violetchi.sprite->x + VIOLETCHI_WIDTH && 
      game.mouse.y >= game.violetchi.sprite->y && 
      game.mouse.y <=  game.violetchi.sprite->y + VIOLETCHI_HEIGHT && 
      game.mouse.packet->lb){

        movement += abs(game.mouse.packet->delta_x) + abs(game.mouse.packet->delta_y);
        game.mouse.in_action = true;
      }
      else{
        game.mouse.in_action = false;
      }
      if (movement > MIN_LOVE_MOVEM){
        game.mouse.action_completed = true;
        game.mouse.in_action = false;
        movement = 0;
      }
    }
  }
}
