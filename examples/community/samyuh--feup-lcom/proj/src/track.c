#include <math.h>
#include "track.h"

int curva(int car_x, int x, int y, int r, int pos) {
  return pos*sqrt(r*r - pow(car_x-x, 2)) + y;
}

int reta_baixo(CarPos *race , int y_max, int state){
  race->y += race->speed;
  if(race->y >= y_max){
    race->y = y_max;
    race->state = state;
  }
  return 0;
}
int reta_cima(CarPos *race , int y_min, int state){
  race->y -= race->speed;
  if(race->y <= y_min){
    race->y = y_min;
    race->state = state;
  }
  return 0;
}
int reta_direita(CarPos *race , int x_max, int state){
  race->x += race->speed;
  if(race->x >= x_max){
    race->x = x_max;
    race->state = state;
  }
  return 0;
}

int reta_esquerda(CarPos *race , int x_min, int state){
  race->x -= race->speed;
  if(race->x <= x_min){
    race->x = x_min;
    race->state = state;
  }
  return 0;
}
int curva_baixo_direita(CarPos *race , int x_centro, int y_centro, int raio, int state){

  int delta_x;
  int delta_y;

  race->x += race->speed;
  race->y = curva(race->x, x_centro, y_centro, raio, 1);

  delta_x = x_centro - race->x;
  delta_y = race->y - y_centro;

  if (race->x >= x_centro || race->y >= (y_centro + raio)) {
    race->x = x_centro;
    race->y = y_centro + raio;
    race->angle = 270;
    race->state = state;
  }
  else if (delta_x != 0) race->angle = 360 - (atan(delta_y / delta_x) * 180 / 3.14159265359);
  return 0;
}
int curva_direita_cima(CarPos *race , int x_centro, int y_centro, int raio, int state){

  int delta_x;
  int delta_y;

  race->x += race->speed;
  race->y = curva(race->x, x_centro, y_centro, raio, 1);

  delta_x = race->x - x_centro;
  delta_y = y_centro - race->y;

  if (race->x >= (x_centro + raio) || race->y <= y_centro) {
    race->x = x_centro + raio;
    race->y = y_centro;
    race->angle = 180;
    race->state = state;
  }
  else if (delta_x != 0) race->angle = 180 - (atan(delta_y / delta_x) * 180 / 3.14159265359);
  return 0;
}
int curva_cima_esquerda(CarPos *race , int x_centro, int y_centro, int raio, int state){

  int delta_x;
  int delta_y;

  race->x -= race->speed;
  race->y = curva(race->x, x_centro, y_centro, raio, -1);

  delta_x = x_centro - race->x;
  delta_y = y_centro - race->y;

  if (race->x <= x_centro || race->y <= (y_centro - raio)){
    race->x = x_centro;
    race->y = y_centro - raio;
    race->angle = 90;
    race->state = state;
  }
  else if (delta_x != 0) race->angle = 180 +( atan(delta_y / delta_x) * 180 / 3.14159265359 );
  return 0;
}
int curva_esquerda_baixo(CarPos *race , int x_centro, int y_centro, int raio, int state){

  int delta_x;
  int delta_y;

  race->x -= race->speed;
  race->y = curva(race->x, x_centro, y_centro, raio, -1);

  delta_x = x_centro - race->x;
  delta_y = y_centro - race->y;

  if (race->x <= (x_centro - raio) || race->y >= y_centro){
    race->x = x_centro - raio;
    race->y = y_centro;
    race->angle = 0;
    race->state = state;
  }
  else if (delta_x != 0) race->angle = ( atan(delta_y / delta_x) * 180 / 3.14159265359 );
  return 0;
}

int curva_direita_baixo(CarPos *race , int x_centro, int y_centro, int raio, int state){

  int delta_x;
  int delta_y;

  race->x += race->speed;
  race->y = curva(race->x, x_centro, y_centro, raio, -1);

  delta_x = race->x - x_centro;
  delta_y = y_centro - race->y;

  if (race->x >= (x_centro + raio) || race->y >= y_centro){
    race->x = x_centro + raio;
    race->y = y_centro;
    race->angle = 0;
    race->state = state;
  }
  else if (delta_x != 0) race->angle =  - ( atan(delta_y / delta_x) * 180 / 3.14159265359 );
  return 0;
}

