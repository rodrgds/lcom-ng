#include "view.h"
#include "setup.h"
#include "controller/controller.h"
#include <math.h>
#include <stdint.h>
#include <stdlib.h>

extern vbe_mode_info_t mode_info;
extern real_time_info time_info;
uint8_t *main_frame_buffer;
uint8_t *secondary_frame_buffer;
uint8_t *third_frame_buffer;
uint8_t *backup_frame_buffer;
uint32_t frame_buffer_size;
extern MouseInfo mouse_info;
extern Sprite *mouse;
extern Sprite *button_quit;
extern Sprite *button_draw;
extern Sprite *pencil_draw;
extern Sprite *number_zero;
extern Sprite *number_one;
extern Sprite *number_two;
extern Sprite *number_three;
extern Sprite *number_four;
extern Sprite *number_five;
extern Sprite *number_six;
extern Sprite *number_seven;
extern Sprite *number_eight;
extern Sprite *number_nine;
extern Sprite *colon;
extern Sprite *eraser;
extern Sprite *hand;
extern Sprite *bucket;
extern Sprite *instructions;
extern Sprite *eraser_cursor;
extern Sprite *pencil_cursor;
extern Sprite *bucket_cursor;
extern Sprite *outline_tool;
extern Sprite *button_help;
extern Sprite *vsdraw;
extern Sprite *letter_b;
extern Sprite *inst1;
extern Sprite *inst2;
extern Sprite *arrow_next;
extern Sprite *arrow_previous;
extern MenuState game_state;
extern ToolState tool;

int inst_page = 1;

Sprite *number_sprite[10]; // Array to store number sprites

void initialize_number_sprites() {
  number_sprite[0] = number_zero;
  number_sprite[1] = number_one;
  number_sprite[2] = number_two;
  number_sprite[3] = number_three;
  number_sprite[4] = number_four;
  number_sprite[5] = number_five;
  number_sprite[6] = number_six;
  number_sprite[7] = number_seven;
  number_sprite[8] = number_eight;
  number_sprite[9] = number_nine;
}

uint32_t drawingColor;
uint8_t drawingColorCode;
uint8_t pointer_radius;

int set_frame_buffers(uint16_t mode) {
  if (set_frame_buffer(mode, &main_frame_buffer))
    return 1;
  frame_buffer_size = mode_info.XResolution * mode_info.YResolution * ((mode_info.BitsPerPixel + 7) / 8);

  if (DOUBLE_BUFFER) {
    secondary_frame_buffer = (uint8_t *) malloc(frame_buffer_size);
    third_frame_buffer = (uint8_t *) malloc(frame_buffer_size);
    backup_frame_buffer = (uint8_t*) malloc(frame_buffer_size);
  }

  return 0;
}
void swap_buffers() {
  memcpy(main_frame_buffer, secondary_frame_buffer, frame_buffer_size);
}
void copyTheDraw() {
  memcpy(secondary_frame_buffer, third_frame_buffer, frame_buffer_size);
}

void cleanSecondaryBuffer() {
  memset(secondary_frame_buffer, 0xFFFFFF, frame_buffer_size);
}
void cleanThirdBuffer() {
  memset(third_frame_buffer, 0xFFFFFF, frame_buffer_size);
}
void cleanBackupBuffer(){
  memset(backup_frame_buffer, 0xFFFFFF, frame_buffer_size);
}

void cleanThirdBuffer2(uint32_t drawingColor){
  
  
  for (int y = 0; y < mode_info.YResolution; y++) {
    for (int x = 0; x < mode_info.XResolution; x++) {
      paint_pixel(x, y, drawingColor, third_frame_buffer);
    }
  }
}

int draw_sprite_xpm(Sprite *sprite, int x, int y) {
  uint32_t current_color;

  if (sprite->colors == NULL)
    return 1;


  for (int h = 0; h < sprite->height; h++) {
    for (int w = 0; w < sprite->width; w++) {
      current_color = sprite->colors[w + h * sprite->width];
      if (paint_pixel(x + w, y + h, current_color, secondary_frame_buffer) != 0)
        return 1;
    }
  }
  return 0;
}

