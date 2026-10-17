#include "assist.h"
#include "sprite.h"

extern int hook_id_timer;
extern int hook_id_keyboard;
extern int hook_id_mouse;
extern int mouseX;
extern int mouseY;
extern int lastMouseX;
extern int lastMouseY;
extern struct packet mouse_packet;
extern uint16_t h_res;
extern uint16_t v_res;
extern uint16_t bitsPerPixel;
extern uint16_t bytesPerPixel;
uint16_t mode = 0x0000;
extern uint32_t mouse_array[12][12];


int (subscribe_all)(uint8_t aux_timer, uint8_t aux_keyboard, uint8_t aux_mouse){
  if (mouse_en_data_report()) return 1;
  if (timer_set_frequency(0, 60) != OK) return 1;
  if (kbd_subscribe_int(&aux_keyboard) != OK ) return 1;
  if ( timer_subscribe_int(&aux_timer) != OK ) return 1;
  if ( mouse_subscribe_int(&aux_mouse) != OK ) return 1;

  hook_id_keyboard = (int)aux_keyboard;
  hook_id_mouse = (int)aux_mouse;
  hook_id_timer = (int)aux_timer;
  
  printf("Mode: %x\n", mode);
  vg_init(mode);
  return 0;
}


int (unsubscribe_all)(){
  if (  kbd_unsubscribe_int() != OK ) return 1;
  if (timer_unsubscribe_int() != OK ) return 1;
  if (mouse_unsubscribe_int() != OK ) return 1;
  if (mouse_dis_data_report()) return 1;
  vg_exit();
  return 0;
}

void (drawMouse)(){
    vg_ultimate_pixmap_handler(abs(mouseX), abs(mouseY), mode, CROSSHAIR);
}

void (eraseMouse)(bool mouse){
    vg_ultimate_pixmap_eraser(abs(lastMouseX), abs(lastMouseY), mode, CROSSHAIR, mouse);
}

int (isInOption)(int x, int y, int width, int height){
    return abs(mouseX+6) >= x && abs(mouseX+6) <= x + width && abs(mouseY-6) >= y && abs(mouseY-6) <= y + height;
}

bool (checkClickEnemy)(int enemyX, int enemyY){
    return isInOption(enemyX, enemyY, 20, 20) && mouse_packet.lb;
}

bool (checkMouseEnemy)(int enemyX, int enemyY){
    return isInOption(enemyX, enemyY, 20, 20);
}

void (updateGameMouse)(){
    lastMouseX = mouseX;
    lastMouseY = mouseY;
    int y_res = (int) v_res - 80, x_res = (int) h_res;
    int increX = (int) mouse_packet.delta_x;
    int increY = (int) mouse_packet.delta_y;
    if (mouse_packet.x_ov || mouse_packet.y_ov) {/*printf("mouse overflow\n");*/} 
    else {
        if ((mouse_packet.delta_x & 0xFF00) == 0xFF) increX = 0 - ((int) mouse_packet.delta_x);
        if ((mouse_packet.delta_y & 0xFF00) == 0xFF) increY = 0 - ((int) mouse_packet.delta_x);
        mouseX += increX;
        mouseY += increY;
        if (mouseX >= x_res-11) mouseX = x_res - 11;
        if (mouseY <= -(y_res-12)) mouseY = -(y_res-12);
        if (mouseX <= 0) mouseX = 0;
        if (mouseY >= 0) mouseY = 0;
    }
}


void (updateMouse)(){
    lastMouseX = mouseX;
    lastMouseY = mouseY;
    int increX = (int) mouse_packet.delta_x;
    int increY = (int) mouse_packet.delta_y;
    if (mouse_packet.x_ov || mouse_packet.y_ov) {/*printf("mouse overflow\n");*/} 
    else {
        if ((mouse_packet.delta_x & 0xFF00) == 0xFF) increX = 0 - ((int) mouse_packet.delta_x);
        if ((mouse_packet.delta_y & 0xFF00) == 0xFF) increY = 0 - ((int) mouse_packet.delta_x);
        mouseX += increX;
        mouseY += increY;
        if (mouseX >= (int) h_res-11) mouseX = (int) h_res - 11;
        if (mouseY <= - ((int) v_res-12)) mouseY = -((int) v_res - 12);
        if (mouseX <= 0) mouseX = 0;
        if (mouseY >= 0) mouseY = 0;
    }
}