int curva_esquerda_cima(CarPos *race , int x_centro, int y_centro, int raio, int state){

  int delta_x;
  int delta_y;

  race->x -= race->speed;
  race->y = curva(race->x, x_centro, y_centro, raio, 1);

  delta_x = x_centro - race->x;
  delta_y = race->y -y_centro;

  if (race->x <= (x_centro - raio) || race->y <= y_centro) {
    race->x = x_centro - raio;
    race->y = y_centro;
    race->angle = 180;
    race->state = state;
  }
  else if (delta_x != 0) race->angle = 180 - (atan(delta_y / delta_x) * 180 / 3.14159265359);
  return 0;
}
int curva_cima_direita(CarPos *race , int x_centro, int y_centro, int raio, int state){

  int delta_x;
  int delta_y;

  race->x += race->speed;
  race->y = curva(race->x, x_centro, y_centro, raio, -1);

  delta_x = x_centro - race->x;
  delta_y = y_centro - race->y;

  if (race->x >= x_centro || race->y <= (y_centro - raio)) {
    race->x = x_centro;
    race->y = y_centro - raio;
    race->angle = 270;
    race->state = state;
  }
  else if (delta_x != 0) race->angle = 180 + (atan(delta_y / delta_x) * 180 / 3.14159265359);
  return 0;
}
int curva_baixo_esquerda(CarPos *race , int x_centro, int y_centro, int raio, int state){
  int delta_x;
  int delta_y;

  race->x -= race->speed;
  race->y = curva(race->x, x_centro, y_centro, raio, 1);

  delta_x = race->x - x_centro ;
  delta_y = race->y - y_centro ;

  if (race->x <= x_centro || race->y >= (y_centro + raio)) {
    race->x = x_centro;
    race->y = y_centro + raio;
    race->angle = 90;
    race->state = state;
  }
  else if (delta_x != 0) race->angle = +(atan(delta_y / delta_x) * 180 / 3.14159265359);
  return 0;

}



void runTrack1(CarPos *race) {
  //int delta_x, delta_y;
  switch (race->state) {
    case 0:
      //Lap Count
      if (race->newlap) {
        (race->lapnum)++;
        race->newlap = false;
      }
      reta_baixo(race,10*64,2);
      break;
    //Primeira curva
    case 2:
      curva_baixo_direita(race,2*64,10*64,32,3);
      break;
    case 3:
      reta_direita(race,12*64,4);
      break;
    case 4:
      curva_direita_cima(race,12*64,10*64,32,5);
      break;
    case 5:
      reta_cima(race,4*64,6);
      break;
    case 6:
      curva_cima_esquerda(race,12*64,4*64,32,7);
      break;
    case 7:
      reta_esquerda(race,2*64,8);
      break;
    case 8:
      curva_esquerda_baixo(race,2*64,4*64,32,9);
      break;
    case 9:
    if(race->y == 6*64) {
        race->newlap = true;
        race->state = 0;
      }
      else reta_baixo(race,6*64,9);
      break;
    default:
      break;
  }
}

void runTrack2(CarPos *race) {
  //int delta_x, delta_y;
  switch (race->state) {
    case 0:
      reta_baixo(race,7*64,2);
      break;
    //Primeira curva
    case 2:
      curva_baixo_direita(race,2*64,7*64,32,3);
     break;
    case 3:
      reta_direita(race,3*64,4);
      break;
    case 4:
      curva_direita_baixo(race,3*64,8*64,32,5);
      break;
    case 5:
      reta_baixo(race,10*64,6);
      break;
    case 6:
      curva_baixo_direita(race,4*64,10*64,32,7);
      break;
    case 7:
    if(race->x == 8*64){
        race->newlap = true;
        race->state = 1;
      }
      else reta_direita(race,8*64,7);
      break;
    case 1:
      //Lap Count
      if (race->newlap) {
        (race->lapnum)++;
        race->newlap = false;
      }
      reta_direita(race,12*64,8);
      break;
    case 8:
      curva_direita_cima(race,12*64,10*64,32,9);
      break;
    case 9:
      reta_cima(race,8*64,10);
      break;
    case 10:
      curva_cima_esquerda(race,12*64,8*64,32,11);
      break;
    case 11:
      reta_esquerda(race,6*64,12);
      break;
    case 12:
      curva_esquerda_cima(race,6*64,7*64,32,13);
      break;
    case 13:
      reta_cima(race,6*64,14);
      break;
    case 14:
      curva_cima_direita(race,6*64,6*64,32,15);
      break;
    case 15:
      reta_direita(race,11*64,16);
      break;
    case 16:
      curva_direita_cima(race,11*64,5*64,32,17);
      break;
    case 17:
      reta_cima(race,4*64,18);
      break;
    case 18:
      curva_cima_esquerda(race,11*64,4*64,32,19);
      break;
    case 19:
      reta_esquerda(race,2*64,20);
      break;
    case 20:
      curva_esquerda_baixo(race,2*64,4*64,32,21);
      break;
    case 21:

      if(race->y == 7*64){
        race->newlap = true;
        race->state = 0;
      }
      else reta_baixo(race,7*64,21);
      break;
    default:
      break;
  }
}

