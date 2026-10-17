#pragma once
#include "lcom/lcf.h"
#include "vector.h"

/**
 * @brief Struct that represents the cue
*/
typedef struct Cue{

  vector_t position;
  double angle;
  vector_t directionVector;
  xpm_image_t img;
  double charge;
  float powerSensitivity;
  uint8_t maximumDistance;
  uint8_t minimalDistance;
  double sideEnglish;
  double verticalEnglish;
  double elevationAngle;

  // Info about the guide lines
  vector_t colisionPoint;
  vector_t targetBallCenter;
  vector_t targetBallVec;
  vector_t whiteBallVec;
  double guideScalar;
  uint8_t guideLineMaximumSize;
  xpm_image_t guideImg;
  bool colides;
}Cue;

/**
 * @brief Creates a new cue
 * 
 * @return Cue* 
*/
Cue* newCue();

/**
 * @brief Destroys a Cue object.
 *
 * This function frees the memory allocated for the Cue object.
 *
 * @param cue Pointer to the Cue object to be destroyed.
 */

void destroyCue(Cue* cue);



