#pragma once

#include <lcom/lcf.h>

/**
 * @brief Enumerates the possible states of the game
 * 
 */
typedef enum STATE {
  PLAYING,
  MENU,
  OVER,
} STATE;

/**
 * @brief Enumerates the possible devices
 * 
 */
typedef enum DEVICE {
  MOUSE,
  KEYBOARD,
  RTC,
  PORT,
  TIMER,
  SP,
} DEVICE;

