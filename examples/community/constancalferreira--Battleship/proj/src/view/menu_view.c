#include "menu_view.h"

extern uint8_t *current_buffer;
extern uint8_t *menu_buffer;
extern unsigned int frame_size;
extern vbe_mode_info_t mode_info;

extern Sprite *menu_shipR;
extern Sprite *menu_shipL;
extern Sprite *logo;
extern Sprite *sun; 
extern Sprite *moon;
extern Sprite *press_enter;

#define DAY_START_HOUR 6
#define DAY_START_MINUTE 0
#define DAY_END_HOUR 20
#define DAY_END_MINUTE 30
#define LIGHT_BLUE 0x8DBCC7 
#define DARK_BLUE  0x213448 

void draw_menu_background(uint8_t *buffer) {
    if (read_rtc_time() != 0) {
        return;
    }

    uint32_t background;
    Sprite *sky;
    Sprite *other;
    if (daytime(rtc_info.hours, rtc_info.minutes)) {
        background = LIGHT_BLUE; 
        sky = sun; 
        other = cloud; 
    } else {
        background = DARK_BLUE;
        sky = moon;
        other = star;
    }

    fill_screen(background, buffer);
    draw_sprite(logo, 5, 20, buffer);
    draw_sprite(sky, 350, 240, buffer);
    draw_sprite(other, 30, 270, buffer);
    draw_sprite(other, 200, 340, buffer);
    draw_sprite(other, 500, 255, buffer);
    draw_sprite(other, 650, 350, buffer);
    draw_rectangle(0, 500, 800, 100, TEAL, buffer);
    draw_sprite(press_enter, 250, 530, buffer);

}

bool daytime(uint8_t hour, uint8_t minute) {
  if (hour < DAY_START_HOUR) return false;
  if (hour > DAY_END_HOUR) return false;
  if (hour == DAY_START_HOUR && minute < DAY_START_MINUTE) return false;
  if (hour == DAY_END_HOUR && minute >= DAY_END_MINUTE) return false;
  return true;
}


void draw_menu() {
  memcpy(current_buffer, menu_buffer, frame_size);

  static int x = 0;
  static int dx = 2;
  int y = 500 - menu_shipR->height / 2;

  x += dx;
  if (x < 0) {
    x = 0;
    dx = -dx;
  }
  if (x > (int)(mode_info.XResolution - menu_shipR->width)) {
    x = mode_info.XResolution - menu_shipR->width;
    dx = -dx;
  }

  Sprite *sprite_to_draw = (dx > 0) ? menu_shipR : menu_shipL;
  draw_sprite(sprite_to_draw, x, y, current_buffer);
}




