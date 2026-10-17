#include "controller.h"

extern MouseInfo mouse_info;

extern Sprite *button_quit;
extern Sprite *button_draw;
extern Sprite *button_help;
extern Sprite *arrow_next;

uint32_t drawingColor = 0x000000;
uint8_t drawingColorCode = 0;
uint8_t pointer_radius = 5;

bool quit_pressed = false;
bool draw_pressed = false;
bool help_pressed = false;
bool arrow_pressed = false;

void draw_controls(uint8_t scancode){
 switch(scancode){
   case 0x02:
            drawingColor = 0x000000;
            drawingColorCode = 0;
            break;
   case 0x03:
            drawingColor = 0xAAAAAA;
            drawingColorCode = 1;
            break; 
  case 0x04:
            drawingColor = 0xFF5555;
            drawingColorCode = 2;
            break; 
  case 0x05:
            drawingColor = 0xFFFF55;
            drawingColorCode = 3;
            break; 
  case 0x06:
            drawingColor = 0x55FF55;
            drawingColorCode = 4;
            break; 
  case 0x07:
            drawingColor = 0x55FFFF;
            drawingColorCode = 5;
            break; 
  case 0x08:
            drawingColor = 0x0000AA;
            drawingColorCode = 6;
            break; 
  case 0x09:
            drawingColor = 0xAA00AA;
            drawingColorCode = 7;
            break;
  case 0x0A:
            drawingColor = 0xFFFFFF;
            drawingColorCode = 8;
            break;
  case 0x48:
            if(pointer_radius<35){
              pointer_radius+=3;
            }
            break;
  case 0x50:
            if(pointer_radius>5){
              pointer_radius-=3;
            }
            break;
  case 0xb2:
            invertColor();    
            break;
  case 0xa6:
            gray_components();    
            break;                
  case 0x30:  
            if(game_state == GAME){
              tool = BUCKET;
            }
            break;
  case 0x99:
            if(game_state == GAME){
              tool = PENCIL;
            }
            break;
  case 0x93:
            if(game_state==GAME){
              tool = RECTANGLE;
            }          
            break;
  case 0xa5:
          if(game_state==GAME){
          backup_draw();
          }
  
   default:
            break;
 }
}

int mouse_over_sprite(Sprite *sprite, int x, int y) {
  return mouse_info.x >= x && mouse_info.x <= x + sprite->width &&
         mouse_info.y >= y && mouse_info.y <= y + sprite->height;
}

void pressed_color() {
  if(mouse_info.left_click){
    switch (mouse_over_color())
    {
    case 1:
      drawingColor = 0x000000;
      drawingColorCode = 0;
      break;
    case 2:
      drawingColor = 0xAAAAAA;
      drawingColorCode = 1;
      break;
    case 3:
      drawingColor = 0xFF5555;
      drawingColorCode = 2;
      break;
    case 4:
      drawingColor = 0xFFFF55;
      drawingColorCode = 3;
      break;
    case 5:
      drawingColor = 0x55FF55;
      drawingColorCode = 4;
      break;
    case 6:
      drawingColor = 0x55FFFF;
      drawingColorCode = 5;
      break;
    case 7:
      drawingColor = 0x0000AA;
      drawingColorCode = 6;
      break;
    case 8:
      drawingColor = 0xAA00AA;
      drawingColorCode = 7;
      break;
    case 9:
      drawingColor = 0xFFFFFF;
      drawingColorCode = 8;
      break;
    case 10:
      tool = BUCKET;
    default:
      break;
    }

  }
}

int mouse_over_color() {
  if(mouse_info.y >= 10 && mouse_info.y <= 60) {
    if (mouse_info.x >= 30 && mouse_info.x <= 80) return 1;
    if (mouse_info.x >= 106 && mouse_info.x <= 156) return 2;
    if (mouse_info.x >= 182 && mouse_info.x <= 232) return 3;
    if (mouse_info.x >= 258 && mouse_info.x <= 308) return 4;
    if (mouse_info.x >= 334 && mouse_info.x <= 384) return 5;
    if (mouse_info.x >= 410 && mouse_info.x <= 460) return 6;
    if (mouse_info.x >= 486 && mouse_info.x <= 536) return 7;
    if (mouse_info.x >= 562 && mouse_info.x <= 612) return 8;
    if (mouse_info.x >= 638 && mouse_info.x <= 688) return 9;
    if (mouse_info.x >= 714 && mouse_info.x <= 764) return 10;
  }

  return 0;
}

int mouse_over_clickable() {
  if (game_state == START) {
    return mouse_over_sprite(button_quit, 300, 425) || mouse_over_sprite(button_draw, 300, 175) || mouse_over_sprite(button_help, 300, 300);
  }
  if (game_state == GAME) {
    return (mouse_over_color() != 0);
  }

  if (game_state == INSTRUCTIONS){
    return mouse_over_sprite(arrow_next, 725, 10);
  }

  return 0;
}

int quit_button_pressed() {

  if(mouse_over_sprite(button_quit, 300, 425)){
    if(!quit_pressed && mouse_info.left_click){
      quit_pressed = true;
      return 0;
    }

    if(quit_pressed && !mouse_info.left_click){
      quit_pressed = false;
      return 1;
    }
  }

  quit_pressed = false;
  return 0;
}

int draw_button_pressed() {

  if(mouse_over_sprite(button_draw, 300, 175)){
    if(!draw_pressed && mouse_info.left_click){
      draw_pressed = true;
      return 0;
    }

    if(draw_pressed && !mouse_info.left_click){
      draw_pressed = false;
      return 1;
    }
  }

  draw_pressed = false;
  return 0;
}

int help_button_pressed() {

  if(mouse_over_sprite(button_help, 300, 300)){
    if(!help_pressed && mouse_info.left_click){
      help_pressed = true;
      return 0;
    }

    if(help_pressed && !mouse_info.left_click){
      help_pressed = false;
      return 1;
    }
  }

  help_pressed = false;
  return 0;
}



int arrow_button_pressed(){
  if(mouse_over_sprite(arrow_next, 725, 10)){
    if(!arrow_pressed && mouse_info.left_click){
      arrow_pressed = true;
      return 0;
    }

    if(arrow_pressed && !mouse_info.left_click){
      arrow_pressed = false;
      return 1;
    }
  }

  arrow_pressed = false;
  return 0;
}

void paint_bucket(){
  invert_backup_draw();
  if(mouse_info.left_click){
    if(mouse_info.y > 100 && mouse_info.y < 600){
      cleanThirdBuffer2(drawingColor);
    }
  }

}
