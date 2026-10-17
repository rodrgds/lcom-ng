#include "cueViewer.h"
#include "../model/vector.h"
#include "../labs/graphics.h"
#include "xpms/cue.xpm"
#include "math.h"
#include "xpms/ballRing.xpm"
#include "lineViewer.h"

int drawCue(Table* table){
  Cue* cue = table->cue;
  drawXPMImage(cue->img, cue->position.x, cue->position.y, cue->angle);

  drawThickLine(table->balls[0]->position, cue->colisionPoint, 3, 0xffffff);
  drawXPMImage(cue->guideImg, cue->colisionPoint.x, cue->colisionPoint.y, 0);
  if (cue->colides){
    vector_t finalPoint;
    finalPoint.x = cue->targetBallCenter.x + cue->targetBallVec.x * cue->guideScalar * cue->guideLineMaximumSize;
    finalPoint.y = cue->targetBallCenter.y + cue->targetBallVec.y * cue->guideScalar * cue->guideLineMaximumSize;
    drawThickLine(cue->targetBallCenter, finalPoint, 3, 0xff0000);
    finalPoint.x = cue->colisionPoint.x + cue->whiteBallVec.x * (1- cue->guideScalar)* cue->guideLineMaximumSize;
    finalPoint.y = cue->colisionPoint.y + cue->whiteBallVec.y * (1- cue->guideScalar)* cue->guideLineMaximumSize;
    drawThickLine(cue->colisionPoint, finalPoint, 3, 0xff0000);
  }

  return 0;
}

