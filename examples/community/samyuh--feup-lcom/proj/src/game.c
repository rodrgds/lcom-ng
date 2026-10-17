#include "game.h"
#include "rtc.h"
#include "graphics.h"

CarInfo (createCar)(xpm_image_t img) {
  CarInfo car;
  car.maxSpeed = 12;
  car.img = img;
  return car;
}

GameInfo (initGame)(){
  //Load images
  ImageLibrary * image = (ImageLibrary*) malloc(sizeof(ImageLibrary));
  newImageLibrary(image);

  //Car Construtor
  CarInfo car = createCar(image->blueCar);
  GameInfo game;

  MouseInfo * mouse_player =  (MouseInfo *) malloc(sizeof(MouseInfo));
  mouse_player->x = 520;
  mouse_player->y = 420;

  Date null;
  null.sec = 0;
  null.min = 0;
  null.hour = 0;
  null.day = 0;
  null.month = 0;
  null.year = 0;

  game.highscoreDate[0] = null;
  game.highscoreDate[1] = null;
  game.highscoreDate[2] = null;
  game.highscoreDate[3] = null;
  game.highscoreDate[4] = null;
  game.highscoreDate[5] = null;
  game.highscoreDate[6] = null;
  game.highscoreDate[7] = null;


  game.mousePlayer = mouse_player;

  game.state = MAIN_MENU;
  game.car = car;
  game.image = image;
  game.racenum = -1;
  game.highscore[0] = -1;
  game.highscore[1] = -1;
  game.highscore[2] = -1;
  game.highscore[3] = -1;
  game.highscore[4] = -1;
  game.highscore[5] = -1;
  game.highscore[6] = -1;
  game.highscore[7] = -1;

  game.targetTime[0] = 13;
  game.targetTime[1] = 18;
  game.targetTime[2] = 25;
  game.targetTime[3] = 30;
  game.targetTime[4] = 30;
  game.targetTime[5] = 30;
  game.targetTime[6] = 30;
  game.targetTime[7] = 36;

  game.highestLevel = 0;
  return game;
}

void numberRace(GameInfo * self, int num){
  switch (num) {
    case 0:
      self->race = newRace1(self->image->track1);
      break;
    case 1:
      self->race = newRace2(self->image->track2);
            break;
    case 2:
      self->race = newRace3(self->image->track3);
            break;
    case 3:
      self->race = newRace4(self->image->track4);
            break;
    case 4:
      self->race = newRace5(self->image->track5);
            break;
    case 5:
      self->race = newRace6(self->image->track6);
            break;
    case 6:
      self->race = newRace7(self->image->track7);
            break;
    case 7:
      self->race = newRace8(self->image->track8);
            break;
    default:
      break;
  }
}

void repeatRace(GameInfo * self) {
  switch (self->racenum) {
    case 0:
      self->race = newRace1(self->image->track1);
      break;
    case 1:
      self->race = newRace2(self->image->track2);
            break;
    case 2:
      self->race = newRace3(self->image->track3);
            break;
    case 3:
      self->race = newRace4(self->image->track4);
            break;
    case 4:
      self->race = newRace5(self->image->track5);
            break;
    case 5:
      self->race = newRace6(self->image->track6);
            break;
    case 6:
      self->race = newRace7(self->image->track7);
            break;
    case 7:
      self->race = newRace8(self->image->track8);
            break;
    default:
      break;
  }
}

void nextRace(GameInfo * self) {
  self->racenum++;
  switch (self->racenum) {
    case 0:
      self->race = newRace1(self->image->track1);
      break;
    case 1:
      self->race = newRace2(self->image->track2);
            break;
    case 2:
      self->race = newRace3(self->image->track3);
            break;
    case 3:
      self->race = newRace4(self->image->track4);
            break;
    case 4:
      self->race = newRace5(self->image->track5);
            break;
    case 5:
      self->race = newRace6(self->image->track6);
            break;
    case 6:
      self->race = newRace7(self->image->track7);
            break;
    case 7:
      self->race = newRace8(self->image->track8);
            break;
    default:
      self->racenum--;
      break;
  }
}

void setHighscore(GameInfo * self, int racenum) {
  uint32_t day, month, year, hour, min, sec;
  isUpdating();
  getDate(&day, &month, &year, &hour, &min, &sec);
  self->highscoreDate[racenum].sec = sec;
  self->highscoreDate[racenum].min = min;
  self->highscoreDate[racenum].hour = hour;
  self->highscoreDate[racenum].day = day;
  self->highscoreDate[racenum].month = month;
  self->highscoreDate[racenum].year = year;
}

void exitGame(GameInfo* self){
  free(self->image);
  free(self->mousePlayer);
  freeBuffering();
}