void runTrack3(CarPos *race) {
  //int delta_x, delta_y;
  switch (race->state) {
    case 0:
      //Lap Count
      if (race->newlap) {
        (race->lapnum)++;
        race->newlap = false;
      }
      reta_baixo(race,6*64,2);
      break;
    //Primeira curva
    case 2:
      curva_baixo_direita(race,2*64,6*64,32,3);
      break;
    case 3:
      reta_direita(race,3*64,4);
      break;
    case 4:
      curva_direita_baixo(race,3*64,7*64,32,5);
      break;
    case 5:
      reta_baixo(race,8*64,6);
      break;
    case 6:
      curva_baixo_esquerda(race,3*64,8*64,32,7);
      break;
    case 7:
      reta_esquerda(race,2*64,8);
      break;
    case 8:
      curva_esquerda_baixo(race,2*64,9*64,32,9);
      break;
    case 9:
      reta_baixo(race,10*64,10);
      break;
    case 10:
      curva_baixo_direita(race,2*64,10*64,32,11);
      break;
    case 11:
    if(race->x == 8*64){
        race->newlap = true;
        race->state = 1;
      }
      else reta_direita(race,8*64,11);
      break;
    case 1:
      //Lap Count
      if (race->newlap) {
        (race->lapnum)++;
        race->newlap = false;
      }
      reta_direita(race,12*64,12);
      break;
    case 12:
      curva_direita_cima(race,12*64,10*64,32,13);
      break;
    case 13:
      reta_cima(race,4*64,14);
      break;
    case 14:
      curva_cima_esquerda(race,12*64,4*64,32,15);
      break;
    case 15:
      reta_esquerda(race,11*64,16);
      break;
    case 16:
      curva_esquerda_baixo(race,11*64,4*64,32,17);
      break;
    case 17:
      reta_baixo(race,6*64,18);
      break;
    case 18:
      curva_baixo_esquerda(race,10*64,6*64,32,19);
      break;
    case 19:
      reta_esquerda(race,9*64,20);
      break;
    case 20:
      curva_esquerda_baixo(race,9*64,7*64,32,21);
      break;
    case 21:
      reta_baixo(race,8*64,22);
      break;
    case 22:
      curva_baixo_esquerda(race,8*64,8*64,32,23);
      break;
    case 23:
      reta_esquerda(race,6*64,24);
      break;
    case 24:
      curva_esquerda_cima(race,6*64,8*64,32,25);
      break;
    case 25:
      reta_cima(race,4*64,26);
      break;
    case 26:
      curva_cima_esquerda(race,5*64,4*64,32,27);
      break;
    case 27:
      reta_esquerda(race,2*64,28);
      break;
    case 28:
      curva_esquerda_baixo(race,2*64,4*64,32,29);
      break;
    case 29:
      reta_baixo(race,5*64,0);
      break;
    default:
      break;
  }
}

