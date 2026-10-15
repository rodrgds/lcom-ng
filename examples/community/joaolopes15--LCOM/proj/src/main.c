#include <lcom/lcf.h>
#include <lcom/timer.h>
#include "drivers/keyboard/keyboard.h"
#include "drivers/mouse/mouse.h"
#include "drivers/video/video.h"
#include "game/game.h"

extern uint8_t scancode;
extern int counter;
extern struct packet m_packet;
extern int byte_idx;

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/proj/src/trace.txt");

  // enables to save the output of printf function calls on a file
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

int (proj_main_loop)(int argc, char *argv[]) {
  int ipc_status;
  message msg;
  uint8_t irq_set_keyboard, irq_set_timer, m_irq_set;   
  game_t* game;

  // Initialize the game
  game = game_init();
  if (game == NULL) {
    printf("Failed to initialize game\n");
    return 1;
  }

  if (timer_set_frequency(0, 60) != 0) {
    game_exit(game);
    return 1;
  }

  if (timer_subscribe_int(&irq_set_timer) != 0) {
    game_exit(game);
    return 1;
  }

  if (keyboard_subscribe_int(&irq_set_keyboard) != 0) {
    mouse_unsubscribe_int();
    timer_unsubscribe_int();
    game_exit(game);
    return 1;
  }

  if (mouse_subscribe_int(&m_irq_set) != 0) {
    keyboard_unsubscribe_int();
    timer_unsubscribe_int();
    game_exit(game);
    return 1;
  }

  // Enable mouse data reporting
  if (m_write(ENABLE_DATA_REPORTING) != 0) {
    keyboard_unsubscribe_int();
    timer_unsubscribe_int();
    mouse_unsubscribe_int();
    game_exit(game);
    return 1;
  }

  if (vg_init(0x115) == NULL) {
    keyboard_unsubscribe_int();
    timer_unsubscribe_int();
    mouse_unsubscribe_int();
    game_exit(game);
    return 1;
  }

  // Main game loop
  while (game->current_state != STATE_EXIT) {
    if (driver_receive(ANY, &msg, &ipc_status) != 0) {
      continue;
    }
    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          if (msg.m_notify.interrupts & irq_set_keyboard) {
            kbc_ih();

            if (scancode != 0) {
              game_process_input(game, scancode);
            }
          }
          
          if (msg.m_notify.interrupts & irq_set_timer) {
            timer_int_handler();
            
            game_update(game);
            game_render(game);
            
            flip();
          }

          if (msg.m_notify.interrupts & m_irq_set) {
            mouse_ih();
            packet_assembly();
            if (byte_idx == 3) {
              m_assemble_packet();
              game_process_mouse_input(game, &m_packet);
              byte_idx = 0; // Reset byte index after processing
            }
          }
          break;
          
        default:
          break;
      }
    } else {
      // Handle other messages
    }
  }

  
  if (timer_unsubscribe_int() != 0) {
    printf("Failed to unsubscribe timer interrupts\n");
  }
  
  if (keyboard_unsubscribe_int() != 0) {
    printf("Failed to unsubscribe keyboard interrupts\n");
  }

  if (mouse_unsubscribe_int() != 0) {
    printf("Failed to unsubscribe mouse interrupts\n");
  }
  
  // Clean up resources
  if (vg_exit() != 0) {
    printf("Failed to exit video mode\n");
  }
  
  // Free game resources
  game_exit(game);
  
  return 0;
}
