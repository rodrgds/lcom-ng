#include "images.h"
#include "graphics.h"
#include "images/car.xpm"
#include "images/locked.xpm"
#include "images/track1.xpm"
#include "images/track2.xpm"
#include "images/track3.xpm"
#include "images/track4.xpm"
#include "images/track5.xpm"
#include "images/track6.xpm"
#include "images/track7.xpm"
#include "images/track8.xpm"
#include "images/menu.xpm"
#include "images/highscores.xpm"
#include "images/levelselect.xpm"
#include "images/fire.xpm"
#include "images/combo.xpm"
#include "images/nitro.xpm"
#include "images/fuel.xpm"
#include "images/nitrofull.xpm"
#include "images/endgame.xpm"
#include "images/arrow.xpm"
#include "images/font/twodots.xpm"
#include "images/font/slash.xpm"
#include "images/font/Number0.xpm"
#include "images/font/Number1.xpm"
#include "images/font/Number2.xpm"
#include "images/font/Number3.xpm"
#include "images/font/Number4.xpm"
#include "images/font/Number5.xpm"
#include "images/font/Number6.xpm"
#include "images/font/Number7.xpm"
#include "images/font/Number8.xpm"
#include "images/font/Number9.xpm"
#include "images/font/LetterA.xpm"
#include "images/font/LetterB.xpm"
#include "images/font/LetterC.xpm"
#include "images/font/LetterD.xpm"
#include "images/font/LetterE.xpm"
#include "images/font/LetterF.xpm"
#include "images/font/LetterG.xpm"
#include "images/font/LetterH.xpm"
#include "images/font/LetterI.xpm"
#include "images/font/LetterJ.xpm"
#include "images/font/LetterK.xpm"
#include "images/font/LetterL.xpm"
#include "images/font/LetterM.xpm"
#include "images/font/LetterN.xpm"
#include "images/font/LetterO.xpm"
#include "images/font/LetterP.xpm"
#include "images/font/LetterQ.xpm"
#include "images/font/LetterR.xpm"
#include "images/font/LetterS.xpm"
#include "images/font/LetterT.xpm"
#include "images/font/LetterU.xpm"
#include "images/font/LetterV.xpm"
#include "images/font/LetterW.xpm"
#include "images/font/LetterX.xpm"
#include "images/font/LetterY.xpm"
#include "images/font/LetterZ.xpm"

ImageLibrary* newImageLibrary(ImageLibrary* tmp) {
  xpm_image_t temp_img;
  enum xpm_image_type type = XPM_5_6_5;

  //Menu
  xpm_load(menu1, type, &temp_img);
  tmp->menu = temp_img;

  xpm_load(highscoresmenu_xpm, type, &temp_img);
  tmp->menu_highscores = temp_img;

  xpm_load(levelselect_xpm, type, &temp_img);
  tmp->menu_LevelSelect = temp_img;

  xpm_load(endgame_xpm, type, &temp_img);
  tmp->endGame = temp_img;

  xpm_load(locked_xpm, type, &temp_img);
  tmp->locked = temp_img;

  //Mouse
  xpm_load(arrow_xpm, type, &temp_img);
  tmp->arrow = temp_img;


  //Cars
  xpm_load(car, type, &temp_img);
  tmp->blueCar = temp_img;

  //Track
  xpm_load(track1_xpm, type, &temp_img);
  tmp->track1 = temp_img;

  xpm_load(track2_xpm, type, &temp_img);
  tmp->track2 = temp_img;

  xpm_load(track3_xpm, type, &temp_img);
  tmp->track3 = temp_img;

  xpm_load(track4_xpm, type, &temp_img);
  tmp->track4 = temp_img;

  xpm_load(track5_xpm, type, &temp_img);
  tmp->track5 = temp_img;

  xpm_load(track6_xpm, type, &temp_img);
  tmp->track6 = temp_img;

  xpm_load(track7_xpm, type, &temp_img);
  tmp->track7 = temp_img;

  xpm_load(track8_xpm, type, &temp_img);
  tmp->track8 = temp_img;

  //Nitro Bar
  xpm_load(combo_xpm, type, &temp_img);
  tmp->nitroAvailable = temp_img;

  xpm_load(fire_xpm, type, &temp_img);
  tmp->nitroActive = temp_img;

  xpm_load(nitroFull_xpm, type, &temp_img);
  tmp->nitroFullBar = temp_img;

  xpm_load(nitroBar_xpm, type, &temp_img);
  tmp->nitroBar = temp_img;

  xpm_load(fuel_xpm, type, &temp_img);
  tmp->fuel = temp_img;

  //letters
xpm_load(letterA_xpm, type, &temp_img);
  tmp->letterA = temp_img;

xpm_load(letterB_xpm, type, &temp_img);
  tmp->letterB = temp_img;

xpm_load(letterC_xpm, type, &temp_img);
  tmp->letterC = temp_img;

xpm_load(letterD_xpm, type, &temp_img);
  tmp->letterD = temp_img;

xpm_load(letterE_xpm, type, &temp_img);
  tmp->letterE = temp_img;

xpm_load(letterF_xpm, type, &temp_img);
  tmp->letterF = temp_img;

xpm_load(letterG_xpm, type, &temp_img);
  tmp->letterG = temp_img;

xpm_load(letterH_xpm, type, &temp_img);
  tmp->letterH = temp_img;

xpm_load(letterI_xpm, type, &temp_img);
  tmp->letterI = temp_img;

xpm_load(letterJ_xpm, type, &temp_img);
  tmp->letterJ = temp_img;

xpm_load(letterK_xpm, type, &temp_img);
  tmp->letterK = temp_img;

xpm_load(letterL_xpm, type, &temp_img);
  tmp->letterL = temp_img;

xpm_load(letterM_xpm, type, &temp_img);
  tmp->letterM = temp_img;

xpm_load(letterN_xpm, type, &temp_img);
  tmp->letterN = temp_img;

xpm_load(letterO_xpm, type, &temp_img);
  tmp->letterO = temp_img;

xpm_load(letterP_xpm, type, &temp_img);
  tmp->letterP = temp_img;

xpm_load(letterQ_xpm, type, &temp_img);
  tmp->letterQ = temp_img;

xpm_load(letterR_xpm, type, &temp_img);
  tmp->letterR = temp_img;

xpm_load(letterS_xpm, type, &temp_img);
  tmp->letterS = temp_img;

xpm_load(letterT_xpm, type, &temp_img);
  tmp->letterT = temp_img;

xpm_load(letterU_xpm, type, &temp_img);
  tmp->letterU = temp_img;

xpm_load(letterV_xpm, type, &temp_img);
  tmp->letterV = temp_img;

xpm_load(letterW_xpm, type, &temp_img);
  tmp->letterW = temp_img;

xpm_load(letterX_xpm, type, &temp_img);
  tmp->letterX = temp_img;

xpm_load(letterY_xpm, type, &temp_img);
  tmp->letterY = temp_img;

xpm_load(letterZ_xpm, type, &temp_img);
  tmp->letterZ = temp_img;


  //Numbers
xpm_load(Number0_xpm, type, &temp_img);
  tmp->number0 = temp_img;

xpm_load(Number1_xpm, type, &temp_img);
  tmp->number1 = temp_img;

xpm_load(Number2_xpm, type, &temp_img);
  tmp->number2 = temp_img;

xpm_load(Number3_xpm, type, &temp_img);
  tmp->number3 = temp_img;

xpm_load(Number4_xpm, type, &temp_img);
  tmp->number4 = temp_img;

xpm_load(Number5_xpm, type, &temp_img);
  tmp->number5 = temp_img;

xpm_load(Number6_xpm, type, &temp_img);
  tmp->number6 = temp_img;

xpm_load(Number7_xpm, type, &temp_img);
  tmp->number7 = temp_img;

xpm_load(Number8_xpm, type, &temp_img);
  tmp->number8 = temp_img;

xpm_load(Number9_xpm, type, &temp_img);
  tmp->number9 = temp_img;

//Punctuation
xpm_load(twodots_xpm, type, &temp_img);
  tmp->twoDots = temp_img;

xpm_load(slash_xpm, type, &temp_img);
  tmp->slash = temp_img;

return tmp;
}

