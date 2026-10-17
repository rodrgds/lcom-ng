#include "vehicle.h"
#include "graphics_card.h"
#include "macros.h"

extern int xRes;

void initVehicle(Vehicle * vehicle, int count){
  srand(time(NULL));
  xpm_image_t img;
  xpm_image_t img2;
  xpm_load(car_xpm,XPM_8_8_8_8,&img);
  xpm_load(truck_xpm,XPM_8_8_8_8,&img2);
  switch (count)
  {
  case 0:
    vehicle->x=rand() % ((xRes/2)-img.width) + 1;
    vehicle->y=467;
    vehicle->car=false;
    vehicle->speed = STARTING_TRUCK_SPEED;
    vehicle->img2=img2;
    break;
  case 1:
    vehicle->x=rand() % ((xRes/2)-img.width) + (xRes/2);
    vehicle->y=467;
    vehicle->car=false;
    vehicle->speed = STARTING_TRUCK_SPEED;
    vehicle->img2=img2;
    break;
  case 2:
    vehicle->x=rand() % ((xRes/2)-img.width) + 1;
    vehicle->y=574;
    vehicle->car=true;
    vehicle->speed = STARTING_CAR_SPEED;
    vehicle->img=img;
    break;
  case 3:
    vehicle->x=rand() % ((xRes/2)-img.width) + (xRes/2);
    vehicle->y=574;
    vehicle->car=true;
    vehicle->speed = STARTING_CAR_SPEED;
    vehicle->img=img;
    break; 
  case 4:
    vehicle->x=rand() % ((xRes/2)-img.width) + 1;
    vehicle->y=663;
    vehicle->car=false;
    vehicle->speed = STARTING_TRUCK_SPEED;
    vehicle->img2=img2;
    break;
  case 5:
    vehicle->x=rand() % ((xRes/2)-img.width) + (xRes/2);
    vehicle->y=663;
    vehicle->car=false;
    vehicle->speed = STARTING_TRUCK_SPEED;
    vehicle->img2=img2;
    break;                 
  
  default:
    break;
  }
    
}
