#include <lcom/lab5.h>
#include <lcom/lcf.h>
#include "vbe_macros.h"
#include "video_gr.h"
#include "kbc.h"

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab5/trace.txt");

  // enables to save the output of zprintf function calls on a file
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

int (video_test_init)(uint16_t mode, uint8_t delay) {
  reg86_t r86;

  memset(&r86, 0, sizeof(r86)); 

  r86.intno = VBE_INT;
  r86.ax = VBE_SET_MODE;
  r86.bx = mode | VBE_LINEAR_FRAME_BUFFER;

  if (sys_int86(&r86) != OK) return 1;

  sleep(delay);

  vg_exit();

  return 0; 
}

extern uint8_t scancode_byte; 

int (video_test_rectangle)(uint16_t mode, uint16_t x, uint16_t y,
                           uint16_t width, uint16_t height, uint32_t color) {

  if (vg_init(mode) == NULL) return 1;

  // loop para desenhar linhas horizontais
  for (int i = 0; i < height; i++) {
    if (vg_draw_hline(x, y + i, width, color) != 0) {
      vg_exit(); 
      return 1;
    }
  }

  // esperar a injecoa do esc
  if (wait_esc() != 0) {
      vg_exit();
      return 1;
  }

  return vg_exit();
}


int (video_test_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y) {
  
  // 1. Iniciar o modo gráfico com 0x105 para XPM_INDEXED)
  if (vg_init(0x105) == NULL) return 1;

  if (vg_draw_xpm(xpm, x, y) != 0) {
    vg_exit();
    return 1;
  }

  // esperar a injecao do esc
  if (wait_esc() != 0) {
      vg_exit();
      return 1;
  }

  return vg_exit();
}

