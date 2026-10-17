#include "draws.h"
 struct Background background;
 struct Bar bar;
 struct Pause pauseWindow;
 struct Menu menu;
 struct Help help;

void loadMap(){
  background.x=0;
  background.y=0;
  xpm_image_t img;
  xpm_load(map_xpm,XPM_8_8_8_8,&img);
  background.img=img;
  xpm_image_t img16;
  xpm_load(zero_xpm,XPM_8_8_8_8,&img16);
  bar.img[0]=img16;
  xpm_image_t img2;
  xpm_load(one_xpm,XPM_8_8_8_8,&img2);
  bar.img[1]=img2;
  xpm_image_t img3;
  xpm_load(two_xpm,XPM_8_8_8_8,&img3);
  bar.img[2]=img3;
  xpm_image_t img4;
  xpm_load(three_xpm,XPM_8_8_8_8,&img4);
  bar.img[3]=img4;
  xpm_image_t img5;
  xpm_load(four_xpm,XPM_8_8_8_8,&img5);
  bar.img[4]=img5;
  xpm_image_t img6;
  xpm_load(five_xpm,XPM_8_8_8_8,&img6);
  bar.img[5]=img6;
  xpm_image_t img7;
  xpm_load(six_xpm,XPM_8_8_8_8,&img7);
  bar.img[6]=img7;
  xpm_image_t img8;
  xpm_load(seven_xpm,XPM_8_8_8_8,&img8);
  bar.img[7]=img8;
  xpm_image_t img9;
  xpm_load(eight_xpm,XPM_8_8_8_8,&img9);
  bar.img[8]=img9;
  xpm_image_t img10;
  xpm_load(nine_xpm,XPM_8_8_8_8,&img10);
  bar.img[9]=img10;
  xpm_image_t img11;
  xpm_load(ten_xpm,XPM_8_8_8_8,&img11);
  bar.img[10]=img11;
  xpm_image_t img12;
  xpm_load(bar_xpm,XPM_8_8_8_8,&img12);
  bar.img[11]=img12;
  xpm_image_t img13;
  xpm_load(lives_xpm,XPM_8_8_8_8,&img13);
  bar.img[12]=img13;
  xpm_image_t img14;
  xpm_load(pause_xpm,XPM_8_8_8_8,&img14);
  pauseWindow.img=img14;
  xpm_image_t img15;
  xpm_load(optionBar_xpm,XPM_8_8_8_8,&img15);
  pauseWindow.img2=img15;
  xpm_image_t img17;
  xpm_load(points_xpm,XPM_8_8_8_8,&img17);
  bar.img[13]=img17;
  xpm_image_t img18;
  xpm_load(time_xpm,XPM_8_8_8_8,&img18);
  bar.img[14]=img18;
  pauseWindow.x=0;
  pauseWindow.y=0;
  xpm_image_t img19;
  xpm_load(menu_xpm,XPM_8_8_8_8,&img19);
  menu.img=img19;
  menu.x=0;
  menu.y=0;
  xpm_image_t img20;
  xpm_load(help_xpm,XPM_8_8_8_8,&img20);
  help.img=img20;
  help.x=0;
  help.y=0;
  drawMenu();
  
  
}

void drawMenu(){
  uint32_t * map = (uint32_t *) menu.img.bytes;
  draw(menu.x,menu.y,menu.img.width,menu.img.height,map);

}

void drawHelp(){
  uint32_t * map = (uint32_t *) help.img.bytes;
  draw(help.x,help.y,help.img.width,help.img.height,map);
  vg_copy();
}


void drawMouse(Mouse mouse){
  if (!mouse.hover){
    uint32_t * map = (uint32_t *) mouse.img1.bytes;
    draw(mouse.x,mouse.y,mouse.img1.width,mouse.img1.height,map);
  }
  else{
    uint32_t * map = (uint32_t *) mouse.img2.bytes;
    draw(mouse.x,mouse.y,mouse.img2.width,mouse.img2.height,map);
  }
  vg_copy();
  
}

