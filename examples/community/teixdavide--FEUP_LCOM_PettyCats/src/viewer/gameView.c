#include "gameView.h"
#include "../models/PlayerInfo.h"

#include "../images/cats/cat0.xpm"
#include "../images/cats/cat1.xpm"
#include "../images/cats/cat2.xpm"
#include "../images/cats/cat3.xpm"
#include "../images/cats/cat4.xpm"
#include "../images/character/front.xpm"
#include "../images/scenery/cloud.xpm"
#include "../images/menu/logo.xpm"
#include "../images/mouse/mouse.xpm"
#include "../images/menu/play_game.xpm"
#include "../images/menu/instructions.xpm"
#include "../images/menu/quit.xpm"
#include "../images/state_machines/state_machine0.xpm"
#include "../images/state_machines/state_machine1.xpm"
#include "../images/state_machines/state_machine2.xpm"
#include "../images/state_machines/state_machine3.xpm"
#include "../images/state_machines/state_machine4.xpm"

#define YELLOW 62
#define BLUE 9
#define LIGHT_BLUE 11
#define SKY_DAY 27
#define SKY_NIGHT 8
#define GREEN_GRASS 2
#define CAT_BROWN 20
#define CAT_RED 44
#define CAT_BLUE 15
#define CAT_GRAY 7
#define CAT_ORANGE 52


extern int mouse_pos_x, mouse_pos_y;
extern uint16_t x_res, y_res;
extern bool hasLeftClick;
extern Datetime datetime;
extern Entity* selectedCat;

char score_string[20];


int drawGame(Game *game) {

  if (game->state == MENU_STATE) {
    if (drawMenu(game) != 0) return 1;
  } else if (game->state == GAME_STATE){
    if (drawGamePlaying(game) != 0) return 1;
  } else if (game->state == INSTRUCTIONS_STATE){
    if (drawInstructions() != 0) return 1;
  }

  return 0;
}

int drawMenu(Game *game) {
  // Background color of the menu (YELLOW)
  if (set_background_color(0x105, YELLOW) != 0) return 1;

  // if (draw_xpm((xpm_map_t) logo, 512 - 458 / 2, 100) != 0) return 1;
  if (draw_sprite(menu_logo, 512 - 458 / 2, 100) != 0) return 1;

  // TODO: CHANGE THIS FROM HARD CODED TO VARIOUS IN GAME DATA STRUCTURE

  //drawButton(const char text[], int x_center, int y_center, int width, int height, uint8_t color)
  if (mouse_pos_x >= 512 - 150 && mouse_pos_x <= 512 + 150 && mouse_pos_y >= 300 - 50 && mouse_pos_y <= 300 + 50) {
    if (drawButton("Play Game", 512, 300, 300, 100, BLUE) != 0) return 1;
    if (hasLeftClick) {
      game->state = GAME_STATE;
    }
  } else {
    if (drawButton("Play Game", 512, 300, 300, 100, BLUE + 1) != 0) return 1;
  }

  if (mouse_pos_x >= 512 - 150 && mouse_pos_x <= 512 + 150 && mouse_pos_y >= 400 - 40 && mouse_pos_y <= 400 + 40) {
    if (drawButton("Instructions", 512, 400, 300, 80, BLUE) != 0) return 1;
    if (hasLeftClick) {
      game->state = INSTRUCTIONS_STATE;
    }
  } else {
    if (drawButton("Instructions", 512, 400, 300, 80, BLUE + 1) != 0) return 1;
  }

  if (mouse_pos_x >= 512 - 150 && mouse_pos_x <= 512 + 150 && mouse_pos_y >= 500 - 40 && mouse_pos_y <= 500 + 40) {
    if (drawButton("Quit", 512, 500, 300, 80, BLUE) != 0) return 1;
    if (hasLeftClick) {
      game->state = LEAVE_STATE;
    }
  } else {
    if (drawButton("Quit", 512, 500, 300, 80, BLUE + 1) != 0) return 1;
  }

  if (drawMouse(mouse_pos_x, mouse_pos_y) != 0) {
    printf("Error drawing mouse");
    return 1;
  }

  if (update_front_buffer() != 0) {
    return 1;
  }

  return 0;
}

int drawGamePlaying(Game *game) {
  // Background drawing

  if (((PlayerInfo*) game->room->player->typeInfo)->isPetting) {
    if (drawPetting(game) != 0) return 1;
    if (drawMouse(mouse_pos_x, mouse_pos_y) != 0) {
      printf("Error drawing mouse");
      return 1;
    }
    return update_front_buffer();
  }

  if (drawGrass()) return 1;
  if (drawSky()) return 1;
  if (drawClouds(game->room->clouds)) return 1;

  // Cats drawing

  for (int i = 0; i < 10; i++) {
    if (drawCat(game->room->cats[i]) != 0) return 1;
  }

  // Player drawing
  if (draw_xpm((xpm_map_t) front, game->room->player->position->x, game->room->player->position->y) != 0) return 1;

  sprintf(score_string, "%d", game->counter);
  if (draw_text(score_string, 0, 0, 0) != 0) return 1;

  if (drawMouse(mouse_pos_x, mouse_pos_y) != 0) {
    printf("Error drawing mouse");
    return 1;
  }

  // copy image to main buffer
  if (update_front_buffer() != 0) {
    return 1;
  }

  return 0;
}

int drawClouds(Cloud clouds[]) {
  for (int i = 0; i < CLOUD_QNT; i++) {
    drawCloud(&clouds[i]);
  }

  return 0;
}