xpm_image_t numberToDraw(ImageLibrary* self, int n){
  xpm_image_t number;
  switch(n) {
    case 0:
      number = self->number0;
      break;
    case 1:
      number = self->number1;
      break;
    case 2:
      number = self->number2;
      break;
    case 3:
      number = self->number3;
      break;
    case 4:
      number = self->number4;
      break;
    case 5:
      number = self->number5;
      break;
    case 6:
      number = self->number6;
      break;
    case 7:
      number = self->number7;
      break;
    case 8:
      number = self->number8;
      break;
    case 9:
      number = self->number9;
      break;
    default:
      break;
  }
  return number;
}

xpm_image_t charToDraw(ImageLibrary* self, char c) {
  xpm_image_t letter;
  switch(c) {
    case 'a':
      letter = self->letterA;
      break;
    case 'b':
      letter = self->letterB;
      break;
    case 'c':
      letter = self->letterC;
      break;
    case 'd':
      letter = self->letterD;
      break;
    case 'e':
      letter = self->letterE;
      break;
    case 'f':
      letter = self->letterF;
      break;
    case 'g':
      letter = self->letterG;
      break;
    case 'h':
      letter = self->letterH;
      break;
    case 'i':
      letter = self->letterI;
      break;
    case 'j':
      letter = self->letterJ;
      break;
    case 'k':
      letter = self->letterK;
      break;
    case 'l':
      letter = self->letterL;
      break;
    case 'm':
      letter = self->letterM;
      break;
    case 'n':
      letter = self->letterN;
      break;
    case 'o':
      letter = self->letterO;
      break;
    case 'p':
      letter = self->letterP;
      break;
    case 'q':
      letter = self->letterQ;
      break;
    case 'r':
      letter = self->letterR;
      break;
    case 's':
      letter = self->letterS;
      break;
    case 't':
      letter = self->letterT;
      break;
    case 'u':
      letter = self->letterU;
      break;
    case 'v':
      letter = self->letterV;
      break;
    case 'w':
      letter = self->letterW;
      break;
    case 'x':
      letter = self->letterX;
      break;
    case 'y':
      letter = self->letterY;
      break;
    case 'z':
      letter = self->letterZ;
      break;
    default:
      break;
  }
  return letter;
}

void drawNumber(ImageLibrary *image, int x, int y, int value, double size) {
  if (value == 0) {
    xpm_image_t img = numberToDraw(image, 0);
    draw_xpm_resize(x, y, img, 0, size);
    return;
  }

  int i = value;
  unsigned int sizeN = 1;
  while (i /= 10)
    sizeN++;

  int num = value;
  char arr[sizeN];
  int k = 0;
  while(num) {
   arr[k] = num % 10;
   k++;
   num /= 10;
  }

  int move = 0;
  for(size_t j = 0; j < sizeN; j++) {
    xpm_image_t img = numberToDraw(image, arr[sizeN-1-j]);
    draw_xpm_resize(x+move, y, img, 0, size);
    move += 128*size;
  }
}
