#include <drivers/i8042.h>
#include <lcom/lcf.h>
#include <stdio.h>
#include <game/controllers/keyboardLogic.h>
#include "game/gameLogic.h"
#include "drivers/graphics.h"
#include "drivers/i8042.h"
#include <lcom/xpm.h>

extern vbe_mode_info_t info;

/**
 * @brief Entry point of the LCOM program.
 * Initializes LCF and starts the main game loop.
 */
int main(int argc, char *argv[]) {
  lcf_set_language("EN-US");
  lcf_trace_calls("/home/lcom/labs/project/src/trace.txt");
  lcf_log_output("/home/lcom/labs/project/src/output.txt");
  srand(time(NULL)); 

  if (lcf_start(argc, argv))
    return 1;

  lcf_cleanup();
  return 0;
}

/**
 * @brief Main loop of the game service.
 * Subscribes interrupts, handles IPC events, and manages game state transitions.
 */
int(proj_main_loop)(int argc, char *argv[]) {
  message msg;
  int ipc_status, index = 0, r;
  uint8_t read = 0;
  uint8_t kbc_bit_no = 0;
  uint8_t mouse_bit_no = 1;
  uint8_t timer_bit_no = 10;
  uint8_t bytes[3];
  struct packet pp;
  uint8_t *map = NULL;

  // Initialize graphical and game resources
  initialize();

  struct mouseCoord mouseC = { .mouse_x = 0, .mouse_y = 0 };
  struct Signal game_signal = { .mouse_pressed = 0, .key_pressed = 0, .key_code = 0x00 };

  struct Game game = {
    .mouseC = mouseC,
    .signal = game_signal,
    .state = STATE_MENU,
    .map = map
  };
  
  if(timer_subscribe_int(&timer_bit_no)){
      return 1;
  }
  if (KBC_subscribe_int(&kbc_bit_no))
    return 1;
  if (mouse_enable_data())
    return 1;
  if (MC_subscribe_int(&mouse_bit_no))
    return 1;
  if (vg_enter(mode_6))
    return 1;
  if (vg_int(mode_6))
    return 1;

  // Game loop
  while (game.state != STATE_END) {
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d\n", r);
      continue;
    }

    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
      case HARDWARE:
        // Timer interrupt: update game logic
        if (msg.m_notify.interrupts & BIT(timer_bit_no)) {
          gameHandler(&game);
        }

        // Keyboard interrupt: process key press
        if (msg.m_notify.interrupts & BIT(kbc_bit_no)) {
          KBC_int_handler(&read);
          if (!(read & BIT(7)) && read != 0xe0) {
            game.signal.key_code = read;
            game.signal.key_pressed = 1;
          }
        }

        // Mouse interrupt: read and parse mouse packet
        if (msg.m_notify.interrupts & BIT(mouse_bit_no)) {
          MC_int_handler(&read);
          if ((read & BIT(3)) && index == 0) {
            bytes[index++] = read;
          } else if (index != 0 && index <= 2) {
            bytes[index++] = read;
          }

          if (index == 3) {
            Write_to_struct_packet(bytes, &pp);
            updateMouse(&game, pp.delta_y, pp.delta_x);
            game.signal.mouse_pressed = pp.lb;
            index = 0;
          }
        }
        break;
      }
    }
  }

  // Cleanup
  if (KBC_unsubscribe_int()) return 1;
  if (MC_unsubscribe_int()) return 1;
  if (mouse_disable_data_reporting()) return 1;
  if (vg_exit()) return 1;
  if (timer_unsubscribe_int()) return 1;

  return 0;
}
