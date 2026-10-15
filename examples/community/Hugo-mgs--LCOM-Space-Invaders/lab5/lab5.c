// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <lcom/lab5.h>

#include <stdint.h>
#include <stdio.h>

#include "video.h"
#include "kbd.h"

extern vbe_mode_info_t info;
extern uint8_t scancode;

// Any header files included below this line should have been created by you

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab5/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab5/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int (ESC_key)(){
  int ipc_status;
  message msg;

  uint8_t bit_no;
  if(kbd_subscribe_int(&bit_no) != 0) return 1;

  while(scancode != BREAK_ESC) { /* You may want to use a different condition */
   /* Get a request message. */
    if ( (driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
        printf("driver_receive failed with: %d");
        continue;
    }
    if (is_ipc_notify(ipc_status)) { /* received notification */
        switch (_ENDPOINT_P(msg.m_source)) {
            case HARDWARE: 	
              if (msg.m_notify.interrupts & bit_no) { 
                  kbc_ih();
              }
              break;
            default:
              break; /* no other notifications expected: do nothing */
        }
    } else { 
    }
  }
  
  printf("Unsubscribing\n");
  if (kbd_unsubscribe_int() != 0) return 1;
  return 0;

}

int(video_test_init)(uint16_t mode, uint8_t delay) {
  if (video_set_mode(mode) != 0) return 1;

  sleep(delay);

  if (vg_exit() != 0) return 1;
  return 0;
}

int(video_test_rectangle)(uint16_t mode, uint16_t x, uint16_t y,
                          uint16_t width, uint16_t height, uint32_t color) {

  if (video_map_memory(mode) != 0) return 1;
  if (video_set_mode(mode) != 0) return 1;

  if (vg_draw_rectangle(x, y, width, height, color) != 0) return 1;

  if (ESC_key() != 0) return 1;

  if (vg_exit() != 0) return 1;
  return 0;
}

int(video_test_pattern)(uint16_t mode, uint8_t no_rectangles, uint32_t first, uint8_t step) {

  if (video_map_memory(mode) != 0) return 1;
  if (video_set_mode(mode) != 0) return 1;

  int x = info.XResolution / no_rectangles;
  int y = info.YResolution / no_rectangles;

  for (int i = 0; i < no_rectangles; i++) {
    for (int j = 0; j < no_rectangles; j++) {
      if (info.MemoryModel == 0x06) {
        if (vg_draw_rectangle(i * x, j * y, x, y, direct_color(j, i, no_rectangles, first, step)) != 0) return 1;
      } else {
        if (vg_draw_rectangle(i * x, j * y, x, y, indexed_color(j, i, no_rectangles, first, step)) != 0) return 1;
      } 
    }
  }
 
  if (ESC_key() != 0) return 1;

  if (vg_exit() != 0) return 1;
  return 0;
}

int(video_test_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y) {

  if (video_map_memory(0x105) != 0) return 1;
  if (video_set_mode(0x105) != 0) return 1;

  if (vg_draw_xpm(xpm, x, y) != 0) return 1;

  if (ESC_key() != 0) return 1;

  if (vg_exit() != 0) return 1;

  return 0;
}

int(video_test_move)(xpm_map_t xpm, uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf,
                     int16_t speed, uint8_t fr_rate) {
  int ipc_status;
  message msg;
  uint8_t bit_no_timer, bit_no_kbc;
  // 1 horizontal, 2 vertical
  uint8_t direction = 0;
  if (!((xi == xf && yi < yf) || (xi < xf && yi == yf))) return 1;
  if (xi < xf && yi == yf) direction = 1;
  if (xi == xf && yi < yf) direction = 2;

  if (kbd_subscribe_int(&bit_no_kbc) != 0) return 1;
  if (timer_subscribe_int(&bit_no_timer) != 0) return 1;

  if (timer_set_frequency(0, fr_rate) != 0) return 1;

  if (video_map_memory(0x105) != 0) return 1;
  if (video_set_mode(0x105) != 0) return 1;

  xpm_image_t img;
  xpm_load(xpm, XPM_INDEXED, &img);

  uint16_t width = img.width;
  uint16_t height = img.height;

  if (vg_draw_xpm(xpm, xi, yi) != 0) return 1;

  while(scancode != BREAK_ESC && (xi < xf || yi < yf)) { 
     if ( (driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
         printf("driver_receive failed with: %d");
         continue;
     }
     if (is_ipc_notify(ipc_status)) { 
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE: 	
          if (msg.m_notify.interrupts & bit_no_kbc) { 
            kbc_ih();
          }

          if (msg.m_notify.interrupts & bit_no_timer) {
            if(vg_draw_rectangle(xi, yi, width, height, 0xFFFFFF) != 0) return 1;

            if (direction == 1){
              xi += speed;
              if (xi > xf) xi = xf;
            }
            else if (direction == 2){
              yi += speed;
              if (yi > yf) yi = yf;
            }
            if(vg_draw_xpm(xpm, xi, yi) != 0) return 1;
          }
          break;
        default:
          break; 
      }
     } 
   }

  if (vg_exit() != 0) return 1;

  if (timer_unsubscribe_int() != 0) return 1;
  if (kbd_unsubscribe_int() != 0) return 1;

  return 0;
}

int(video_test_controller)() {
  /* This year you do not need to implement this */
  printf("%s(): under construction\n", __func__);

  return 1;
}