void drawTime(int time[2]){
  int minMin = time[1]%10;
  int maxMin = time[1]/10;
  int minHour = time[0]%10;
  int maxHour = time[0]/10;
  

  uint32_t * map = (uint32_t *) bar.img[14].bytes;
  draw(480,824,bar.img[14].width,bar.img[14].height,map);
  map = (uint32_t *) bar.img[maxHour].bytes;
  draw(705,826,bar.img[maxHour].width,bar.img[maxHour].height,map);
  map = (uint32_t *) bar.img[minHour].bytes;
  draw(725,826,bar.img[minHour].width,bar.img[minHour].height,map);
  map = (uint32_t *) bar.img[13].bytes;
  draw(730,829,bar.img[13].width,bar.img[13].height,map);
  map = (uint32_t *) bar.img[maxMin].bytes;
  draw(760,826,bar.img[maxMin].width,bar.img[maxMin].height,map);
  map = (uint32_t *) bar.img[minMin].bytes;
  draw(780,826,bar.img[minMin].width,bar.img[minMin].height,map);

}

void drawBar(int level, int lives, int time[2]){
  uint32_t * map = (uint32_t *) bar.img[11].bytes;
  draw(0,824,bar.img[11].width,bar.img[11].height,map);
  map = (uint32_t *) bar.img[level].bytes;
  draw(1040,822,bar.img[level].width,bar.img[level].height,map);
  for (int i=0;i<lives;i++){
    map = (uint32_t *) bar.img[12].bytes;
    draw(145+(i*(bar.img[12].width+5)),822,bar.img[12].width,bar.img[12].height,map);
  }
  if (time[0]!=32){
    drawTime(time);
  }

}

void drawPause(){
  uint32_t * map = (uint32_t*) pauseWindow.img.bytes;
  draw(pauseWindow.x,pauseWindow.y,pauseWindow.img.width,pauseWindow.img.height,map);
  
  
}

void drawPauseOption(bool option){
  
  drawPause();
  uint32_t * map = (uint32_t*) pauseWindow.img2.bytes;
  if (option){

    draw(287,216,pauseWindow.img2.width,pauseWindow.img2.height,map);
  }
  else{
    draw(287,450,pauseWindow.img2.width,pauseWindow.img2.height,map);
  }

  
}

int (drawBackground(bool day)){
  uint32_t * map = (uint32_t*) background.img.bytes;
  draw(background.x,background.y,background.img.width,background.img.height,map);
  if (!day){
    vg_draw_rectangle(0,98,xRes,302,0x202A44);
  }
  return 0;
}

void (draw(int x, int y, int img_width, int img_height, uint32_t * map)){
  for (int width = 0 ; width < img_width ; width++) {
    for (int height = 0 ; height < img_height; height++) {
      if (*(map + width + height*img_width) != xpm_transparency_color(XPM_8_8_8_8)){
        changeColorPixel(x + width,y + height,*(map+width+height*img_width));
      }
      
      
    }
  }
}

void (drawDown(int x, int y, int img_width, int img_height, uint32_t * map)){
  

  for(int i = 0; i < img_width; i++) {
    for (int j = 0; j < img_height; j++) {
      if (*(map - i - j*img_width) != xpm_transparency_color(XPM_8_8_8_8)){
        changeColorPixel(x+i,y+j,*(map - i - j*img_width));
      }        
    }
  }
}

void (drawLeft(int x, int y, int img_width, int img_height, uint32_t * map)){
  for(int j = 0 ; j < img_height; j++) {
    for (int i = 0 ; i < img_width ; i++) {
      if (*(map + i + j*img_width) != xpm_transparency_color(XPM_8_8_8_8)){
        changeColorPixel(x + j,y + img_width - i ,*(map + i + j*img_width));
      }
    }
  }
}

void (drawRight(int x, int y, int img_width, int img_height, uint32_t * map)){
  for(int i = 0; i < img_width; i++) {
    for (int j = 0; j < img_height; j++) {
      if (*(map + i + j*img_width) != xpm_transparency_color(XPM_8_8_8_8)){
        changeColorPixel(x + img_height - j,y + i,*(map + i + j*img_width));
      }
    }
  }
}

