#include <lcom/lcf.h>
#include "../inc/timer.h"
#include <stdint.h>

#include "../inc/i8254.h"
#include "../inc/controller.h"
#include "../inc/rtc.h"

int hook_id_timer = 0;
int counter_timer = 0;
extern int timed;
extern vbe_mode_info_t vbe;
extern uint8_t *frame_buffer;
extern uint8_t *double_buffer;
extern uint32_t frame_buffer_size;
extern bool unlimited_bombs;
extern int count_unlimited_bombs;

int (timer_subscribe_int)(uint8_t *bit_no) {
  
  if (bit_no == NULL) return 1;

  *bit_no = BIT(hook_id_timer);
  if(sys_irqsetpolicy(0, IRQ_REENABLE, &hook_id_timer) != 0) return 1;

  return 0;
}

int (timer_unsubscribe_int)() {
  
  if(sys_irqrmpolicy(&hook_id_timer) != 0) return 1;

  return 0;
}

void (timer_int_handler)() {
  counter_timer++;
}

int (timer_update)(){
  timer_int_handler();
  if(counter_timer % 60 == 0) {
    timed++;
    if(hero_actions()) return 1;
    if(check_bomb()) return 1;
    if(move_monsters_green()) return 1;
    rtc_ih();
  }

  if(counter_timer % 10 == 0){
    if(move_monsters_red()) return 1;
    if(check_unlimited_bombs()) return 1;
    check_game();
  }
  
  memcpy(frame_buffer,double_buffer,frame_buffer_size);

  return 0;
}
