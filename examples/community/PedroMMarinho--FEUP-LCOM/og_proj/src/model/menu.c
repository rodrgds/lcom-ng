#include "menu.h"
#include "viewer/menuViewer.h"
#include "../labs/graphics.h"

#include "xpms/menu/back.xpm"
#include "xpms/menu/backSelected.xpm"
#include "xpms/menu/gameModeMenuLocal.xpm"
#include "xpms/menu/gameModeMenuLocalSelected.xpm"
#include "xpms/menu/gameModeMenuOnline.xpm"
#include "xpms/menu/gameModeMenuOnlineSelected.xpm"
#include "xpms/menu/mainMenuExit.xpm"
#include "xpms/menu/mainMenuExitSelected.xpm"
#include "xpms/menu/mainMenuPlay.xpm"
#include "xpms/menu/mainMenuPlaySelected.xpm"
#include "xpms/menu/mainMenuBackground.xpm"
#include "xpms/menu/mainMenuInfo.xpm"
#include "xpms/menu/mainMenuInfoSelected.xpm"
#include "xpms/menu/menuMouse.xpm"
#include "xpms/menu/menuMouseHover.xpm"
#include "xpms/menu/instructionsMenuBackground.xpm"
#include "xpms/menu/logo.xpm"
#include "xpms/menu/mainMenuOptionsButton.xpm"
#include "xpms/menu/optionsMenuBackground.xpm"
#include "xpms/menu/optionsMenuSaveButton.xpm"
#include "xpms/menu/optionsMenuSaveButtonSelected.xpm"
#include "xpms/menu/mainMenuOptionsButtonSelected.xpm"
#include "xpms/font/font.xpm"
#include "xpms/menu/onlineMenuBackground.xpm"
#include "xpms/menu/ellipsesBall.xpm"
#include "xpms/menu/gameOverMenuBackground.xpm"

Menu* newMenu(MenuType type){
  Menu* menu = (Menu*)malloc(sizeof(Menu));
  setMenuType(menu,type);
  menu->mouse = newMouse();
  xpm_image_t menuMouse;
  xpm_image_t menuMouseHover;
  xpm_load(menuMouseXpm, XPM_8_8_8, &menuMouse);
  xpm_load(menuMouseHoverXpm, XPM_8_8_8, &menuMouseHover);
  menu->mouseImg = menuMouse;
  menu->mouseImgHover = menuMouseHover;
  menu->winner = NULL;
  return menu;
}


Menu* newGameOverMenu(char* winnerName){
  Menu* menu = newMenu(GAME_OVER_MENU);
  menu->winner = winnerName;
  return menu;
}




