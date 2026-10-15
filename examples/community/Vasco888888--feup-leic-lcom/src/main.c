/**
 * @file main.c
 * @brief Entry point for the Dr.Minix project. Initializes and manages the main program loop.
 */

#include "conf.h"
#include "game/game.h"
#include "game/tile.h"
#include "highscore/highscore.h"
#include "keyboard/KBC.h"
#include "keyboard/keyboard.h"
#include "menu/menu.h"
#include "timer/timer.h"
#include "video/VBE.h"
#include "video/graphics.h"
#include <lcom/lcf.h>
#include <stdbool.h>
#include <stdio.h>


vbe_mode_info_t mode_info;
extern uint8_t *back_buffer;
extern size_t buffer_size;

/**
 * @brief Main entry point. Initializes LCF and starts the main loop.
 * @param argc Argument count
 * @param argv Argument vector
 * @return 0 on success, non-zero otherwise
 */
int main(int argc, char *argv[]) {
  // sets the language for printf
  lcf_set_language("EN-US");
  lcf_trace_calls("/home/lcom/labs/proj/trace.txt");
  lcf_log_output("/home/lcom/labs/proj/output.txt");

  if (lcf_start(argc, argv))
    return 1;

  lcf_cleanup();
  return 0;
}

/**
 * @brief Project main loop. Handles the main menu, game, and highscore screens.
 * @param argc Argument count
 * @param argv Argument vector
 * @return 0 on success, non-zero otherwise
 */
int(proj_main_loop)(int argc, char *argv[]) {
  // Entrar em modo gráfico
  if (set_graphic_mode(MODE_0) != 0) {
    printf("Error setting graphics mode\n");
    return 1;
  }

  if (vbe_get_mode_info(MODE_0, &mode_info) != 0) {
    printf("Error getting mode info\n");
    vg_exit();
    return 1;
  }

  if (set_frame_buffer(MODE_0) != 0) {
    printf("Error setting framebuffer\n");
    vg_exit();
    return 1;
  }

  if (init_back_buffer() != 0) {
    vg_exit();
    return 1;
  }

  // Inicializar o gerador de números aleatórios
  srand(time(NULL));

  while (1) {
    // Inicializar o menu
    printf("Starting menu...\n");
    if (menu_loop() != 0) {
      printf("Ended menu loop\n");
      vg_exit();
      break; // Sair do loop do menu
    }
    // Inicializar o jogo
    printf("Starting game...\n");
    if (game_loop() != 0) {
      printf("Ended game loop\n");
      vg_exit();
      return 1;
    }
    // highscore
    printf("Checking highscore...\n");
    if (highscore_loop() != 0) {
      printf("Ended highscore loop\n");
      vg_exit();
      return 1;
    }
  }
  printf("%d \n", sys_hz());
  if (back_buffer)
    free(back_buffer);
  vg_exit(); // Sair do modo gráfico
  return 0;
}
