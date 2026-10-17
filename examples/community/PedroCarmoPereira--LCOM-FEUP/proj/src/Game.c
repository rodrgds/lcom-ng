#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "Game.h"
#include "kbd.h"
#include "assets/background.h"
#include "assets/font.h"
#include "assets/writtenAssets.h"
#include "Score.h"

xpm_image_t title;
xpm_image_t gameOver;
xpm_image_t newHSimg;
bool lockU1, lockU2, lockU0, lockD1, lockD2, lockD0;
Entity arrowU1;
Entity arrowD1;
Entity arrowU2;
Entity arrowD2;
Entity arrowU0;
Entity arrowD0;
Entity playButton;
Entity highScoresButton;
Entity quitButton;
Entity menuButton;

extern bool pintou;
extern uint8_t byte_no;
extern struct packet p;
extern uint8_t scancode[2];
extern int counter;


void loadFont(){
  xpm_load(n0, XPM_8_8_8_8, &font[0]);
  xpm_load(n1, XPM_8_8_8_8, &font[1]);
  xpm_load(n2, XPM_8_8_8_8, &font[2]);
  xpm_load(n3, XPM_8_8_8_8, &font[3]);
  xpm_load(n4, XPM_8_8_8_8, &font[4]);
  xpm_load(n5, XPM_8_8_8_8, &font[5]);
  xpm_load(n6, XPM_8_8_8_8, &font[6]);
  xpm_load(n7, XPM_8_8_8_8, &font[7]);
  xpm_load(n8, XPM_8_8_8_8, &font[8]);
  xpm_load(n9, XPM_8_8_8_8, &font[9]);
  xpm_load(lA, XPM_8_8_8_8, &font[10]);
  xpm_load(lB, XPM_8_8_8_8, &font[11]);
  xpm_load(lC, XPM_8_8_8_8, &font[12]);
  xpm_load(lD, XPM_8_8_8_8, &font[13]);
  xpm_load(lE, XPM_8_8_8_8, &font[14]);
  xpm_load(lF, XPM_8_8_8_8, &font[15]);
  xpm_load(lG, XPM_8_8_8_8, &font[16]);
  xpm_load(lH, XPM_8_8_8_8, &font[17]);
  xpm_load(lI, XPM_8_8_8_8, &font[18]);
  xpm_load(lJ, XPM_8_8_8_8, &font[19]);
  xpm_load(lK, XPM_8_8_8_8, &font[20]);
  xpm_load(lL, XPM_8_8_8_8, &font[21]);
  xpm_load(lM, XPM_8_8_8_8, &font[22]);
  xpm_load(lN, XPM_8_8_8_8, &font[23]);
  xpm_load(lO, XPM_8_8_8_8, &font[24]);
  xpm_load(lP, XPM_8_8_8_8, &font[25]);
  xpm_load(lQ, XPM_8_8_8_8, &font[26]);
  xpm_load(lR, XPM_8_8_8_8, &font[27]);
  xpm_load(lS, XPM_8_8_8_8, &font[28]);
  xpm_load(lT, XPM_8_8_8_8, &font[29]);
  xpm_load(lU, XPM_8_8_8_8, &font[30]);
  xpm_load(lV, XPM_8_8_8_8, &font[31]);
  xpm_load(lW, XPM_8_8_8_8, &font[32]);
  xpm_load(lU, XPM_8_8_8_8, &font[30]);
  xpm_load(lV, XPM_8_8_8_8, &font[31]);
  xpm_load(lW, XPM_8_8_8_8, &font[32]);
  xpm_load(lX, XPM_8_8_8_8, &font[33]);
  xpm_load(lY, XPM_8_8_8_8, &font[34]);
  xpm_load(lZ, XPM_8_8_8_8, &font[35]);
  xpm_load(arrowup, XPM_8_8_8_8, &font[36]);
  xpm_load(arrowdown, XPM_8_8_8_8, &font[37]);
}

