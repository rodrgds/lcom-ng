// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>
#include <lcom/liblm.h>
#include "proj.h"

// Any header files included below this line should have been created by you
#include "main_menu.h"

extern Interface background;

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/proj/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/proj/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

static int print_usage() {
  printf("Usage: <mode - hex>\n");

  return 1;
}

int(proj_main_loop)(int argc, char *argv[]) {
  /* 
   * Substitute the code below by your own
   */
  //
  // if you're interested, try to extend the command line options so that the usage becomes:
  // <mode - hex> <minix3 logo  - true|false> <grayscale - true|false> <delay (secs)>
  //
  bool const minix3_logo = true;
  bool const grayscale = false;
  uint8_t const delay = 1;
  uint16_t mode;

  if (argc != 1)
    return print_usage();

  // parse mode info (it has some limitations for the sake of simplicity)
  if (sscanf(argv[0], "%hx", &mode) != 1) {
    printf("%s: invalid mode (%s)\n", __func__, argv[0]);

    return print_usage();
  }

  mode = 0x117;
  
  if (set_graphic_mode(mode) != OK) {
    if(vg_exit() != OK) return NOT_OK;
    printf("Failed to set graphic mode\n");
    return NOT_OK;
  }

  uint8_t bit_no_mouse = 0 , bit_no_timer = 0 , bit_no_kbd = 0 , bit_no_rtc = 0; 

  if(mouse_data_reporting(true) != OK) return NOT_OK;

  if(mouse_subscribe_int(&bit_no_mouse) != OK) return SUBSCRIBE_FAIL;
  uint32_t irq_set_mouse = BIT(bit_no_mouse);

  if(timer_subscribe_int(&bit_no_timer) != OK) return SUBSCRIBE_FAIL;
  uint32_t irq_set_timer = BIT(bit_no_timer);

  if(kbd_subscribe_int(&bit_no_kbd) != OK) return SUBSCRIBE_FAIL;
  uint32_t irq_set_kbd = BIT(bit_no_kbd);

  if(rtc_subscribe_int(&bit_no_rtc) != OK) return SUBSCRIBE_FAIL;
  uint32_t irq_set_rtc = BIT(bit_no_rtc);

  background.irq_timer = irq_set_timer;
  background.irq_kbd = irq_set_kbd;
  background.irq_mouse = irq_set_mouse;
  background.irq_rtc = irq_set_rtc;

  if(mainLoop() != OK) return NOT_OK;

  if(rtc_unsubscribe_int() != OK) return UNSUBSCRIBE_FAIL;
  if(kbd_unsubscribe_int() != OK) return UNSUBSCRIBE_FAIL;
  if(timer_unsubscribe_int() != OK) return UNSUBSCRIBE_FAIL;
  if(mouse_unsubscribe_int() != OK) return UNSUBSCRIBE_FAIL;
  if(mouse_data_reporting(false) != OK) return UNSUBSCRIBE_FAIL;

  printf("MODE IS %d , %x\n",mode,mode);
  return proj_demo(mode, minix3_logo, grayscale, delay);
}