void runTrack4(CarPos *race) {
    //int delta_x, delta_y;
  switch (race->state) {
    case 0:
      //Lap Count
      if (race->newlap) {
        (race->lapnum)++;
        race->newlap = false;
      }
      reta_baixo(race,10*64,2);
      break;
    //Primeira curva
    case 2:
      curva_baixo_direita(race,2*64,10*64,32,3);
      break;
    case 3:
      reta_direita(race,3*64,4);
      break;
    case 4:
      curva_direita_cima(race,3*64,10*64,32,5);
      break;
    case 5:
      reta_cima(race,9*64,6);
      break;
    case 6:
      curva_cima_direita(race,4*64,9*64,32,7);
      break;
    case 7:
      reta_direita(race,5*64,8);
      break;
    case 8:
      curva_direita_cima(race,5*64,8*64,32,9);
      break;
    case 9:
      reta_cima(race,6*64,10);
      break;
    case 10:
      curva_cima_direita(race,6*64,6*64,32,11);
      break;
    case 11:
      reta_direita(race,9*64,12);
      break;
    case 12:
      curva_direita_baixo(race,9*64,6*64,32,13);
      break;
    case 13:
      reta_baixo(race,8*64,14);
      break;
    case 14:
      curva_baixo_esquerda(race,9*64,8*64,32,15);
      break;
    case 15:
      reta_esquerda(race,8*64,16);
      break;
    case 16:
      curva_esquerda_baixo(race,8*64,9*64,32,17);
      break;
    case 17:
      reta_baixo(race,10*64,18);
      break;
    case 18:
      curva_baixo_direita(race,8*64,10*64,32,19);
      break;
    case 19:
      reta_direita(race,12*64,20);
      break;
    case 20:
      curva_direita_cima(race,12*64,10*64,32,21);
      break;
    case 21:
      reta_cima(race,4*64,22);
      break;
    case 22:
      curva_cima_esquerda(race,12*64,4*64,32,23);
      break;
    case 23:
      reta_esquerda(race,2*64,24);
      break;
    case 24:
      curva_esquerda_baixo(race,2*64,4*64,32,25);
      break;
    case 25:
      if(race->y == 5*64) {
        race->newlap = true;
        race->state = 0;
      }
      else reta_baixo(race,5*64,25);
      break;
    default:
      break;
  }
}

void runTrack5(CarPos *race) {
  //int delta_x, delta_y;
  switch (race->state) {
    case 0:
      //Lap Count
      if (race->newlap) {
        (race->lapnum)++;
        race->newlap = false;
      }
      reta_baixo(race,10*64,2);
      break;
    //Primeira curva
    case 2:
      curva_baixo_direita(race,2*64,10*64,32,3);
      break;
    case 3:
      reta_direita(race,12*64,4);
      break;
    case 4:
      curva_direita_cima(race,12*64,10*64,32,5);
      break;
    case 5:
      reta_cima(race,4*64,6);
      break;
    case 6:
      curva_cima_esquerda(race,12*64,4*64,32,7);
      break;
    case 7:
      reta_esquerda(race,9*64,8);
      break;
    case 8:
      curva_esquerda_baixo(race,9*64,4*64,32,9);
      break;
    case 9:
      reta_baixo(race,8*64,10);
      break;
    case 10:
      curva_baixo_esquerda(race,8*64,8*64,32,11);
      break;
    case 11:
      reta_esquerda(race,4*64,12);
      break;
    case 12:
      curva_esquerda_cima(race,4*64,8*64,32,13);
      break;
    case 13:
      reta_cima(race,7*64,14);
      break;
    case 14:
      curva_cima_direita(race,4*64,7*64,32,15);
      break;
    case 15:
      reta_direita(race,6*64,16);
      break;
    case 16:
      curva_direita_cima(race,6*64,6*64,32,17);
      break;
    case 17:
      reta_cima(race,4*64,18);
      break;
    case 18:
      curva_cima_esquerda(race,6*64,4*64,32,19);
      break;
    case 19:
      reta_esquerda(race,2*64,20);
      break;
    case 20:
      curva_esquerda_baixo(race,2*64,4*64,32,21);
      break;
    case 21:
      if(race->y == 6*64) {
        race->newlap = true;
        race->state = 0;
      }
      else reta_baixo(race,6*64,21);
      break;
    default:
      break;
  }
}

