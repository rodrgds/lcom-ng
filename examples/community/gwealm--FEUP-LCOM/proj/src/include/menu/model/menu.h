#pragma once

#include <lcom/lcf.h>

/**
 * @brief Keeps menu info
 * 
 */
typedef struct {
  uint16_t x, y;
  uint16_t width, height;

  char *description;
  void (*action)();
} menu_option;

/**
 * @brief Keeps menu options
 * 
 */
typedef struct {
  menu_option options[8];
  size_t n;
} menu;

/**
 * @brief Get the menu
 * 
 */
menu *(menu_get)();

/**
 * @brief Resets the menu
 * 
 */
void (menu_reset)();
