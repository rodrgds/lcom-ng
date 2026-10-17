#include "frog.h"

void initFrog(Frog * frog){
  xpm_image_t img;
  xpm_load(frog1_xpm,XPM_8_8_8_8,&img);
  frog->img[0]=img;
  xpm_image_t img2;
  xpm_load(frog2_xpm,XPM_8_8_8_8,&img2);
  frog->img[1]=img;
  xpm_image_t img3;
  xpm_load(frog3_xpm,XPM_8_8_8_8,&img3);
  frog->img[2]=img3;
  xpm_image_t img4;
  xpm_load(frog4_xpm,XPM_8_8_8_8,&img4);
  frog->img[3]=img4;
  xpm_image_t img5;
  xpm_load(frog5_xpm,XPM_8_8_8_8,&img5);
  frog->img[4]=img5;
  xpm_image_t img6;
  xpm_load(frog6_xpm,XPM_8_8_8_8,&img6);
  frog->img[5]=img6;
  xpm_image_t img7;
  xpm_load(frog7_xpm,XPM_8_8_8_8,&img7);
  frog->img[6]=img7;
  frog->x=540;
  frog->y=775;
  frog->animationIndex=0;
  
}


void resetFrog(Frog * frog){
  frog->x=540;
  frog->y=775;
}

bool canFrogMove(Frog frog, int move,int size){
  if (move==0){
    if (frog.x+size>xRes-frog.img[0].width){
      return false;
    }
  }
  else{
    if (frog.x-size<0){
      return false;
    }
  }
  
  return true;
}


void frogAnimation(Frog * frog, bool * frogIdle){
  if (frog->animationIndex==7){
    frog->animationIndex=0;
    *frogIdle=true;
  }
  else{
    frog->animationIndex+=1;
  }
}