void draw_mouse() {
  if (mouse_over_clickable()) draw_sprite_xpm(hand, mouse_info.x, mouse_info.y);
  else if(game_state == GAME && mouse_info.y > 100){
    if(tool == PENCIL){ 
      if(drawingColorCode == 8) draw_sprite_xpm(eraser_cursor, mouse_info.x, mouse_info.y);
      else draw_sprite_xpm(pencil_cursor, mouse_info.x, mouse_info.y);
    }
    else if(tool == BUCKET) draw_sprite_xpm(bucket_cursor, mouse_info.x, mouse_info.y);
    else draw_sprite_xpm(mouse, mouse_info.x, mouse_info.y);
  }
  else draw_sprite_xpm(mouse, mouse_info.x, mouse_info.y);
}

void draw_game_menu() {
  memset(secondary_frame_buffer, 0x4f4f4f, frame_buffer_size);
  draw_sprite_xpm(vsdraw, 200, 0);
  draw_sprite_xpm(button_draw, 300, 175);
  draw_sprite_xpm(button_help, 300, 300);
  draw_sprite_xpm(button_quit, 300, 425);
}
void draw_frame() {

  switch (game_state) {
    case START:
      draw_game_menu();
      break;
    case GAME:
      copyTheDraw();
      draw_header();
      draw_tool_outline();

      if (draw_game() != 0) {
        vg_exit();
      }
      break;
    case INSTRUCTIONS:
      draw_instructions();
      break;
    default:
      break;
  }
  draw_mouse();
  updateClock();
  swap_buffers();
}

int draw_game(){
    draw_sprite_xpm(number_one, 50, 70);
    draw_sprite_xpm(number_two, 126, 70);
    draw_sprite_xpm(number_three, 202, 70);
    draw_sprite_xpm(number_four, 278, 70);
    draw_sprite_xpm(number_five, 354, 70);
    draw_sprite_xpm(number_six, 430, 70);
    draw_sprite_xpm(number_seven, 506, 70);
    draw_sprite_xpm(number_eight, 582, 70);
    draw_sprite_xpm(number_nine, 658, 70);
    draw_sprite_xpm(letter_b, 734, 70);
    return 0;

}

void draw_header(){
  uint32_t colors[9] = {0x000000, 0xAAAAAA, 0xFF5555, 0xFFFF55, 0x55FF55, 0x55FFFF, 0x0000AA, 0xAA00AA,0xFFFFFF};
  uint8_t bytes_per_pixel = (mode_info.BitsPerPixel + 7) / 8;
  memset(secondary_frame_buffer,0x4f4f4f,mode_info.XResolution*100*bytes_per_pixel);
  int index =0;
  int x = 30;
  for( int i = 0; i<8; i++){
    drawColor(x, 10, 50, 50, colors[index++]);
    x+=76;
  }
  draw_sprite_xpm(eraser, x, 10);
  draw_sprite_xpm(bucket, x+76, 10);

}

void draw_instructions(){
  if (inst_page == 1){
    draw_sprite_xpm(inst1, 0, 0);
    draw_sprite_xpm(arrow_next, 725, 10);
  }
  if(inst_page == 2){
    draw_sprite_xpm(inst2, 0, 0);
    draw_sprite_xpm(arrow_previous, 725, 10);
  }
  
}



