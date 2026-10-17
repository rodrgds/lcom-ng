#include "mouse.h"

int mouse_hook_id = 2;
uint8_t mouse_byte;
uint8_t which_byte = 0;
uint8_t mouse_bytes[3];
struct package mouse_package;
int mouse_x = 69;
int mouse_y = 69;
click_info info_mouse={0, 0, 0, 0, 0, 0};

int(mouse_subscribe)(){
  return sys_irqsetpolicy(IRQ_MOUSE, IRQ_REENABLE | IRQ_EXCLUSIVE, &mouse_hook_id);
}

int(mouse_unsubscribe)(){
  return sys_irqrmpolicy(&mouse_hook_id);
}

int(mouse_ihandler)(){
  if(read_KBC_out(KBC_OUT_CMD, &mouse_byte, true) != 0){
    printf("ERROR Couldn't read the mouse byte! \n");
    return 1;
  }
  return 0;
}

int (mouse_config)(uint8_t control_word){

  uint32_t mouse_response;

  do{
    if(write_KBC(KBC_IN_CMD, MOUSE_TESTER)) return 1;
    if(write_KBC(KBC_OUT_CMD, control_word)) return 1;
    tickdelay(micros_to_ticks(20000));
    if(sys_inb(KBC_OUT_CMD, &mouse_response)) return 1;

  } while (mouse_response != MOUSE_RESPONSE_OK);

  return 0;
}

void (mouse_sync_bytes)() {
  if (which_byte == 0 && (mouse_byte & FIRST_BYTE)) {
    mouse_bytes[which_byte]= mouse_byte;
    which_byte++;
  }
  else if (which_byte > 0) {
    mouse_bytes[which_byte] = mouse_byte;
    which_byte++;
  }
}

int (mouse_bytes_to_package)(){
  for (int i = 0 ; i < 3 ; i++) {
    mouse_package.bytes[i] = mouse_bytes[i];
  }

  mouse_package.lb = mouse_bytes[0] & MOUSE_LEFT_BUTTON;
  info_mouse.last_left=info_mouse.left;
  info_mouse.left=mouse_bytes[0] & MOUSE_LEFT_BUTTON;

  mouse_package.mb = mouse_bytes[0] & MOUSE_MIDDLE_BUTTON;
  info_mouse.last_middle=info_mouse.middle;
  info_mouse.middle=mouse_bytes[0] & MOUSE_MIDDLE_BUTTON;

  mouse_package.rb = mouse_bytes[0] & MOUSE_RIGHT_BUTTON;
  info_mouse.last_right=info_mouse.right;
  info_mouse.right=mouse_bytes[0] & MOUSE_RIGHT_BUTTON;
  
  mouse_package.x_ov = mouse_bytes[0] & MOUSE_X_SIGNAL;
  mouse_package.y_ov = mouse_bytes[0] & MOUSE_Y_SIGNAL;
  mouse_package.delta_x = (mouse_bytes[0] & MOUSE_X_DELTA) ? (0xff00 | mouse_bytes[1]) : mouse_bytes[1];
  mouse_package.delta_y = (mouse_bytes[0] & MOUSE_Y_DELTA) ? (0xff00 | mouse_bytes[2]) : mouse_bytes[2];

  return 0;
}

bool (left_button_released)(){
  if (info_mouse.last_left && !info_mouse.left){
    info_mouse.last_left = 0;
    return 1;
  }
  return 0;
}

void (update_mouse_location)(){
  if(mouse_x + mouse_package.delta_x < 0 && !mouse_package.x_ov){
    mouse_x = 0;
  }
  else if(mouse_x + mouse_package.delta_x > 800-51 && !mouse_package.x_ov){
    mouse_x = 800-51;
  }
  else if(!mouse_package.x_ov){
    mouse_x += mouse_package.delta_x;
  }

  if(mouse_y - mouse_package.delta_y < 0 && !mouse_package.y_ov){
    mouse_y = 0;
  }
  else if(mouse_y - mouse_package.delta_y > 600-51 && !mouse_package.y_ov){
    mouse_y = 600-51;
  }
  else if(!mouse_package.y_ov){
    mouse_y -= mouse_package.delta_y;
  }
}