Game game(){
  xpm_load(title_xpm, XPM_8_8_8_8, &title);
  xpm_load(gameover_xpm, XPM_8_8_8_8, &gameOver);
  xpm_load(newHighScore, XPM_8_8_8_8, &newHSimg);
  title.type = XPM_8_8_8_8;
  playButton = entity(PLAYBUTTON);
  highScoresButton = entity(HIGHSCORESBUTTON);
  quitButton = entity(QUITBUTTON);
  menuButton = entity(MENUBUTTON);
  Game g;
  g.state = MENU;
  g.mouse = entity(MOUSE);
  loadFont();
  arrowU0.img = font[36]; setPosition(&arrowU0, SCOREX*9 + arrowU0.img.width/4, SCOREY*1.5 - 50);
  arrowU1.img = font[36]; setPosition(&arrowU1, SCOREX*11 + arrowU0.img.width/4, SCOREY*1.5 - 50);
  arrowU2.img = font[36]; setPosition(&arrowU2, SCOREX*13 + arrowU0.img.width/4, SCOREY*1.5 - 50);
  arrowD0.img = font[37]; setPosition(&arrowD0, SCOREX*9 + arrowU0.img.width/4, SCOREY*1.5 + 75);
  arrowD1.img = font[37]; setPosition(&arrowD1, SCOREX*11 + arrowU0.img.width/4, SCOREY*1.5 + 75);
  arrowD2.img = font[37]; setPosition(&arrowD2, SCOREX*13 + arrowU0.img.width/4, SCOREY*1.5 + 75);
  lockD0 = false; lockD1 = false; lockD2 = false; lockU0 = false; lockU1 = false; lockU2 = false;
  xpm_load(background, XPM_8_8_8_8, &g.background);
  return g;

}


void drawMenu(){
  draw_sproot(title, 175, 25);
  drawEntity(&playButton);
  drawEntity(&highScoresButton);
  drawEntity(&quitButton);
  return;
}

void drawNHS(Score *s1){
  draw_sproot(newHSimg, 175, 50);
  draw_sproot(font[s1->letters[0] - 55], SCOREX*9, SCOREY*1.5);
  draw_sproot(font[s1->letters[1] - 55], SCOREX*11, SCOREY*1.5);
  draw_sproot(font[s1->letters[2] - 55], SCOREX*13, SCOREY*1.5);
  drawEntity(&arrowD0);
  drawEntity(&arrowD1);
  drawEntity(&arrowD2);
  drawEntity(&arrowU0);
  drawEntity(&arrowU1);
  drawEntity(&arrowU2);
  drawEntity(&menuButton);
  drawEntity(&quitButton);
}

void drawTopPlayerTags(){
  draw_sproot(font[highscores[0].letters[0] - 55], SCOREX + SCOREDISPSIZE, SCOREY);
  draw_sproot(font[highscores[0].letters[1] - 55], SCOREX + 2*SCOREDISPSIZE, SCOREY);
  draw_sproot(font[highscores[0].letters[2] - 55], SCOREX + 3*SCOREDISPSIZE, SCOREY);
  draw_sproot(font[highscores[1].letters[0] - 55], SCOREX + SCOREDISPSIZE, SCOREY + SCOREDISPSIZE);
  draw_sproot(font[highscores[1].letters[1] - 55], SCOREX + 2*SCOREDISPSIZE, SCOREY + SCOREDISPSIZE);
  draw_sproot(font[highscores[1].letters[2] - 55], SCOREX + 3*SCOREDISPSIZE, SCOREY + SCOREDISPSIZE);
  draw_sproot(font[highscores[2].letters[0] - 55], SCOREX + SCOREDISPSIZE, SCOREY + 2*SCOREDISPSIZE);
  draw_sproot(font[highscores[2].letters[1] - 55], SCOREX + 2*SCOREDISPSIZE, SCOREY + 2*SCOREDISPSIZE);
  draw_sproot(font[highscores[2].letters[2] - 55], SCOREX + 3*SCOREDISPSIZE, SCOREY + 2*SCOREDISPSIZE);
}


void drawTopPlayerScores(){
  char s0[10];
  char s1[10];
  char s2[10];
  sprintf(s0, "%d", highscores[0].value);
  sprintf(s1, "%d", highscores[1].value);
  sprintf(s2, "%d", highscores[2].value);
  int i = 0;
  while(s0[i] != '\0'){
    draw_sproot(font[s0[i] - 48], SCOREX + (4 + i)*SCOREDISPSIZE, SCOREY);
    i++;
  }
  i = 0;
  while(s1[i] != '\0'){
    draw_sproot(font[s1[i] - 48], SCOREX + (4 + i)*SCOREDISPSIZE, SCOREY + SCOREDISPSIZE);
    i++;
  }
  i = 0;
  while(s2[i] != '\0'){
    draw_sproot(font[s2[i] - 48], SCOREX + (4 + i)*SCOREDISPSIZE, SCOREY + 2 * SCOREDISPSIZE);
    i++;
  }
}

