/**
 * @file Datetime.h
 * @brief This file contains the Datetime struct
 * @date 2024-06-02
 */
#ifndef _DATETIME_H_
#define _DATETIME_H_

#include <lcom/lcf.h>

#include <stdint.h>

/**
 * @brief Datetime struct
 * 
 */
typedef struct Datetime {
  uint8_t day;
  uint8_t month;
  uint8_t year;

  uint8_t seconds;
  uint8_t minutes;
  uint8_t hours;
} Datetime;

#endif
