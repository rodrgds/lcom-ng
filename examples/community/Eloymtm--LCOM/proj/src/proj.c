#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>

#include <model/game.h>
#include <model/board.h>
#include <state/state.h>
#include <viewer/menuViewer.h>

// Any header files included below this line should have been created by you
extern uint8_t scancode;
int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/g2/proj/src/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/g2/proj/src/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasksmak
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int (proj_main_loop)(int argc, char *argv[]) {

    if(allocVirtMem(0x14C)!=0)return 1;

    if(set_graphic_mode(0x14C)!=0)return 1;

    if(drawMenu()!=0)return 1;

    STATE state = MENU;

    Game *game = StartGame(state);

    game->state = MENU;

    if(GameLoop(game) != 0)return 1;
  
    if(vg_exit()!=0)return 1;

    free(game->board1);
    free(game->board2);
    free(game);

    return 0;
}
