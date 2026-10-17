
#include "../labs/graphics.h"
#include <lcom/lcf.h>
#include "lineViewer.h"




void drawLineOverlap(unsigned int aXStart, unsigned int aYStart, unsigned int aXEnd, unsigned int aYEnd, uint8_t aOverlap,
                     uint32_t aColor) {

  int16_t tDeltaX, tDeltaY, tDeltaXTimes2, tDeltaYTimes2, tError, tStepX, tStepY;

  // calculate direction
  tDeltaX = aXEnd - aXStart;
  tDeltaY = aYEnd - aYStart;
  if (tDeltaX < 0) {
    tDeltaX = -tDeltaX;
    tStepX = -1;
  }
  else {
    tStepX = +1;
  }
  if (tDeltaY < 0) {
    tDeltaY = -tDeltaY;
    tStepY = -1;
  }
  else {
    tStepY = +1;
  }
  tDeltaXTimes2 = tDeltaX << 1;
  tDeltaYTimes2 = tDeltaY << 1;
  // draw start pixel
  draw_pixel(aXStart, aYStart, aColor);
  if (tDeltaX > tDeltaY) {
    // start value represents a half step in Y direction
    tError = tDeltaYTimes2 - tDeltaX;
    while (aXStart != aXEnd) {
      // step in main direction
      aXStart += tStepX;
      if (tError >= 0) {
        if (aOverlap & LINE_OVERLAP_MAJOR) {
          // draw pixel in main direction before changing
          draw_pixel(aXStart, aYStart, aColor);
        }
        // change Y
        aYStart += tStepY;
        if (aOverlap & LINE_OVERLAP_MINOR) {
          // draw pixel in minor direction before changing
          draw_pixel(aXStart - tStepX, aYStart, aColor);
        }
        tError -= tDeltaXTimes2;
      }
      tError += tDeltaYTimes2;
      draw_pixel(aXStart, aYStart, aColor);
    }
  }
  else {
    tError = tDeltaXTimes2 - tDeltaY;
    while (aYStart != aYEnd) {
      aYStart += tStepY;
      if (tError >= 0) {
        if (aOverlap & LINE_OVERLAP_MAJOR) {
          // draw pixel in main direction before changing
          draw_pixel(aXStart, aYStart, aColor);
        }
        aXStart += tStepX;
        if (aOverlap & LINE_OVERLAP_MINOR) {
          // draw pixel in minor direction before changing
          draw_pixel(aXStart, aYStart - tStepY, aColor);
        }
        tError -= tDeltaYTimes2;
      }
      tError += tDeltaXTimes2;
      draw_pixel(aXStart, aYStart, aColor);
    }
  }
}

