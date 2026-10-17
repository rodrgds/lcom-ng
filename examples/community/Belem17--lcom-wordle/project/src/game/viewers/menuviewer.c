#include <lcom/lcf.h>
#include "menuviewer.h"
#include "../../utils/structs.h"
#include "../../utils/frames.h"
extern  Resources resources;
void showTitle() {
  drawframe(150,0,resources.Title);

}

void showMouse(struct Game *game){
  drawframe(game->mouseC.mouse_x, game->mouseC.mouse_y,resources.Mouse);

}
void showStartButton(){
  drawframe(350,y_axis/2,resources.startButton);

}


void showMenu(struct Game *game) {
  showStartButton();
  showTitle(); 
  showMouse(game);
}