void runTrack6(CarPos *race) {
  //int delta_x, delta_y;
  switch (race->state) {
    case 0:
      //Lap Count
      if (race->newlap) {
        (race->lapnum)++;
        race->newlap = false;
      }
      reta_baixo(race,10*64,2);
      break;
    //Primeira curva
    case 2:
      curva_baixo_direita(race,2*64,10*64,32,3);
      break;
    case 3:
      reta_direita(race,3*64,4);
      break;
    case 4:
      curva_direita_cima(race,3*64,10*64,32,5);
      break;
    case 5:
      reta_cima(race,6*64,6);
      break;
    case 6:
      curva_cima_direita(race,4*64,6*64,32,7);
      break;
    case 7:
      reta_direita(race,5*64,8);
      break;
    case 8:
      curva_direita_baixo(race,5*64,6*64,32,9);
      break;
    case 9:
      reta_baixo(race,10*64,10);
      break;
    case 10:
      curva_baixo_direita(race,6*64,10*64,32,11);
      break;
    case 11:
      reta_direita(race,7*64,12);
      break;
    case 12:
      curva_direita_cima(race,7*64,10*64,32,13);
      break;
    case 13:
      reta_cima(race,6*64,14);
      break;
    case 14:
      curva_cima_direita(race,8*64,6*64,32,15);
      break;
    case 15:
      reta_direita(race,9*64,16);
      break;
    case 16:
      curva_direita_baixo(race,9*64,6*64,32,17);
      break;
    case 17:
      reta_baixo(race,10*64,18);
      break;
    case 18:
      curva_baixo_direita(race,10*64,10*64,32,19);
      break;
    case 19:
      reta_direita(race,11*64,20);
      break;
    case 20:
      curva_direita_cima(race,11*64,10*64,32,21);
      break;
    case 21:
      reta_cima(race,4*64,22);
      break;
    case 22:
      curva_cima_esquerda(race,11*64,4*64,32,23);
      break;
    case 23:
      reta_esquerda(race,2*64,24);
      break;
    case 24:
      curva_esquerda_baixo(race,2*64,4*64,32,25);
      break;
    case 25:
      if(race->y == 6*64) {
        race->newlap = true;
        race->state = 0;
      }
      else reta_baixo(race,6*64,25);
      break;
    default:
      break;
  }
}


void runTrack7(CarPos *race) {
  //int delta_x, delta_y;
  switch (race->state) {
    case 0:
      //Lap Count
      if (race->newlap) {
        (race->lapnum)++;
        race->newlap = false;
      }
      reta_baixo(race,10*64,2);
      break;
    //Primeira curva
    case 2:
      curva_baixo_direita(race,2*64,10*64,32,3);
      break;
    case 3:
      reta_direita(race,11*64,4);
      break;
    case 4:
      curva_direita_cima(race,11*64,10*64,32,5);
      break;
    case 5:
      reta_cima(race,9*64,6);
      break;
    case 6:
      curva_cima_esquerda(race,11*64,9*64,32,7);
      break;
    case 7:
      reta_esquerda(race,5*64,8);
      break;
    case 8:
      curva_esquerda_cima(race,5*64,8*64,32,9);
      break;
    case 9:
      reta_cima(race,6*64,10);
      break;
    case 10:
      curva_cima_direita(race,5*64,6*64,32,11);
      break;
    case 11:
      reta_direita(race,11*64,12);
      break;
    case 12:
      curva_direita_cima(race,11*64,5*64,32,13);
      break;
    case 13:
      reta_cima(race,4*64,14);
      break;
    case 14:
      curva_cima_esquerda(race,11*64,4*64,32,15);
      break;
    case 15:
      reta_esquerda(race,2*64,16);
      break;
    case 16:
      curva_esquerda_baixo(race,2*64,4*64,32,17);
      break;
    case 17:
      if(race->y == 5*64) {
        race->newlap = true;
        race->state = 0;
      }
      else reta_baixo(race,5*64,17);
      break;
    default:
      break;
  }
}