void drawColor(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {
  uint8_t bytes_per_pixel = (mode_info.BitsPerPixel + 7) / 8;
  for (int h = 0; h < height; h++) {
    for (int w = 0; w < width; w++) {
      if ((x + w) >= mode_info.XResolution || (y + h) >= mode_info.YResolution)
        return;
      if (color == 0xfffffe)
        return;
      memcpy(&secondary_frame_buffer[(mode_info.XResolution * (y + h) + (x + w)) * bytes_per_pixel], &color, bytes_per_pixel);
    }
  }
}


int draw_line_pointer(uint16_t x, uint16_t y, uint16_t radius, uint32_t color) {
  for (uint16_t i = x - radius; i < x + radius; i++)
    for (uint16_t j = y - radius; j < y + radius; j++) {
      if ((x - i) * (x - i) + (y - j) * (y - j) <= radius * radius)
        paint_pixel(i, j, color, third_frame_buffer);
    }
  return 0;
}

int drawLineSegment(uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf, uint32_t color) {
  float slope, intercept;
  if (xi == xf && yi == yf)
    return draw_line_pointer(xi, yi, pointer_radius, color);

  if (xi != xf) {
    slope = (float)(yf - yi) / (float)(xf - xi);
  }
  else {
    slope = 0;
  }

  if (xi != xf && fabs(slope) <= 1.0) {
    if (xi > xf) {
      uint16_t tempX = xf;
      uint16_t tempY = yf;
      xf = xi;
      xi = tempX;
      yf = yi;
      yi = tempY;
    }

    intercept = yi - slope * xi;

    for (uint16_t i = xi; i < xf; i++) {
      float j = slope * i + intercept;
      if (draw_line_pointer(i, roundf(j), pointer_radius, color)) {
        return 1;
      }
    }
  }
  else {
    if (yi > yf) {
      uint16_t tempX = xf;
      uint16_t tempY = yf;
      xf = xi;
      xi = tempX;
      yf = yi;
      yi = tempY;
    }

    slope = (float)(xf - xi) / (float)(yf - yi);
    intercept = xi - slope * yi;

    for (uint16_t j = yi; j < yf; j++) {
      float i = slope * j + intercept;
      if (draw_line_pointer(roundf(i), j, pointer_radius, color)) {
        return 1;
      }
    }
  }

  return 0;
}

int processDrawing(int x, int y, int prevX, int prevY, uint32_t drawingColor) {
  
  if (y > 100 && y < 600) {
    if (drawLineSegment(x, y, prevX, prevY, drawingColor))
      return 1;
  }
  else {
    return 1;
  }
  return 0;
}
int processRectangle(int x, int y,int prevX, int prevY, uint32_t drawingColor){
  if( y>100 && y <600){
    if (prevX >= x && prevY >= y){
    if(draw_rectangle(x, y, prevX-x, prevY-y, drawingColor,third_frame_buffer)) return 1;
    }
    if (prevX >= x && prevY <= y){
    if(draw_rectangle(x, prevY, prevX-x, y-prevY, drawingColor,third_frame_buffer)) return 1;
    }
    if (prevX <= x && prevY >= y){
    if(draw_rectangle(prevX, y, x-prevX, prevY-y, drawingColor,third_frame_buffer)) return 1;
    }
    if (prevX <= x && prevY <= y){
    if(draw_rectangle(prevX, prevY, x-prevX, y-prevY, drawingColor,third_frame_buffer)) return 1;
    }
    return 0;
  }
  else{
    return 1;
  }
  return 0;
}
void updateClock(){

  initialize_number_sprites();

  int x = 652;
  int y = 575;

  int hours = time_info.hours;
  int tens_hour = hours / 10;
  int ones_hour = hours % 10;

  draw_sprite_xpm(number_sprite[tens_hour], x, y);
  x += 15;

  draw_sprite_xpm(number_sprite[ones_hour], x, y);
  x += 15;

  draw_sprite_xpm(colon, x, y);

  x += 20;

  int minutes = time_info.minutes;
  int tens_minute = minutes / 10; 
  int ones_minute = minutes % 10; 

  draw_sprite_xpm(number_sprite[tens_minute], x, y);
  x += 15;

  draw_sprite_xpm(number_sprite[ones_minute], x, y);
  x += 15;

  draw_sprite_xpm(colon, x, y);

  x += 20;
  int seconds = time_info.seconds;
  int tens_second = seconds / 10; 
  int ones_second = seconds % 10; 


  draw_sprite_xpm(number_sprite[tens_second], x, y);
  x += 15;

  draw_sprite_xpm(number_sprite[ones_second], x, y);
}






void invertColor(){
   invert_backup_draw();
   for (int y = 0; y < mode_info.YResolution; y++) {
    for (int x = 0; x < mode_info.XResolution; x++) {
      invert_pixel_color(x, y,third_frame_buffer);
    }
  }
}

void gray_components(){
   invert_backup_draw();
   for (int y = 0; y < mode_info.YResolution; y++) {
    for (int x = 0; x < mode_info.XResolution; x++) {
      convert_to_gray(x, y, third_frame_buffer);
    }
  }
}
void backup_draw(){
  memcpy(third_frame_buffer,backup_frame_buffer,frame_buffer_size);
}
void invert_backup_draw(){
  memcpy(backup_frame_buffer,third_frame_buffer,frame_buffer_size);
}

void draw_tool_outline(){
  if(tool == PENCIL){
    draw_sprite_xpm(outline_tool, 27+(76*drawingColorCode), 7);
  }
  if(tool == BUCKET){
    draw_sprite_xpm(outline_tool, 711, 7);
  }
}