void setMenuType(Menu* menu, MenuType type){
  menu->type = type;
  
  xpm_image_t menuBackground;
  
  vector_t pos = {195,500};
  vector_t size = {200,102};
  xpm_image_t gameModeMenuLocal;
  xpm_image_t gameModeMenuLocalSelected;
  xpm_image_t gameModeMenuOnline;
  xpm_image_t gameModeMenuOnlineSelected;
  xpm_image_t back;
  xpm_image_t backSelected;
  xpm_image_t mainMenuPlay;
  xpm_image_t mainMenuPlaySelected;
  xpm_image_t mainMenuInfo;
  xpm_image_t mainMenuInfoSelected;
  xpm_image_t mainMenuExit;
  xpm_image_t mainMenuExitSelected;
  xpm_image_t logo;
  xpm_image_t mainMenuOptionsButton;
  xpm_image_t mainMenuOptionsButtonSelected;
  xpm_image_t optionsMenuBackground;
  xpm_image_t onlineMenuBackground;

  switch (type)
  {
  case MAIN_MENU:
  
    menu->nOptions = 4;
    menu->buttons = (Button**)malloc(sizeof(Button*)*menu->nOptions);    
    xpm_load(mainMenuPlayXpm, XPM_8_8_8, &mainMenuPlay);
    xpm_load(mainMenuPlaySelectedXpm, XPM_8_8_8, &mainMenuPlaySelected);
    xpm_load(mainMenuInfoXpm, XPM_8_8_8, &mainMenuInfo);
    xpm_load(mainMenuInfoSelectedXpm, XPM_8_8_8, &mainMenuInfoSelected);
    xpm_load(mainMenuExitXpm, XPM_8_8_8, &mainMenuExit);
    xpm_load(mainMenuExitSelectedXpm, XPM_8_8_8, &mainMenuExitSelected);
    xpm_load(mainMenuOptionsButtonXpm, XPM_8_8_8, &mainMenuOptionsButton);
    xpm_load(mainMenuOptionsButtonSelectedXpm, XPM_8_8_8, &mainMenuOptionsButtonSelected);
    
  
    menu->buttons[0] = newButton(mainMenuPlay,mainMenuPlaySelected,pos,size);
    pos.x = 629;
    menu->buttons[1] = newButton(mainMenuInfo,mainMenuInfoSelected,pos,size);
    pos.y = 650;
    pos.x = 412;
    menu->buttons[2] = newButton(mainMenuExit,mainMenuExitSelected,pos,size);
    pos.y = 500;
    menu->buttons[3] = newButton(mainMenuOptionsButton,mainMenuOptionsButtonSelected,pos,size);
    xpm_load(mainMenuBackgroundXpm, XPM_8_8_8, &menuBackground);
    printf("Background loaded\n");
    menu->backgroundImg = menuBackground;
    xpm_load(logoXpm, XPM_8_8_8, &logo);
      
    
    menu->mainMenuLogo = logo;
    break;
  case GAME_MODE_MENU:
    
    menu->nOptions = 3;
      menu->buttons = (Button**)malloc(sizeof(Button*)*menu->nOptions);
      xpm_load(gameModeMenuLocalXpm, XPM_8_8_8, &gameModeMenuLocal);
      xpm_load(gameModeMenuLocalSelectedXpm, XPM_8_8_8, &gameModeMenuLocalSelected);
      xpm_load(gameModeMenuOnlineXpm, XPM_8_8_8, &gameModeMenuOnline);
      xpm_load(gameModeMenuOnlineSelectedXpm, XPM_8_8_8, &gameModeMenuOnlineSelected);
      xpm_load(backXpm, XPM_8_8_8, &back);
      xpm_load(backSelectedXpm, XPM_8_8_8, &backSelected);
      menu->buttons[0] = newButton(gameModeMenuLocal,gameModeMenuLocalSelected,pos,size);
      pos.x = 629;
      menu->buttons[1] = newButton(gameModeMenuOnline,gameModeMenuOnlineSelected,pos,size);
      pos.y = 650;
      pos.x = 412;
      menu->buttons[2] = newButton(back,backSelected,pos,size);
      xpm_load(mainMenuBackgroundXpm, XPM_8_8_8, &menuBackground);
      menu->backgroundImg = menuBackground;
      
      break;
  case INSTRUCTIONS_MENU:
      pos.x = 629;
      pos.y = 650;
      menu->nOptions = 1;
      menu->buttons = (Button**)malloc(sizeof(Button*)*menu->nOptions);
      xpm_load(backXpm, XPM_8_8_8, &back);
      xpm_load(backSelectedXpm, XPM_8_8_8, &backSelected);
      menu->buttons[0] = newButton(back,backSelected,pos,size);
      xpm_load(instructionsMenuBackgroundXpm, XPM_8_8_8, &menuBackground);
      menu->backgroundImg = menuBackground;
      break;
  case GAME_OVER_MENU:
      pos.x = 500;
      pos.y = 650;       
      menu->nOptions = 1;
      menu->buttons = (Button**)malloc(sizeof(Button*)*menu->nOptions);
      xpm_load(backXpm, XPM_8_8_8, &back);
      xpm_load(backSelectedXpm, XPM_8_8_8, &backSelected);
      menu->buttons[0] = newButton(back, backSelected,pos,size);
      xpm_load(gameOverMenuBackgroundXpm, XPM_8_8_8, &menuBackground);
      menu->font = malloc(sizeof(xpm_image_t)*36);
      for(int i = 0; i < 36; i++){
        xpm_load(xpm_table[i], XPM_8_8_8, &menu->font[i]);
      }
      menu->backgroundImg = menuBackground;
     break;
  case OPTIONS_MENU:
      pos.y = 650;
      pos.x = 412;
      menu->nOptions = 1;
      menu->buttons = (Button**)malloc(sizeof(Button*)*menu->nOptions);
      xpm_load(backXpm, XPM_8_8_8, &back);
      xpm_load(backSelectedXpm, XPM_8_8_8, &backSelected);
      menu->buttons[0] = newButton(back,backSelected,pos,size);
      printf("Loading font\n");
      menu->font = malloc(sizeof(xpm_image_t)*36);
      for(int i = 0; i < 36; i++){
        xpm_load(xpm_table[i], XPM_8_8_8, &menu->font[i]);
      }

      xpm_load(optionsMenuBackgroundXpm, XPM_8_8_8, &optionsMenuBackground);
      menu->backgroundImg = optionsMenuBackground;     
      break;
    case ONLINE_MENU:
      pos.y = 650;
      pos.x = 412;
      menu->nOptions = 1;
      menu->buttons = (Button**)malloc(sizeof(Button*)*menu->nOptions);
      xpm_load(backXpm, XPM_8_8_8, &back);
      xpm_load(backSelectedXpm, XPM_8_8_8, &backSelected);
      menu->buttons[0] = newButton(back,backSelected,pos,size);
      xpm_load(onlineMenuBackgroundXpm, XPM_8_8_8, &onlineMenuBackground);

      menu->backgroundImg = onlineMenuBackground;
      break;
  }
  menu->selectedOption = -1;
}

