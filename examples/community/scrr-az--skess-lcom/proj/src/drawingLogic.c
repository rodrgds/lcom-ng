#include "drawingLogic.h"

typedef enum{
  NOT_DRAWING,
  DRAWING
} DrawingState;

DrawingState state = NOT_DRAWING;

void (update_drawing_state)(struct packet mouse_packet){
  switch (state){
    case NOT_DRAWING:
      if (mouse_packet.lb && !mouse_packet.rb && !mouse_packet.mb){
        state = DRAWING;
      }
      break;
    case DRAWING:
      if (!mouse_packet.lb || mouse_packet.rb || mouse_packet.mb){
        state = NOT_DRAWING;
      }
      break;
  }
}

int (update_drawing_screen)(int initial_x, int initial_y, int final_x, int final_y, uint32_t drawingScreen[]){ //bresenham's line generation algorithm
  if(state == NOT_DRAWING) return 0;

  int dx_abs = abs(final_x - initial_x);
  int dy_abs = abs(final_y - initial_y);

  bool increase_x = initial_x < final_x;
  bool increase_y = initial_y < final_y;

  int err = dx_abs - dy_abs;

  while (initial_x != final_x || initial_y != final_y) {

    if(initial_y * MAX_X_GUESSING + initial_x > MAX_X_GUESSING * MAX_Y_GUESSING) return 1;
    drawingScreen[initial_y * MAX_X_GUESSING + initial_x] = 0x000000;

    int e2 = 2 * err;
    if (e2 > -dy_abs) {
      err -= dy_abs;
      
      if(increase_x) initial_x++;
      else initial_x--;
    }
    if (e2 < dx_abs) {
      err += dx_abs;
      
      if(increase_y) initial_y++;
      else initial_y--;
    }
  }

  return 0;
}

int (mouse_event_handler_guessing)(xpm_image_t img, struct packet mouse_packet, int* x, int* y, uint32_t drawingScreen[], int* c_mouse){
  update_drawing_state(mouse_packet);

  int start_x = *x, start_y = *y;
  calculate_placement(x, y, MAX_X_GUESSING - img.width + 50, 864, 50, 50, mouse_packet);

  update_drawing_screen(start_x - 50, start_y - 50, *x - 50, (*y - 50) > MAX_Y_GUESSING ? MAX_Y_GUESSING : (*y - 50), drawingScreen);

  if ((mouse_packet.lb) && (*x >= 495 && *x <= 750) && (*y >= 795 && *y <= 850)){
    reset_drawing_area(drawingScreen);
    *c_mouse = 2;
  }

  return 0;
}

int (reset_drawing_area)(uint32_t drawingScreen[]){
  uint32_t* pointer = &drawingScreen[0];

  //make the drawing area white
  for (int i = 0; i < MAX_Y_GUESSING; i++){
    for (int j = 0; j < MAX_X_GUESSING; j++){
      *pointer = DRAWING_AREA_COLOR;
      pointer++;
    }
  }

  //make the bottom part backgroung color (mouse can leave by the bottom part)
  for (int i = 0; i < 84; i++){
    for (int j = 0; j < MAX_X_GUESSING; j++){
      *pointer = BACKGROUND_COLOR;
      pointer++;
    }
  }

  return 0;
}