/**
 * Bresenham with thickness
 * No pixel missed and every pixel only drawn once!
 * The code is bigger and more complicated than drawThickLineSimple() but it tends to be faster, since drawing a pixel is often a slow operation.
 * aThicknessMode can be one of LINE_THICKNESS_MIDDLE, LINE_THICKNESS_DRAW_CLOCKWISE, LINE_THICKNESS_DRAW_COUNTERCLOCKWISE
 */
  void drawThickLine(vector_t start, vector_t end, unsigned int aThickness, uint32_t aColor) {
    unsigned int aXStart=start.x;
    unsigned int aYStart=start.y;
    unsigned int aXEnd=end.x;
    unsigned int aYEnd = end.y;

  // printf("First %d:%d .... Second %d:%d\n",aXStart, aYStart, aXEnd, aYEnd);
  int16_t i, tDeltaX, tDeltaY, tDeltaXTimes2, tDeltaYTimes2, tError, tStepX, tStepY;

  if (aThickness <= 1) {
    drawLineOverlap(aXStart, aYStart, aXEnd, aYEnd, LINE_OVERLAP_NONE, aColor);
  }

  /**
   * For coordinate system with 0.0 top left
   * Swap X and Y delta and calculate clockwise (new delta X inverted)
   * or counterclockwise (new delta Y inverted) rectangular direction.
   * The right rectangular direction for LINE_OVERLAP_MAJOR toggles with each octant
   */
  tDeltaY = aXEnd - aXStart;
  tDeltaX = aYEnd - aYStart;
  // mirror 4 quadrants to one and adjust deltas and stepping direction
  bool tSwap = true; // count effective mirroring
  if (tDeltaX < 0) {
    tDeltaX = -tDeltaX;
    tStepX = -1;
    tSwap = !tSwap;
  }
  else {
    tStepX = +1;
  }
  if (tDeltaY < 0) {
    tDeltaY = -tDeltaY;
    tStepY = -1;
    tSwap = !tSwap;
  }
  else {
    tStepY = +1;
  }
  tDeltaXTimes2 = tDeltaX << 1;
  tDeltaYTimes2 = tDeltaY << 1;
  bool tOverlap;
  // adjust for right direction of thickness from line origin
  int tDrawStartAdjustCount = aThickness / 2;

  /*
   * Now tDelta* are positive and tStep* define the direction
   * tSwap is false if we mirrored only once
   */
  // which octant are we now
  if (tDeltaX >= tDeltaY) {
    // Octant 1, 3, 5, 7 (between 0 and 45, 90 and 135, ... degree)
    if (tSwap) {
      tDrawStartAdjustCount = (aThickness - 1) - tDrawStartAdjustCount;
      tStepY = -tStepY;
    }
    else {
      tStepX = -tStepX;
    }
    /*
     * Vector for draw direction of the starting points of lines is rectangular and counterclockwise to main line direction
     * Therefore no pixel will be missed if LINE_OVERLAP_MAJOR is used on change in minor rectangular direction
     */
    // adjust draw start point
    tError = tDeltaYTimes2 - tDeltaX;
    for (i = tDrawStartAdjustCount; i > 0; i--) {
      // change X (main direction here)
      aXStart -= tStepX;
      aXEnd -= tStepX;
      if (tError >= 0) {
        // change Y
        aYStart -= tStepY;
        aYEnd -= tStepY;
        tError -= tDeltaXTimes2;
      }
      tError += tDeltaYTimes2;
    }
    // draw start line.
    drawLineOverlap(aXStart, aYStart, aXEnd, aYEnd, LINE_OVERLAP_NONE, aColor);
    // draw aThickness number of lines
    tError = tDeltaYTimes2 - tDeltaX;
    for (i = aThickness; i > 1; i--) {
      // change X (main direction here)
      aXStart += tStepX;
      aXEnd += tStepX;
      tOverlap = LINE_OVERLAP_NONE;
      if (tError >= 0) {
        // change Y
        aYStart += tStepY;
        aYEnd += tStepY;
        tError -= tDeltaXTimes2;
        tOverlap = LINE_OVERLAP_MAJOR;
      }
      tError += tDeltaYTimes2;
      drawLineOverlap(aXStart, aYStart, aXEnd, aYEnd, tOverlap, aColor);
    }
  }
  else {
    // the other octant 2, 4, 6, 8 (between 45 and 90, 135 and 180, ... degree)
    if (tSwap) {
      tStepX = -tStepX;
    }
    else {
      tDrawStartAdjustCount = (aThickness - 1) - tDrawStartAdjustCount;
      tStepY = -tStepY;
    }
    // adjust draw start point
    tError = tDeltaXTimes2 - tDeltaY;
    for (i = tDrawStartAdjustCount; i > 0; i--) {
      aYStart -= tStepY;
      aYEnd -= tStepY;
      if (tError >= 0) {
        aXStart -= tStepX;
        aXEnd -= tStepX;
        tError -= tDeltaYTimes2;
      }
      tError += tDeltaXTimes2;
    }
    // draw start line
    drawLineOverlap(aXStart, aYStart, aXEnd, aYEnd, LINE_OVERLAP_NONE, aColor);
    // draw aThickness number of lines
    tError = tDeltaXTimes2 - tDeltaY;
    for (i = aThickness; i > 1; i--) {
      aYStart += tStepY;
      aYEnd += tStepY;
      tOverlap = LINE_OVERLAP_NONE;
      if (tError >= 0) {
        aXStart += tStepX;
        aXEnd += tStepX;
        tError -= tDeltaYTimes2;
        tOverlap = LINE_OVERLAP_MAJOR;
      }
      tError += tDeltaXTimes2;
      drawLineOverlap(aXStart, aYStart, aXEnd, aYEnd, tOverlap, aColor);
    }
  }
}