void runTrack8(CarPos *race) {
  //int delta_x, delta_y;
  switch (race->state) {
    case 0:
      //Lap Count
      if (race->newlap) {
        (race->lapnum)++;
        race->newlap = false;
      }
      reta_baixo(race,8*64,2);
      break;
    //Primeira curva
    case 2:
      curva_baixo_direita(race,2*64,8*64,32,3);
      break;
    case 3:
      reta_direita(race,4*64,4);
      break;
    case 4:
      curva_direita_baixo(race,4*64,9*64,32,5);
      break;
    case 5:
      reta_baixo(race,10*64,6);
      break;
    case 6:
      curva_baixo_direita(race,5*64,10*64,32,7);
      break;
    case 7:
      reta_direita(race,6*64,8);
      break;
    case 8:
      curva_direita_cima(race,6*64,10*64,32,9);
      break;
    case 9:
      reta_cima(race,9*64,10);
      break;
    case 10:
      curva_cima_direita(race,7*64,9*64,32,11);
      break;
    case 11:
      reta_direita(race,8*64,12);
      break;
    case 12:
      curva_direita_baixo(race,8*64,9*64,32,13);
      break;
    case 13:
      reta_baixo(race,10*64,14);
      break;
    case 14:
      curva_baixo_direita(race,9*64,10*64,32,15);
      break;
    case 15:
      reta_direita(race,12*64,16);
      break;
    case 16:
      curva_direita_cima(race,12*64,10*64,32,17);
      break;
    case 17:
      reta_cima(race,9*64,18);
      break;
    case 18:
      curva_cima_esquerda(race,12*64,9*64,32,19);
      break;
    case 19:
      reta_esquerda(race,11*64,20);
      break;
    case 20:
      curva_esquerda_cima(race,11*64,8*64,32,21);
      break;
    case 21:
      reta_cima(race,6*64,22);
      break;
    case 22:
      curva_cima_direita(race,11*64,6*64,32,23);
      break;
    case 23:
      reta_direita(race,12*64,24);
      break;
    case 24:
      curva_direita_cima(race,12*64,5*64,32,25);
      break;
    case 25:
      reta_cima(race,4*64,26);
      break;
    case 26:
      curva_cima_esquerda(race,12*64,4*64,32,27);
      break;
    case 27:
      reta_esquerda(race,9*64,28);
      break;
    case 28:
      curva_esquerda_baixo(race,9*64,4*64,32,29);
      break;
    case 29:
      reta_baixo(race,7*64,30);
      break;
    case 30:
      curva_baixo_esquerda(race,8*64,7*64,32,31);
      break;
    case 31:
      reta_esquerda(race,7*64,32);
      break;
    case 32:
      curva_esquerda_cima(race,7*64,7*64,32,33);
      break;
    case 33:
      reta_cima(race,6*64,34);
      break;
    case 34:
      curva_cima_esquerda(race,6*64,6*64,32,35);
      break;
    case 35:
      reta_esquerda(race,5*64,36);
      break;
    case 36:
      curva_esquerda_cima(race,5*64,5*64,32,37);
      break;
    case 37:
      reta_cima(race,4*64,38);
      break;
    case 38:
      curva_cima_esquerda(race,4*64,4*64,32,39);
      break;
    case 39:
      reta_esquerda(race,2*64,40);
      break;
    case 40:
      curva_esquerda_baixo(race,2*64,4*64,32,41);
      break;
    case 41:
      if(race->y == 5*64) {
        race->newlap = true;
        race->state = 0;
      }
      else reta_baixo(race,5*64,41);
      break;
    default:
      break;
  }
}

RaceInfo (newRace1)(xpm_image_t img){
  RaceInfo race;
  //Car Position
  race.position.x = 1.5*64;
  race.position.y = 6*64;
  race.position.angle = 360;
  race.position.speed = 0;
  race.position.lapnum = 0;
  race.position.state = 0;
  race.position.newlap = true;

  //Race Information
  race.combo = 0;
  race.nitro = false;
  race.nitroh = 0;
  race.key = 0;
  race.countdown = 0;
  race.start = false;
  race.time = 0;
  race.totalLaps = 3;

  race.track = img;

  race.dinamicImg = img;
  race.runTrack = runTrack1;
  return race;
}

RaceInfo (newRace2)(xpm_image_t img){
  RaceInfo race;
  //Car Position
  race.position.x = 8*64;
  race.position.y = 10.5*64;
  race.position.angle = 270;
  race.position.speed = 0;
  race.position.lapnum = 0;
  race.position.state = 1;
  race.position.newlap = true;

  //Race Information
  race.combo = 0;
  race.nitro = false;
  race.nitroh = 0;
  race.key = 0;
  race.countdown = 0;
  race.start = false;
  race.time = 0;
  race.totalLaps = 5;

  race.track = img;

  race.dinamicImg = img;
  race.runTrack = runTrack2;
  return race;
}

