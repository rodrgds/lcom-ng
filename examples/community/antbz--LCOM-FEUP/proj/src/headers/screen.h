#pragma once

#include <lcom/lcf.h>
#include <stdio.h>
#include <stdarg.h>
#include "videocard.h"
#include "fruit.h"


/**
 * @brief Screen struct.
 * 
 * Struct for Screen objects. Has the background image for a screen,
 * it's sprite array aka FruitBasket and the screen's interrupt handler.
 */
typedef struct {
  xpm_image_t background;
  FruitBasket fruit_basket ;
  void (*menu_handler)(int n);
} Screen;


/**
 * @brief Screen constructor.
 * 
 * Constructs a new Screen object.
 * 
 * @param menu_handler(int n) the screen's interrupt handler.
 * @param background the screen's background image.
 * @param fruit_basket the screen's FruitBasket.
 * @return A pointer to the newly created Screen.
 */
Screen* createScreen(void (*menu_handler)(int n), xpm_image_t background, FruitBasket fruit_basket);


/**
 * @brief Screen destructor.
 * 
 * @param screen a pointer to the screen to be destroyed.
 */
void destroyScreen(Screen* screen);
