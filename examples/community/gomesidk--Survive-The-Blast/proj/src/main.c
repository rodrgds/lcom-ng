
/**
 * @file main.c
 * 
 * This file contains the main function and the interrupt loop for the game.
 * It initializes the graphics mode, subscribes to interrupts, and handles the game state.
 */

#include <lcom/lcf.h>
#include "graphics.h"
#include "kbd.h"
#include "mouse.h"
#include "timer_count.h"
#include "spriteLoader.h"
#include "cursor.h"
#include "state.h"
#include "key_pressed.h"
#include "serial_port.h"
#include "queue.h"

static SpriteLoader *loader = NULL;
static Cursor *c = NULL;
static KeyPressed *key_pressed = NULL;
static State *state = NULL;

int main(int argc, char *argv[]) {
  lcf_set_language("EN-US");
  lcf_trace_calls("/home/lcom/labs/lab5/trace.txt");
  lcf_log_output("/home/lcom/labs/lab5/output.txt");

  if (lcf_start(argc, argv))
    return 1;

  lcf_cleanup();
  return 0;
}


/**
 * @brief Main loop that handles interrupts and updates the game state.
 * 
 * This function subscribes to keyboard, mouse, timer, and serial port interrupts,
 * processes the input, updates the game state, and draws the current state on the screen.
 */
void interrups_loop() {
  if ( mouse_write_command(KBC_ENABLE_DATA_REPORT) != 0 ) {
    printf("Error enabling data reporting\n");
    return;
  }

  uint8_t bit_no_timer, bit_no_kbd, bit_no_mouse, bit_no_serial;
  int ipc_status;
  message msg;

  if (timer_subscribe_int(&bit_no_timer) != 0) {
    printf("Error subscribing timer interrupts\n");
    return;
  }
  if (kbd_subscribe_int(&bit_no_kbd) != 0) {
    printf("Error subscribing kbd interrupts\n");
    return;
  }
  if (mouse_subscribe_int(&bit_no_mouse) != 0) {
    printf("Error subscribing mouse interrupts\n");
    return;
  }
  if (sp_subscribe_int(&bit_no_serial) != 0) {
    printf("Error subscribing serial port interrupts\n");
    return;
  }

  uint32_t irq_set_timer = BIT(bit_no_timer);
  uint32_t irq_set_kbd = BIT(bit_no_kbd);
  uint32_t irq_set_mouse = BIT(bit_no_mouse);
  uint32_t irq_set_serial = BIT(bit_no_serial);

  uint8_t *scancode = get_scancode();

  while ( !is_game_over(state) ) {
    if( driver_receive(ANY, &msg, &ipc_status) != 0 ){
      printf("Error");
      continue;
    }
    if(is_ipc_notify(ipc_status)) {
      switch(_ENDPOINT_P(msg.m_source)){
        case HARDWARE:

          if (msg.m_notify.interrupts & irq_set_serial) {
            sp_ih(); 
          }

          if (msg.m_notify.interrupts & irq_set_kbd) {
            kbc_ih();

            if (check_scancode_complete() == 0){
              key_pressed_update(key_pressed, *scancode);
            }
          }

          if (msg.m_notify.interrupts & irq_set_timer) {
            timer_int_handler();
            if (timer_get_count() % 2 == 0) { // 30 FPS
              clear_back_buffer();

              update_state(state, key_pressed, c, get_sp_byte());
              draw_state(state);

              vg_flip_buffer();
              timer_reset_count();
            }
          }

          if (msg.m_notify.interrupts & irq_set_mouse) {
            mouse_ih();

            if (check_packet_complete() == 1) {
              packet_parse();
              struct packet *pp = get_packet();
              update_cursor_with_mouse(c, pp);
            }
          }

          break;
        default:
          break;
      }
    }
  }

  if (sp_unsubscribe_int() != 0) {
    printf("Error unsubscribing serial port interrupts\n");
    return;
  }

  if (mouse_unsubscribe_int() != 0) {
    printf("Error unsubscribing mouse interrupts\n");
    return;
  }
  if (kbd_unsubscribe_int() != 0) {
    printf("Error unsubscribing kbd interrupts\n");
    return;
  }
  if (timer_unsubscribe_int() != 0) {
    printf("Error unsubscribing timer interrupts\n");
    return;
  }
  if (mouse_write_command(KBC_DISABLE_DATA_REPORT) != 0) {
    printf("Error disabling data reporting\n");
    return;
  }
}


/**
 * @brief Main function that initializes the game and starts the main loop.
 * 
 * This function sets up the graphics mode, initializes the sprite loader, cursor, and state,
 * and then enters the interrupt loop to handle input and update the game state.
 * 
 * @param argc Number of command line arguments
 * @param argv Array of command line arguments
 * @return 0 on success
 */
int (proj_main_loop)(int argc, char *argv[]) {

  map_graphics_vram(VBE_1024p_DC);
  set_video_mode(VBE_1024p_DC);
  sp_init();

  loader = load_sprites();
  key_pressed = key_pressed_create();
  c = create_cursor(get_cursor(loader), get_mode_info().XResolution, get_mode_info().YResolution);
  state = create_state(loader, c);

  interrups_loop();
  
  destroy_state(state);
  destroy_cursor(c);
  key_pressed_destroy(key_pressed);
  destroy_sprites(loader);

  sp_exit();
  vg_exit();

  return 0;
}
