/**
 * @file Cloud.h
 * @brief tHis file contains information about the clouds drawn in the game
 * @date 2024-06-02
 */
#ifndef _CLOUD_H_
#define _CLOUD_H_

#include <stdio.h>
#include <stdlib.h>
#include "../utils/aux_structs/Position.h"

/**
 * @brief Cloud information
 * 
 */
typedef struct {
  double x, y;
  double velocity;
} Cloud;

#endif
