#include "bitmap.h"
#include "macros.h"
#include "textRender.h"
#include <string.h>

static char* bmpPathInvalidChar = "/home/rush/res/0.bmp";
static char* bmpPath0 = "/home/rush/res/0.bmp";
static char* bmpPath1 = "/home/rush/res/1.bmp";
static char* bmpPath2 = "/home/rush/res/2.bmp";
static char* bmpPath3 = "/home/rush/res/3.bmp";
static char* bmpPath4 = "/home/rush/res/4.bmp";
static char* bmpPath5 = "/home/rush/res/5.bmp";
static char* bmpPath6 = "/home/rush/res/6.bmp";
static char* bmpPath7 = "/home/rush/res/7.bmp";
static char* bmpPath8 = "/home/rush/res/8.bmp";
static char* bmpPath9 = "/home/rush/res/9.bmp";

void renderNumber(ObjLayer* objs, unsigned int nbr, int x, int y, int align){
  int runNbr = nbr;
  int widthCount = 0;
  if(runNbr == 0) zIndexPush(objs, 5, createGameObject("numberedText", x, y, 0, 0, getBmpPathFromNbr(0), MAGENTA));
  while(runNbr > 0){
    int unit = runNbr % 10;
    runNbr = runNbr / 10;
    GameObject* g;
    if(align == ALIGN_LEFT){
      g = zIndexPush(objs, 5, createGameObject("numberedText", x + widthCount, y, 0, 0, getBmpPathFromNbr(unit), MAGENTA));
    }else if(align == ALIGN_RIGHT){
      g = zIndexPush(objs, 5, createGameObject("numberedText", x - widthCount, y, 0, 0, getBmpPathFromNbr(unit), MAGENTA));
    }else return;
    widthCount += g->sizeX;

  }
}

void clearText(ObjLayer* objs){
  int i;
  for(i = 0; i < objs[5].size; i++){
    if(strcmp(objs[5].gameObjects[i].label, "numberedText") == 0){
      zIndexRemove(objs, &objs[5].gameObjects[i]);
    }
  }
}

char* getBmpPathFromNbr(int src){
  switch(src){
    case 0:
      return bmpPath0;
    case 1:
      return bmpPath1;
    case 2:
      return bmpPath2;
    case 3:
      return bmpPath3;
    case 4:
      return bmpPath4;
    case 5:
      return bmpPath5;
    case 6:
      return bmpPath6;
    case 7:
      return bmpPath7;
    case 8:
      return bmpPath8;
    case 9:
      return bmpPath9;
    default:
      return bmpPathInvalidChar;
  }
}