void drawTopPlayerDates(){
  char d0[13];
  char d1[13];
  char d2[13];
  int i = 0;
  int j = 0;
  dateToString(&highscores[0].data, d0);
  dateToString(&highscores[1].data, d1);
  dateToString(&highscores[2].data, d2);
  if (highscores[0].data.day != 0){
    while(d0[i] != '\0'){
      if(d0[i] == '/') {
        i++;
        continue;
      }
      draw_sproot(font[d0[i] - 48], 14.5*SCOREX + j*SCOREDISPSIZE, SCOREY);
      i++;
      j++;
    }
  }
  i = 0; j = 0;
  if (highscores[1].data.day != 0){
    while(d1[i] != '\0'){
      if(d1[i] == '/'){
        i++;
        continue;
      }
      draw_sproot(font[d1[i] - 48], 14.5*SCOREX + j*SCOREDISPSIZE, SCOREY);
      j++;
      i++;
    }
  }
  i = 0; j = 0;
  if (highscores[2].data.day != 0){
    while(d2[i] != '\0'){
      if(d2[i] == '/') {
        i++;
        continue;
      }
      draw_sproot(font[d2[i] - 48], 14.5*SCOREX + j*SCOREDISPSIZE, SCOREY);
      i++;
      j++;
    }
  }

}

void drawHighScores(){
  draw_sproot(title, 175, 50);
  draw_sproot(font[1], SCOREX,  SCOREY);
  draw_sproot(font[2], SCOREX,  SCOREY + SCOREDISPSIZE);
  draw_sproot(font[3], SCOREX,  SCOREY + 2*SCOREDISPSIZE);
  drawTopPlayerTags();
  drawTopPlayerScores();
  drawTopPlayerDates();
  drawEntity(&menuButton);
}

void drawGameOver(){
  draw_sproot(gameOver, 175, 50);
  drawEntity(&menuButton);
  drawEntity(&quitButton);
}

void drawGame(Game *g){
  draw_background(&g->background);
  if (g->state == PLAYING){
    drawGS(g->gamestate);
  }
  else if (g->state == MENU) drawMenu();

  else if (g->state == GAMEOVER) drawGameOver();

  else if (g->state == HIGHSCORES) drawHighScores();

  else if (g->state == NEWHIGHSCORE) drawNHS(&g->fs);

  drawEntity(&g->mouse);
  swap_buffer();
}

void reactToMouseEvent(Game *  g){
  mouse_ih();
  if(byte_no == 0 && pintou) setPosition(&g->mouse, g->mouse.p.x + p.delta_x, g->mouse.p.y - p.delta_y);

  if (g->state == PLAYING){
    if (p.lb == 1 && g->gamestate.player.fc == 0){
      generateLaser(&g->gamestate, g->mouse.p.x, g->mouse.p.y);
      g->gamestate.player.fc++;
    }
  }


  else if (g->state == MENU) {
    if (colision(&g->mouse, &quitButton) && p.lb == 1) g->state = EXIT;

    else if (colision(&g->mouse, &playButton) && p.lb == 1){
      g->state = PLAYING;
      g->gamestate = gamestate();
      return;
    }

    else if(colision(&g->mouse, &highScoresButton) && p.lb == 1) g->state = HIGHSCORES;
  }

  else if(g->state == GAMEOVER){
    if (colision(&g->mouse, &quitButton) && p.lb == 1) g->state = EXIT;

    if (colision(&g->mouse, &menuButton) && p.lb == 1) g->state = MENU;
  }

  else if (g->state == HIGHSCORES){
    if (colision(&g->mouse, &menuButton) && p.lb == 1) g->state = MENU;
  }

  else if (g->state == NEWHIGHSCORE){
    if (colision(&g->mouse, &menuButton) && p.lb == 1){
       g->state = MENU;
       updateHighScores(&g->fs);
    }

    else if(colision(&g->mouse, &quitButton) && p.lb == 1){
       g->state = EXIT;
       updateHighScores(&g->fs);
    }

    else if(colision(&g->mouse, &arrowU0) && p.lb == 1 && !lockU0){
       upLetterZero(&g->fs);
       lockU0 = true;
    }

    else if(colision(&g->mouse, &arrowU1) && p.lb == 1 && !lockU1){
       upLetterOne(&g->fs);
       lockU1 = true;
    }

    else if(colision(&g->mouse, &arrowU2) && p.lb == 1&& !lockU2){
       upLetterTwo(&g->fs);
       lockU2 = true;
    }

    else if(colision(&g->mouse, &arrowD0) && p.lb == 1 && !lockD0){
       downLetterZero(&g->fs);
       lockD0 = true;
    }

    else if(colision(&g->mouse, &arrowD1) && p.lb == 1 && !lockD1){
       downLetterOne(&g->fs);
       lockD1 = true;
    }

    else if(colision(&g->mouse, &arrowD2) && p.lb == 1 && !lockD2){
       downLetterTwo(&g->fs);
       lockD2 = true;
    }
  }
}

