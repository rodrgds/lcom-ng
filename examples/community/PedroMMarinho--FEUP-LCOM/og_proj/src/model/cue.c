
#include "cue.h"
#include "../xpms/ballRing.xpm"
#include "../xpms/cue.xpm"
#include "../labs/graphics.h"


Cue* newCue(){
  Cue* cue = (Cue*)malloc(sizeof(Cue));
  cue->charge = 0;
  cue->powerSensitivity = 0.5;
  cue->maximumDistance = 200;
  cue->minimalDistance = 20;
  cue->guideLineMaximumSize = 100;

  cue->sideEnglish = 0;
  cue->verticalEnglish = 0;

  // Set image
  xpm_image_t img;
  xpm_load(ballRingXpm, XPM_8_8_8, &img);
  cue->guideImg = img;

  xpm_load(cueXpm, XPM_8_8_8, &img);
  cue->img = img;
  return cue;
}

void destroyCue(Cue* cue){
  free(cue);
}
