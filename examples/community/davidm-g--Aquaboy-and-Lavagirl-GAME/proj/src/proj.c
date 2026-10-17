#include "defines.h"
#include "devices/graphics/video.h"
#include "devices/keyboard/i8042.h"
#include "devices/keyboard/keyboard.h"
#include "devices/mouse/mouse.h"
#include "devices/rtc/rtc.h"
#include "devices/timer/i8254.h"
#include "devices/timer/timer.h"
#include "devices/utils.h"
#include "draws/draw.h"
#include "visuals/model.h"
#include <lcom/lcf.h>
extern rtc_values rtc;
extern int global_counter;
extern uint8_t kbd_outbuf;
extern uint32_t *background_map;
extern xpm_image_t background_img;
extern uint32_t *background_map_menu;
extern xpm_image_t background_img_menu;
extern uint32_t *background_map_leaderboard;
extern xpm_image_t background_img_leaderboard;
extern SystemState systemState;
extern SpriteState boyState;
extern Sprite *boys[6];
extern SpriteState girlState;
extern Sprite *girls[6];
extern Sprite *cursor;
extern Sprite *start;
extern bool change;
extern Sprite *opendoor;
extern Sprite *walls20[1200];
extern int *levelArray;
extern bool completed;
extern int level;
extern LeaderboardEntry leaderboard[LEADERBOARD_SIZE];

extern bool is_binary;
int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/proj/src/trace.txt");

  // enables to save the output of printf function calls
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/proj/src/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(proj_main_loop)(int argc, char **argv) {
  map_phys_virt(VIDEO_MODE);
  if (set_graphic_mode(VIDEO_MODE) != 0)
    return 1;

  levelArray = malloc(40 * 30 * sizeof(int));
  updateArrayWithLevel(1);
  load_sprites();
  read_leaderboard_data();
  draw_frame();
  timer_set_frequency(0, FRAME_RATE);
  uint8_t kbd_bit_no = 0x01, timer_bit_no = 0x00, mouse_bit_no = 0x02, rtc_bit_no = 0x03;
  int ipc_status, r;
  message msg;
  flip_screen();

  if (timer_subscribe_int(&timer_bit_no) != 0)
    return 1;
  if ((keyboard_subscribe_int(&kbd_bit_no)) != 0)
    return 1;
  if (mouse_subscribe_int(&mouse_bit_no) != 0)
    return 1;
  if (rtc_subscribe_int(&rtc_bit_no) != 0)
    return 1;
  start_rtc();
  send_cmd_mouse(SET_STREAM_MODE);
  send_cmd_mouse(ENABLE_DATA);
  change_sample_rate(20);
  while (systemState == RUNNING) {
    /* Get a request message. */
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          if (msg.m_notify.interrupts & mouse_bit_no) {
            update_mouse();
          }
          if (msg.m_notify.interrupts & kbd_bit_no) {
            update_keyboard();
          }
          if (msg.m_notify.interrupts & timer_bit_no) {
            update_timer();
          }
          if (msg.m_notify.interrupts & rtc_bit_no) {
            update_rtc();
          }
          break;
        default:
          break;
      }
    }
    else { /* received a standard message, not a notification */
      /* no standard messages expected: do nothing */
    }
  }
  write_leaderboard_data();
  if (timer_unsubscribe_int() != 0)
    return 1;
  if (keyboard_unsubscribe_int())
    return 1;
  if (mouse_unsubscribe_int())
    return 1;
  if (rtc_unsubscribe_int())
    return 1;
  send_cmd_mouse(DISABLE_DATA);
  destroy_sprites();
  if (vg_exit() != 0)
    return 1;

  return 0;
}
