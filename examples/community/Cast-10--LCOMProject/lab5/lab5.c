// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>
#include <lcom/lab5.h>

#include <stdint.h>
#include <stdio.h>
// Any header files included below this line should have been created by you
#include "video.h"
#include "keyboard.h"

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

int(video_test_init)(uint16_t mode, uint8_t delay) {
    
    
    if (vbe_set_mode(mode) != 0) return 1;


    sleep(delay);

    if (vg_exit() != 0) return 1;

    return 0;


}

int(video_test_rectangle)(uint16_t mode, uint16_t x, uint16_t y,
                          uint16_t width, uint16_t height, uint32_t color) {

    
    if (set_vram_mapping(mode) != 0) {
    return 1;
  }

  if (vbe_set_mode(mode) != 0) {
    return 1;
  }

  if (vg_draw_rectangle(x, y, width, height, color) != 0) {
    vg_exit();
    return 1;
  }

  if (video_swap_buffers() != 0) return 1;

  if (wait_for_esc() != 0) {
    vg_exit();
    return 1;
  }

  if (vg_exit() != 0) {
    return 1;
  }

  return 0;


}

// AFTER MAKING DOUBLE BUFFERING THE TEST RETURNS FAILED BUT THE IMAGE IS SHOWN

int(video_test_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y) {
  uint16_t mode = 0x105;

  if (video_init_page_flipping(mode) != 0) {
    return 1;
  }

  video_clear_back_buffer(0);

  if (vg_draw_xpm(xpm, x, y) != 0) {
    vg_exit();
    return 1;
  }

  if (video_swap_buffers() != 0) {
    vg_exit();
    return 1;
  }

  if (wait_for_esc() != 0) {
    vg_exit();
    return 1;
  }

  if (vg_exit() != 0) {
    return 1;
  }

  return 0;
}
