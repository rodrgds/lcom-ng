/*! \mainpage Tennis For Two - LCOM

* Este projeto foi desenvolvido para a unidade curricular de Laboratório de Computadores. Teve como
* objetivo a aplicação dos conceitos apresentados durante as aulas relacioandos com os vários
* dispositivos de um computador o rato ou o teclado.
* 
*
* O trabalho consiste num jogo chamado Tennis for Two onde o utilizador controla um jogador de ténis que tem como objetivo marcar mais pontos que o adversário. Ganha o primeiro a marcar 10 pontos.
*/
#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>
#include "devices/rtc.h"
#include "devices/kbd.h"
#include "devices/i8042.h"
#include "xpms.h"
#include "devices/VBE.h"
#include "devices/video.h"
#include "game.h"

extern uint8_t scancode;

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

int (proj_main_loop)(int argc, char **argv) {
  if(map_VRAM(VBE_MODE_8888) != 0){
    return EXIT_FAILURE;
  }

  
  if(gameLoop() != 0){
    printf("failed");
    return EXIT_FAILURE;
  }
  
  //exits the graphics mode
  if(vg_exit() != 0){
    return EXIT_FAILURE;
  }

  return 0;
}
