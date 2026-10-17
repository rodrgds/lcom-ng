#include <lcom/lcf.h>
#include <stdio.h>

#include "controllers/utils.h"
#include "controllers/keyboard_mouse/keyboard.h"
#include "controllers/video/graphic.h"
#include "sprites/sprites.h"
#include "controllers/timer/timer.h"
#include "game_logic/game/Game.h"
#include "menu/menu.h"

/// @brief Global variable to store the current game screen state
extern GameScreen current_screen;

/**
 * @brief Main function of the program.
 * 
 * Initializes the LCF framework and starts the main loop.
 * 
 * @param argc Argument count
 * @param argv Argument vector
 * @return int Returns 0 on success, 1 on failure
 */
int main(int argc, char *argv[]) {
    lcf_set_language("EN-US");
    lcf_trace_calls("/home/lcom/labs/grupo_2leic13_7/proj/trace.txt");
    lcf_log_output("/home/lcom/labs/grupo_2leic13_7/proj/output.txt"); // Set log output file

    if (lcf_start(argc, argv))  // Start LCF framework
        return 1;
    return 0;
}

/**
 * @brief Entry point for the main event loop of the game.
 * 
 * Initializes graphics, loads resources, sets up interrupts, and handles
 * keyboard and timer events for game and menu navigation.
 * 
 * @param argc Argument count
 * @param argv Argument vector
 * @return int Returns 0 on success, 1 on failure
 */

int(proj_main_loop)(int argc, char *argv[]) {
  // Initialize graphics mode
  if (set_frame_buffer(0x14C) || set_graphic_mode(0x14C)) {
    printf("Failed to set graphics mode.\n");
    return 1;
  }

  // Loading do jogo:
  sprites_carregar_1();
  sprite_desenhar(loading1,0,0);
  swap_buffers();

  sprites_carregar_2();
  sprite_desenhar(loading2,0,0);
  swap_buffers();

  sprites_carregar_3();
  sprite_desenhar(loading3,0,0);
  swap_buffers();

  sprites_carregar_4();
  sprite_desenhar(loading4,0,0);
  swap_buffers();

  sprites_carregar_5();
  sprite_desenhar(loading5,0,0);
  swap_buffers();

  sprites_carregar_6();
  sprite_desenhar(loading6,0,0);
  swap_buffers();

  sprites_carregar_7();
  sprite_desenhar(loading7,0,0);
  swap_buffers();

  sprites_carregar_8();
  sprite_desenhar(loading8,0,0);
  swap_buffers();

  sprites_carregar_9();
  sprite_desenhar(loading9,0,0);
  swap_buffers();

  sprites_carregar_10();
  sprite_desenhar(loading10,0,0);
  swap_buffers();

  sprites_carregar_11();
  sprite_desenhar(loading11,0,0);
  swap_buffers();

  sprites_carregar_12();
  sprite_desenhar(loading12,0,0);
  swap_buffers();

  sprites_carregar_13();
  sprite_desenhar(loading13,0,0);
  swap_buffers();

  sprites_carregar_14();
  sprite_desenhar(loading14,0,0);
  swap_buffers();

  sprites_carregar_15();
  sprite_desenhar(loading15,0,0);
  swap_buffers();

  sprites_carregar_16();
  sprite_desenhar(loading16,0,0);
  swap_buffers();

  // Subscrição de interrupções do teclado
  uint8_t irq_set;
  if (kbd_subscribe_int(&irq_set) != 0) {
    printf("Failed to subscribe keyboard interrupt\n");
    return 1;
  }

  // Subscribe to timer interrupts
  uint8_t bit_no_timer;
  if (my_timer_subscribe_int(&bit_no_timer) != 0) {
      printf("Failed to subscribe timer interrupt\n");
      return 1;
  }

  // Set timer to 60 Hz for smooth gameplay
  if (my_timer_set_frequency(0, 60) != 0) {
    printf("Failed to set timer frequency\n");
    return 1;
  }

  // Desenho inicial
  menu_init();

  my_vg_draw_rectangle(0, 0, mode_info.XResolution, mode_info.YResolution, 0x0000FF);
  menu_draw(&irq_set);
  sprite_desenhar(mouseCursorSprite, cursor_x, cursor_y);
  swap_buffers();

  // Main event loop variables
  int ipc_status;
  message msg;
  // Main game loop - runs until user quits
  while (!menu_should_quit()) {
    // Wait for interrupts or messages
    if (driver_receive(ANY, &msg, &ipc_status) != 0) {
      printf("Error in driver_receive\n");
      continue;
    }
    // Check if we received a hardware interrupt
    if (is_ipc_notify(ipc_status)) {
      if (_ENDPOINT_P(msg.m_source) == HARDWARE && (msg.m_notify.interrupts & irq_set)) {
        // Handle keyboard interrupt
        kbd_ih();

        menu_handle_input(last_keycode);  //Handle menu navigation input

        // Redraw screen with updated state
        my_vg_draw_rectangle(0, 0, mode_info.XResolution, mode_info.YResolution, 0x0000FF);
        menu_draw(&irq_set);
        sprite_desenhar(mouseCursorSprite, cursor_x, cursor_y);
        swap_buffers();
      }
    }
  }

  // Unsubscribe from keyboard interrupts
  if (kbd_unsubscribe_int() != 0) {
    printf("Failed to unsubscribe keyboard interrupt\n");
  }

  // Unsubscribe from timer interrupts
  if (my_timer_unsubscribe_int() != 0) {
    printf("Failed to unsubscribe timer interrupt\n");
  }

  // Clear screen before exiting
  my_vg_draw_rectangle(0, 0, mode_info.XResolution, mode_info.YResolution, 0x0000FF);  

  // Final cleanup (e.g. ESC key wait)
  //ESC_wait();

  // Free loaded sprite resources
  sprites_liberar();
  libertar_cartas_fruta(ABACATE);
  libertar_cartas_fruta(BANANA);
  libertar_cartas_fruta(CEREIJA);
  libertar_cartas_fruta(COCO);
  libertar_cartas_fruta(LARANJA);
  libertar_cartas_fruta(MACA);
  libertar_cartas_fruta(MELANCIA);
  libertar_cartas_fruta(MORANGO);

  // Restore text mode and clean up LCF
  set_text_mode();
  lcf_cleanup();

  return 0;
}

