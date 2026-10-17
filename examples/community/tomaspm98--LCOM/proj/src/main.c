#include <lcom/lcf.h>
#include <string.h>
#include "graphics/video_gr.h"
#include "timer/i8254.h"
#include "colors.h"
#include "entity.h"
#include "piece.h"
#include "ball.h"
#include "interrupts.h"
#include "images/transferir.xpm"
#include <stdlib.h>


extern int piece_1_x;
extern int piece_1_y;
extern int piece_2_x;
extern int piece_2_y;
extern int ball_x;
extern int ball_y;
extern bool not_random;
extern int side_start;
extern bool startBall;
extern int ball_var_y;
extern int ball_var_x;
extern bool goal;
extern int goal_left;
extern int goal_right;

int (main)(int argc, char *argv[]) {
  lcf_set_language("EN-US");
  lcf_trace_calls("/home/lcom/labs/g4/proj/src/debug/trace.txt");
  lcf_log_output("/home/lcom/labs/g4/proj/src/debug/output.txt");
  if (lcf_start(argc, argv)) return 1;
  lcf_cleanup();
  return 0;
}

int start(){
    if (timer_set_frequency(0,GAME_FREQ)) return 1;
    //iniciar buffers video
    if(set_graphic_mode(0x115)) return 1;
    if (vg_init(0x115)) return 1;
    //falta interrupts mouse+rtc+serial port
    if (subscribe_interrupts()) return 1; 
    allocate_img_buffer();
    //allocateDrawBuffer();

    return 0;
}

int end(){
    free_im_buffer();
    if (vg_exit()) return 1;
    //terminar sprites
    //falta interrupts mouse+rtc+serial port
    if (unsubscribe_interrupts()) return 1;
    //freeDrawBuffer();
    //desativar interrupts

    return 0;
}

int (proj_main_loop)(int argc, char *argv[]){
  if (start()) return end();

  interrupts();

  if (end()) return 1;
  return 0;
}
