#include "pause.h"

int optionPause = 0;
extern enum STATE GameState;

// loads xpm
extern uint8_t *resume;
extern xpm_image_t imgResume;

extern uint8_t *pauseExit;
extern xpm_image_t imgPauseExit;


void PauseTimerHandler() {
  drawPause(optionPause);
}

void PauseHandlerKBC(enum KEY k) {
  switch (k)
  {
  case DOWN:
    if(optionPause == 1) {
      optionPause = 1;
    }
    else {
      optionPause++;
    }
    break;
  
  case UP:
    if(optionPause == 0) {
      optionPause = 0;
    }
    else {
      optionPause--;
    }
    break;
  case ENTER:
    if(optionPause == 0) {
      GameState = PLAY_SOLO;
    }
    else if(optionPause == 1) {
      GameState = EXIT;
    }
    break;
  default:
    break;
  }
}

void drawPause(int opt) {
  switch (opt)
  {
  case 0:
    draw_pix_map(0,0,resume,imgResume);
    break;
  
  case 1:
    draw_pix_map(0,0,pauseExit,imgPauseExit);
    break;
  }
}
