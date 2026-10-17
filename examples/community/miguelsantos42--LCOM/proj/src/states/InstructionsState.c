#include "src/include/states/InstructionsState.h"
#include "src/include/controllers/video/projXPM.h"
#include "src/include/controllers/video/graphics.h"
#include <string.h>
#include <ctype.h>


InstructionsState* instructionState = NULL;


struct InstructionsStateData { 
  int number;
};
  
typedef struct InstructionsStateData InstructionsStateData;

static const xpm_map_t* letters[26] = {
  &XPMMapLetterA, &XPMMapLetterB, &XPMMapLetterC, &XPMMapLetterD, &XPMMapLetterE, &XPMMapLetterF,
  &XPMMapLetterG, &XPMMapLetterH, &XPMMapLetterI, &XPMMapLetterJ, &XPMMapLetterK, &XPMMapLetterL,
  &XPMMapLetterM, &XPMMapLetterN, &XPMMapLetterO, &XPMMapLetterP, &XPMMapLetterQ, &XPMMapLetterR,
  &XPMMapLetterS, &XPMMapLetterT, &XPMMapLetterU, &XPMMapLetterV, &XPMMapLetterW, &XPMMapLetterX,
  &XPMMapLetterY, &XPMMapLetterZ
};

static xpm_image_t loaddedLetters[26];
static uint8_t* mappedLetters[26];

static void loadLetters() {
  for (int i = 0; i < 26; i++){
    mappedLetters[i] = NULL;
  }
}

static void drawLetter(char letter, int16_t x, int16_t y) {
  loadLetters();

  int index = letter - 'A';
  if(mappedLetters[index] == NULL) {
    load_xpm(*letters[index], &loaddedLetters[index], &mappedLetters[index]);
  }
  draw_xpm(loaddedLetters[index], mappedLetters[index], x, y);
}

static void drawText(const char* text, int16_t x, int16_t y) {
  int startX = x;
  
  for(int i = 0; text[i] != '\0'; i++) {
    if(text[i] == '\n') {
      x = startX;
      y += 20;
    } else if (text[i] == ' ') {
      x += 16;
    } else if (text[i] >= 'A' && text[i] <= 'Z') {
      drawLetter(text[i], x, y);
      x += 16;
    }

    if(x > 1000) { 
      x = startX;
      y += 20;
    }
  }
}

static void destroyInstructionsData(InstructionsStateData* data) {
  if(data) free(data);
}

static void destroy(InstructionsState* state) {
  if(!state) return;
  InstructionsStateData* data = state->data;
  if (data) destroyInstructionsData(data);
  free(state);
}

static void draw(InstructionsState* state) {
  reset_buffer();

  // Draw the instructions content
  drawText("GAME INSTRUCTIONS", 50, 50);
  drawText("", 50, 70); // Empty line
  
  drawText("WELCOME TO THE 2D MAP ADVENTURE GAME IN THIS GAME, YOU TAKE ON", 50, 90);
  drawText("THE ROLE OF A HEROIC CHARACTER NAVIGATING A DANGEROUS", 50, 110);
  drawText("MAP WHILE AVOIDING MONSTER UTILIZING STRATEGIC SKILLS AND OVERCOMING OBSTACLES", 50, 130);
  drawText("", 50, 150); // Empty line
  
  drawText("OBJECTIVE", 50, 180);
  drawText("", 50, 200); // Empty line
  
  drawText("SURVIVE AS LONG POSSIBLE BY AVOIDING MONSTER, MANAGING YOUR", 50, 220);
  drawText(" RESOURCE AND USING YOUR SKILLS STRATEGICALLY. EARN POINTS BY ", 50, 240);
  drawText("OUTSMARTING THE MONSTER AND STAYING ALIVE", 50, 260);
  drawText("", 50, 280); // Empty line
  
  drawText("CONTROLS", 50, 310);
  drawText("", 50, 330); // Empty line
  
  drawText("MOVEMENT: USE THE ARROW KEYS OR (W,S,A,D) TO MOVE YOUR HERO ACROSS", 50, 350);
  drawText("THE MAP. SHOOT: PRESS THE SPACEBAR TO SHOOT BULLETS", 50, 370);
  drawText("", 50, 390); // Empty line
  
  drawText("TIPS", 50, 420);
  drawText("WHEN SHOOTING HERO WILL SHOOT IN THE LAST DIRECTION YOU ", 50, 440);
  drawText("MOVED SO PLAN AHEAD YOUR MOVEMENT TO NOT BE CAUGHT DEFENDLESS", 50, 460);
  drawText("YOU HAVE LIMITED AMMO SO USE YOUR SHOTS WISELY", 50, 480);
  drawText("THERE IS A RELOAD TIME AFTER USING YOUR BULLETS", 50, 500);
  drawText("", 50, 520); // Empty line
  
  drawText("PRESS ESC TO RETURN TO MENU", 50, 700);
}

InstructionsState* instructionsStateConstructor() {
  InstructionsState* state = malloc(sizeof(InstructionsState));
  InstructionsStateData* data = malloc(sizeof(InstructionsStateData));

  data->number = 0;

  state->data = data;
  state->draw = draw;
  state->destroy = destroy;

  return state;
}
