#include "button.h"

Button* newButton(xpm_image_t img, xpm_image_t imgSelected, vector_t pos, vector_t size){
  Button* button = (Button*)malloc(sizeof(Button));
  button->img = img;
  button->imgSelected = imgSelected;
  button->pos = pos;
  button->size = size;
  button->selected = false;
  return button;
}
void destroyButton(Button* button){
  free(button);
}


// TODO: fix this function, it is not working properly. coordinates are not being compared correctly. Coordinates are being compared as if the origin was at the top left corner of the screen, but it should be at the center of the object.
bool isMouseOverButton(Button* button, vector_t mousePos){
  //printf("mousePos.x: %d\nmousePos.y: %d\nbutton->pos.x: %d\nbutton->pos.y: %d\n", mousePos.x,mousePos.y,button->pos.x,button->pos.y);
  
  return mousePos.x >= button->pos.x && mousePos.x <= button->pos.x + button->size.x && mousePos.y >= button->pos.y && mousePos.y <= button->pos.y + button->size.y;
}
