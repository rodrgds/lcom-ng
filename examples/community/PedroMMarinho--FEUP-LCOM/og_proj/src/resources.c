

#include "resources.h"
#include "model/table.h"
#include "model/ball/ball.h"
#include "model/menu.h"

Resources* loadResources(){

  Resources* resources = (Resources*)malloc(sizeof(Resources));
  resources->menu = newMenu(MAIN_MENU);
  resources->state = MENU;
  int randomNumber = rand() % 10000 + 1000;  
  sprintf(resources->playerName, "guest%d", randomNumber);
  return resources;
}

void destroyResources(Resources* resources){
  if(resources->state == PLAYING){
    destroyTable(resources->table);
  }
  else if(resources->state == MENU){
    destroyMenu(resources->menu);
  }
  free(resources->table);
  free(resources);
}