void reactToKBDEvent(Game *g){
  kbd_ih();
  if (g->state == PLAYING){
    if (scancode[0] == A_MAKE || scancode[0] == ARROW_LEFT_MAKE)
      g->gamestate.player.dir.left = true;
    if (scancode[0] == D_MAKE || scancode[0] == ARROW_RIGHT_MAKE)
      g->gamestate.player.dir.right = true;
    if (scancode[0] == W_MAKE || scancode[0] == ARROW_UP_MAKE)
      g->gamestate.player.dir.up = true;
    if (scancode[0] == S_MAKE || scancode[0] == ARROW_DOWN_MAKE)
      g->gamestate.player.dir.down = true;
    if (scancode[0] == A_BREAK || scancode[0] == ARROW_LEFT_BREAK)
      g->gamestate.player.dir.left = false;
    if (scancode[0] == D_BREAK || scancode[0] == ARROW_RIGHT_BREAK)
      g->gamestate.player.dir.right = false;
    if (scancode[0] == W_BREAK || scancode[0] == ARROW_UP_BREAK)
      g->gamestate.player.dir.up = false;
    if (scancode[0] == S_BREAK || scancode[0] == ARROW_DOWN_BREAK)
      g->gamestate.player.dir.down = false;

    movePlayer(&g->gamestate.player);
  }
  if(is_twoByte() && scancode[1] != 0) scancode[0] = 0;
}

void reactToTimerEvent(Game *g){
  timer_int_handler();
  if((FRAME_RATE * counter) % sys_hz() == 0){
    if (g->state == PLAYING){
      moveObstacles(&g->gamestate.obstacles);
      moveLasers(&g->gamestate);
      dmgEnemies(&g->gamestate);
      LaserColide(&g->gamestate);
      freeObstacles(&g->gamestate.obstacles);
      freeLasers(&g->gamestate.laser);
      freeLasers(&g->gamestate.enemyFire);
      g->gamestate.ek += freeEnemies(&g->gamestate.enemies);
      g->gamestate.score = POINTS_PER_KILL * g->gamestate.ek;
      if(g->gamestate.ek >= 6 && g->gamestate.ek < 12) g->gamestate.phase = 2;
      else if(g->gamestate.ek >= 12 && g->gamestate.ek < 18) g->gamestate.phase = 3;
      else if(g->gamestate.ek == 18) g->gamestate.phase = 4;
      else if(g->gamestate.ek > 18) g->gamestate.phase = 3;
  }
  drawGame(g);
  }
  if (g->state == PLAYING && counter % sys_hz() == 0){
      g->gamestate.player.fc ++;
      if (g->gamestate.player.invulnerable) g->gamestate.player.invulnerable = false;
      if(g->gamestate.player.fc == 2) g->gamestate.player.fc = 0;
      g->gamestate.score++;
      enemiesFire(&g->gamestate);
    }
if (g->state == NEWHIGHSCORE && counter % sys_hz() == 0){
  if (lockD0) lockD0 = false;
  if (lockD1) lockD1 = false;
  if (lockD2) lockD2 = false;
  if (lockU0) lockU0 = false;
  if (lockU1) lockU1 = false;
  if (lockU2) lockU2 = false;
  }
}
