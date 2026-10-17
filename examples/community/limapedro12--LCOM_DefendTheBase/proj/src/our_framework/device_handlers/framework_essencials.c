#include "../framework_essencials.h"

bool game_running = true;
u8_t code = 0;
u8_t mouse_arr[3] = {0, 0, 0};
struct packet pp;
int current = 0;

position mouse_position = {0, 0};

unsigned long long time_counter = 0;

bool graphics_on = false;

uint minutes = 33;
uint hours = 33;
bool update_rtc = true;

int run(void (*func)()){
  is_rtc_in_binary();

  if(mouse_enable_data_reporting_ours())
    game_running = false;
  if(timer_subscribe_interrupt())
    game_running = false;
  if(keyboard_subscribe_interrupt())
    game_running = false;
  if(mouse_subscribe_interrupt())
    game_running = false;
  
  int ipc_status;
  message msg;
  int r = 0;
  while(game_running){
    r = driver_receive(ANY, &msg, &ipc_status);
    if(r != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }

    if ((is_ipc_notify(ipc_status) && 
     (_ENDPOINT_P(msg.m_source) == HARDWARE))){
      if (msg.m_notify.interrupts & IRQ_SET_KBD){
        read_scancode(&code);
        // printf("Code: 0x%x\n", code);
      }
      if (msg.m_notify.interrupts & IRQ_SET_MOUSE){
        read_scancode(&(mouse_arr[current]));
        if(current == 2){
          parse_to_packet(mouse_arr, &pp);
          mouse_position.x += pp.delta_x;
          mouse_position.y -= pp.delta_y;
          // printf("Mouse position: (%d, %d)\n", mouse_position.x, mouse_position.y);

          
        }
        current = (current + 1) % 3;
      }
    }
    // printf("Time counter: %d\n", time_counter);

    if(is_timer_0_interrupt(ipc_status, msg)){
      

      func();
      

      doublebuffer_to_vram();
      time_counter++;
      if(time_counter % 60 == 0)
        update_rtc = true;
    }

    if(update_rtc){
      if(!is_rtc_updating()){
        rtc_update_time(&hours, &minutes);
        update_rtc = false;
      }
    }
  }

  mouse_unsubscribe_interrupt();
  keyboard_unsubscribe_interrupt();
  timer_unsubscribe_interrupt();
  mouse_disable_data_reporting();

  if(graphics_on)
    gpu_exit();

  uint reg_b;
  rtc_input(11, &reg_b);
  printf("Reg B: 0x%x\n", reg_b);
  printf("Time: %d:%d\n", hours, minutes);

  return 0;
}

void quit(){
  game_running = false;
  return;
}

int get_time_counter(){
    return time_counter % 600000000;
}

bool is_time_interval_elapsed_seconds(int start_time, int interval){
  if(interval > 10000000){
    printf("Interval too big, max interval is 10000000 seconds\n");
    return false;
  }
  if(600000000-start_time < interval)
    start_time = -start_time;
  return ((int)(time_counter % 600000000) >= start_time + interval * 60);
}

bool is_time_interval_elapsed_milliseconds(int start_time, int interval){
  interval = (interval * 60)/1000;
  if(600000000-start_time < interval)
    start_time = -start_time;
  return ((int)(time_counter%600000000) >= start_time + interval);
}

bool is_key_pressed_code(uint8_t scancode){
  return code == scancode;
}

bool is_key_pressed(char key, bool isBreak){
  if(isBreak){
    if(code == (char_to_scancode(key) | BIT(7))){
      // printf("Got break code: 0x%x", code);
      code = 0;
      return true;
    }
    return false;
  }

  return code == char_to_scancode(key);
}

position get_mouse_position(){
  return mouse_position;
}

void turn_on_graphics(){
  if(!graphics_on){
    graphics_on = true;
    vg_init(0x115);
  }
}

void draw_rectangle(int x, int y, int width, int height, uint32_t color){
  if(!graphics_on){
    printf("Graphics not on!\n");
    return;
  }
  draw_rect(x, y, width, height, color, 0x115);
}

void clear_screen(){
  if(!graphics_on){
    printf("Graphics not on!\n");
    return;
  }
  draw_rect(0, 0, get_width(0x115), get_height(0x115), 0, 0x115);
}

bool set_mouse_position(int x, int y){
  if(!graphics_on){
    printf("Graphics not on!\n");
    return false;
  }
  if(x < 0 || x > get_width(0x115) || y < 0 || y > get_height(0x115)){
    printf("Invalid position!\n");
    return false;
  }
  mouse_position.x = x;
  mouse_position.y = y;
  return true;
}

bool verify_mouse_limits(int x_min, int x_max, int y_min, int y_max){
  if(!graphics_on){
    printf("Graphics not on!\n");
    return false;
  }
  if(mouse_position.x < x_min)
    mouse_position.x = x_min;
  else if(mouse_position.x > x_max)
    mouse_position.x = x_max;
  if(mouse_position.y < y_min)
    mouse_position.y = y_min;
  else if(mouse_position.y > y_max)
    mouse_position.y = y_max;
  return true;

}

bool is_mb_pressed() {
  if(pp.mb) {
    return true;
  }
  return false;
}

bool is_lb_pressed() {
  if(pp.lb) {
    return true;
  }
  return false;
}

bool is_rb_pressed() {
  if(pp.rb) {
    return true;
  }
  return false;
}

uint get_hours(){
  return hours;
}

uint get_minutes(){
  return minutes;
}