RaceInfo (newRace3)(xpm_image_t img){
  RaceInfo race;
  //Car Position
  race.position.x = 8*64;
  race.position.y = 10.5*64;
  race.position.angle = 270;
  race.position.speed = 0;
  race.position.lapnum = 0;
  race.position.state = 1;
  race.position.newlap = true;

  //Race Information
  race.combo = 0;
  race.nitro = false;
  race.nitroh = 0;
  race.key = 0;
  race.countdown = 0;
  race.start = false;
  race.time = 0;
  race.totalLaps = 7;

  race.track = img;

  race.dinamicImg = img;
  race.runTrack = runTrack3;
  return race;
}

RaceInfo (newRace4)(xpm_image_t img){
  RaceInfo race;
  //Car Position
  race.position.x = 1.5*64;
  race.position.y = 5*64;
  race.position.angle = 360;
  race.position.speed = 0;
  race.position.lapnum = 0;
  race.position.state = 0;
  race.position.newlap = true;

  //Race Information
  race.combo = 0;
  race.nitro = false;
  race.nitroh = 0;
  race.key = 0;
  race.countdown = 0;
  race.start = false;
  race.time = 0;
  race.totalLaps = 9;

  race.track = img;

  race.dinamicImg = img;
  race.runTrack = runTrack4;
  return race;
}

RaceInfo (newRace5)(xpm_image_t img){
  RaceInfo race;
  //Car Position
  race.position.x = 1.5*64;
  race.position.y = 6*64;
  race.position.angle = 360;
  race.position.speed = 0;
  race.position.lapnum = 0;
  race.position.state = 0;
  race.position.newlap = true;

  //Race Information
  race.combo = 0;
  race.nitro = false;
  race.nitroh = 0;
  race.key = 0;
  race.countdown = 0;
  race.start = false;
  race.time = 0;
  race.totalLaps = 9;

  race.track = img;

  race.dinamicImg = img;
  race.runTrack = runTrack5;
  return race;
}

RaceInfo (newRace6)(xpm_image_t img){
  RaceInfo race;
  //Car Position
  race.position.x = 1.5*64;
  race.position.y = 6*64;
  race.position.angle = 360;
  race.position.speed = 0;
  race.position.lapnum = 0;
  race.position.state = 0;
  race.position.newlap = true;

  //Race Information
  race.combo = 0;
  race.nitro = false;
  race.nitroh = 0;
  race.key = 0;
  race.countdown = 0;
  race.start = false;
  race.time = 0;
  race.totalLaps = 10;

  race.track = img;

  race.dinamicImg = img;
  race.runTrack = runTrack6;
  return race;
}

RaceInfo (newRace7)(xpm_image_t img){
  RaceInfo race;
  //Car Position
  race.position.x = 1.5*64;
  race.position.y = 5*64;
  race.position.angle = 360;
  race.position.speed = 0;
  race.position.lapnum = 0;
  race.position.state = 0;
  race.position.newlap = true;

  //Race Information
  race.combo = 0;
  race.nitro = false;
  race.nitroh = 0;
  race.key = 0;
  race.countdown = 0;
  race.start = false;
  race.time = 0;
  race.totalLaps = 11;

  race.track = img;

  race.dinamicImg = img;
  race.runTrack = runTrack7;
  return race;
}

RaceInfo (newRace8)(xpm_image_t img){
  RaceInfo race;
  //Car Position
  race.position.x = 1.5*64;
  race.position.y = 5*64;
  race.position.angle = 360;
  race.position.speed = 0;
  race.position.lapnum = 0;
  race.position.state = 0;
  race.position.newlap = true;

  //Race Information
  race.combo = 0;
  race.nitro = false;
  race.nitroh = 0;
  race.key = 0;
  race.countdown = 0;
  race.start = false;
  race.time = 0;
  race.totalLaps = 14;

  race.track = img;

  race.dinamicImg = img;
  race.runTrack = runTrack8;
  return race;
}
