/*! \mainpage Main Page
- **Project name:** LCOM18G4 - Brick Blitz
- **Short description:** Breakout Clone
- **Environment:** Minix console
- **Tools:** C
- **Institution:** [FEUP](https://sigarra.up.pt/feup/en/web_page.Inicial)
- **Course:** [LC](https://sigarra.up.pt/feup/en/UCURR_GERAL.FICHA_UC_VIEW?pv_ocorrencia_id=520323) (Computer Laboratory)
- **Project grade:** TBA
- **Group members:**
    1. Afonso Castro (up202208026@fe.up.pt)
    2. José Martins (up202204857@fe.up.pt)
    3. Pedro Santos (up202205900@fe.up.pt)
    4. Rodrigo de Sousa (up202205751@fe.up.pt)
 */

// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>
#include "game/game.h"

extern uint8_t bit_no;
extern Paddle paddle;
extern Brick bricks[32];
extern Ball ball;

// Any header files included below this line should have been created by you

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

int (proj_main_loop)(int argc, char *argv[]){
    if (prepare_keyboard()) return 1;
    if(prepare_timer()) return 1;
    if (prepare_video()) return 1;
    if (prepare_mouse()) return 1;
    if (prepare_screens()) return 1;
    if (prepare_objects()) return 1;
    if(draw_init()) return 1;
    if (run()) return 1;
    if (disable_mouse()) return 1;
    if (disable_timer()) return 1;
    if (disable_video()) return 1;
    if (disable_keyboard()) return 1;

    return 0;
}
