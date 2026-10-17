#include "menuViewer.h"
#include "../labs/graphics.h"

int drawMenuBackground(xpm_image_t img){
  if(drawXPMImage(img,512,384,0)) return 1;
  return 0;
}
int drawMenuButton(Button* button){
    if(button->selected){
        if(drawXPMImage(button->imgSelected,button->pos.x+100,button->pos.y+51,0)) return 1;
    }
    else{
        if(drawXPMImage(button->img,button->pos.x+100,button->pos.y+51,0)) return 1;
    }
    return 0;
}

int drawMenuMouse(Mouse* mouse, xpm_image_t mouseImg, xpm_image_t imgMouseHover, bool isHovering){
    if(isHovering){
        if(drawXPMImage(imgMouseHover,mouse->pos.x,mouse->pos.y,0)) return 1;
    }
    else{
        if(drawXPMImage(mouseImg,mouse->pos.x,mouse->pos.y,0)) return 1;
    }
    return 0;
}
int drawMenuLogo(xpm_image_t img,int time){
    int y = 175 + (int)(sin(time*0.05)*10);
    if(drawXPMImage(img,512,y,0)) return 1;
    return 0;
}

int drawPlayerName(char* playerName, xpm_image_t* playerNameFont, uint16_t x, uint16_t y, uint8_t spacing){
    if(drawText(playerName,playerNameFont,x,y,spacing)) return 1;
    printf("Drawing player name\n");
    return 0;
}


int drawEllipses(xpm_image_t imgBall,int time){
    int x = 450;
    int y = 430;
    int spacing = 69;
    int frame = (time/30)%4;
    for(int i = 0; i < frame; i++){
        if(drawXPMImage(imgBall,x + i*spacing,y,0)) return 1;
    }
    return 0;
}