int drawCloud(Cloud *cloud) {
  draw_xpm((xpm_map_t) cloud0, cloud->x, cloud->y);
  if (cloud->x + cloud_width > x_res) {
    draw_xpm((xpm_map_t) cloud0, cloud->x - x_res, cloud->y);
  }
  if (cloud->x > x_res) {
    cloud->x = 0;
  }
  cloud->x += cloud->velocity/5;

  return 0;
}

int drawGrass() {
  if (draw_rectangle(0x105, 0, y_res * SKY_SPLIT, x_res, y_res * (1 - SKY_SPLIT), GREEN_GRASS) != 0) {
    return 1;
  }

  return 0;
}

int drawSky() {
  uint16_t sky_color;

  if (datetime.hours >= 21 && datetime.hours < 7) {
    sky_color = SKY_NIGHT;
  } else {
    sky_color = SKY_DAY;
  }

  if (draw_rectangle(0x105, 0, 0, x_res, y_res * SKY_SPLIT, sky_color) != 0) {
    return 1;
  }

  return 0;
}

int drawCat(Entity *entity) {
  // TODO switch case to get cat type
  switch (((CatInfo*) entity->typeInfo)->color)
  {
  case RED_CAT:
    if (draw_xpm((xpm_map_t) cat0, entity->position->x, entity->position->y) != 0) return 1;
    break;

  case BROWN_CAT:
    if (draw_xpm((xpm_map_t) cat1, entity->position->x, entity->position->y) != 0) return 1;
    break;
    //return draw_xpm((xpm_map_t) cat1, entity->position->x, entity->position->y);

  case BLUE_CAT:
    if (draw_xpm((xpm_map_t) cat2, entity->position->x, entity->position->y) != 0) return 1;
    break;
    //return draw_xpm((xpm_map_t) cat2, entity->position->x, entity->position->y);
  
  case GRAY_CAT:
    if (draw_xpm((xpm_map_t) cat3, entity->position->x, entity->position->y) != 0) return 1;
    break;
    //return draw_xpm((xpm_map_t) cat3, entity->position->x, entity->position->y);
  
  case ORANGE_CAT:
    if (draw_xpm((xpm_map_t) cat4, entity->position->x, entity->position->y) != 0) return 1;
    break;
    //return draw_xpm((xpm_map_t) cat4, entity->position->x, entity->position->y);
  
  default:
    return 1;
  }

  if (((CatInfo*)entity->typeInfo)->isSelected) return draw_rectangle(0x105, entity->position->x, entity->position->y, 10, 10, YELLOW);

  return 0;
}

int drawInstructions(){
  if (draw_rectangle(0x105, 0, 0, x_res, y_res, LIGHT_BLUE) != 0) return 1;
  if (draw_xpm((xpm_map_t) instructions, 350, 10) != 0) return 1;

  if (draw_xpm((xpm_map_t) cat0, 100, 300) != 0) return 1;
  if (draw_xpm((xpm_map_t) state_machine0, 50, 350) != 0) return 1;

  if (draw_xpm((xpm_map_t) cat1, 280, 300) != 0) return 1;
  if (draw_xpm((xpm_map_t) state_machine1, 230, 350) != 0) return 1;

  if (draw_xpm((xpm_map_t) cat2, 460, 300) != 0) return 1;
  if (draw_xpm((xpm_map_t) state_machine2, 410, 350) != 0) return 1;

  if (draw_xpm((xpm_map_t) cat3, 640, 300) != 0) return 1;
  if (draw_xpm((xpm_map_t) state_machine3, 590, 350) != 0) return 1;

  if (draw_xpm((xpm_map_t) cat4, 820, 300) != 0) return 1;
  if (draw_xpm((xpm_map_t) state_machine4, 770, 350) != 0) return 1;
  
  if (update_front_buffer() != 0) {
    return 1;
  }
  return 0;
}

int drawButton(const char text[], int x_center, int y_center, int width, int height, uint8_t color) {
  int x_init = x_center - width / 2;
  int y_init = y_center - height / 2;

  if (draw_rectangle(0x105, x_init, y_init, width, height, color) != 0) {
    return 1;
  }

  if (text[0] == 'P'){
    // if (draw_xpm((xpm_map_t) play_game, x_init, y_init) != 0) return 1;
    if (draw_sprite(menu_play_game, x_init, y_init) != 0) return 1;
  } else if (text[0] == 'I'){
    // if (draw_xpm((xpm_map_t) instructions, x_init, y_init) != 0) return 1;
    if (draw_sprite(menu_instructions, x_init, y_init) != 0) return 1;
  } else if (text[0] == 'Q'){
    // if (draw_xpm((xpm_map_t) quit, x_init + 100, y_init) != 0) return 1;
    if (draw_sprite(menu_quit, x_init + 100, y_init) != 0) return 1;
  }

  return 0;
}

int drawMouse(int x, int y) {
  return draw_xpm((xpm_map_t) mouse, x, y);
}

int drawPetting(Game* game) {
  gid_t catColor;

  switch (((CatInfo*) selectedCat->typeInfo)->color) {
    case RED_CAT: // type of cat
      catColor = CAT_RED; // cat specific color
      break;
    
    case BROWN_CAT:
      catColor = CAT_BROWN;
      break;

    case BLUE_CAT:
      catColor = CAT_BLUE;
      break;

    case GRAY_CAT:
      catColor = CAT_GRAY;
      break;

    case ORANGE_CAT:
      catColor = CAT_ORANGE;
      break;
    
    default:
      return 1;
  }

  if (set_background_color(0x105, catColor) != 0) return 1;

  //place holder: will maybe draw a black cat face outline
  if (draw_rectangle(0x105, 100, 100, 200, 200, YELLOW) != 0) return 1;

  return 0;
}