void destroyMenu(Menu* menu){
  free(menu->mouse);
  free(menu);
}
void resetMenu(Menu* menu){
  menu->selectedOption = -1;
  for(int i = 0; i < menu->nOptions; i++){
    destroyButton(menu->buttons[i]);
  }
}
int drawMenu(Menu* menu,int time,char* playerName){
  if( drawMenuBackground(menu->backgroundImg)) return 1;
  if(menu->type == MAIN_MENU ){
    if(drawMenuLogo(menu->mainMenuLogo,time)) return 1;
  }
  // draw player name
  if(menu->type == OPTIONS_MENU){

    if(drawPlayerName(playerName,menu->font,452,345,14)) return 1;
  }

  if(menu->type == GAME_OVER_MENU){
    if(drawPlayerName(menu->winner,menu->font,452,345,14)) return 1;
    if(drawText("congratulations",menu->font,452,365,14)) return 1;
    if(drawText("you",menu->font,452,385,14)) return 1;
    if(drawText("won",menu->font,542,385,14)) return 1;
  }
  if(menu->type == ONLINE_MENU){
    xpm_image_t ellipsesBall;
    xpm_load(ellipsesBallXpm, XPM_8_8_8, &ellipsesBall);
    if(drawEllipses(ellipsesBall,time)) return 1;
  }
  for(int i = 0; i < menu->nOptions; i++){
    if(drawMenuButton(menu->buttons[i])) return 1;
  }
  if(drawMenuMouse(menu->mouse,menu->mouseImg,menu->mouseImgHover,menu->selectedOption != -1)) return 1;
  return 0;
}
int seeIfMouseIsOverOption(Menu* menu, int* option){ 
  
  for( int i = 0; i < menu->nOptions; i++){
    if(isMouseOverButton(menu->buttons[i],menu->mouse->pos)){
      *option = i;
      return 0;
    }
  }
  *option = -1;
  return 0;
}
void updateSelectedOption(Menu* menu, int option){

  for( int i = 0; i < menu->nOptions; i++){
    bool selected = i == option;
    menu->buttons[i]->selected = selected;
  }
  menu->selectedOption = option;
}
void destroyFont(xpm_image_t* font){
  for(int i = 0; i < 36; i++){
    free(font[i].bytes);
  }
  free(font);
}

