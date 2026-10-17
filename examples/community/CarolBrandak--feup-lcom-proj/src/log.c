#include "log.h"
#include "graphics_card.h"
#include "macros.h"

extern int xRes;


void initLog(Log * log, int count){
  xpm_image_t img;
  xpm_load(log_xpm,XPM_8_8_8_8,&img);
  srand(time(NULL));
  switch (count)
  {
  case 0:
    log->x= rand() % ((xRes/2)-img.width) + 1;
    log->y=105;
    log->speed=STARTING_LOG3_SPEED;
    break;
  case 1:
    log->x=rand() % ((xRes/2)-img.width) + (xRes/2);
    log->y=105;
    log->speed=STARTING_LOG3_SPEED;
    printf("%d",log->x);
    break;
  case 2:
    log->x= rand() % ((xRes/2)-img.width) + 1;
    log->y=165;
    log->speed=STARTING_LOG2_SPEED;
    break;
  case 3:
    log->x=rand() % ((xRes/2)-img.width) + (xRes/2);
    log->y=165;
    log->speed=STARTING_LOG2_SPEED;
    break; 
  case 4:
    log->x= rand() % ((xRes/2)-img.width) + 1;
    log->y=285;
    log->speed=STARTING_LOG1_SPEED;
    break;
  case 5:
    log->x=rand() % ((xRes/2)-img.width) + (xRes/2);
    log->y=285;
    log->speed=STARTING_LOG1_SPEED;
    break;                 
  
  default:
    break;
  }
  log->img=img;  
}
