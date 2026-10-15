#include "menu.h"
#include "game/logic/game_logic.h"
extern uint8_t scancode;
extern struct packet mouse_packet;
char names[7][10];
int scores[7];
char dates[7][18];
GameState currentState = RESOLUTION; 
extern vbe_mode_info_t info;
extern enemy monsters_fly[2];
extern uint16_t mode;
extern int score;
extern double multiplier;


char name[10];
uint8_t indexName=0;

int processScanCode(uint8_t scancode) {
  if(indexName<9) {
    if (scancode == SCAN_CODE_A) { 
      name[indexName] = 'a';
      indexName++;
    }
    else if (scancode == SCAN_CODE_B) { 
      name[indexName] = 'b';
      indexName++;
    }
    else if (scancode == SCAN_CODE_C) { 
      name[indexName] = 'c';
      indexName++;
    }
    else if (scancode == SCAN_CODE_D) { 
      name[indexName] = 'd';
      indexName++;
    }
    else if (scancode == SCAN_CODE_E) { 
      name[indexName] = 'e';
      indexName++;
    }
    else if (scancode == SCAN_CODE_F) { 
      name[indexName] = 'f';
      indexName++;
    }
    else if (scancode == SCAN_CODE_G) { 
      name[indexName] = 'g';
      indexName++;
    }
    else if (scancode == SCAN_CODE_H) { 
      name[indexName] = 'h';
      indexName++;
    }
    else if (scancode == SCAN_CODE_I) { 
      name[indexName] = 'i';
      indexName++;
    }
    else if (scancode == SCAN_CODE_J) {
      name[indexName] = 'j';
      indexName++;
    }
    else if (scancode == SCAN_CODE_K) {
      name[indexName] = 'k';
      indexName++;
    }
    else if (scancode == SCAN_CODE_L) {
      name[indexName] = 'l';
      indexName++;
    }
    else if (scancode == SCAN_CODE_M) {
      name[indexName] = 'm';
      indexName++;
    }
    else if (scancode == SCAN_CODE_N) {
      name[indexName] = 'n';
      indexName++;
    }
    else if (scancode == SCAN_CODE_O) {
      name[indexName] = 'o';
      indexName++;
    }
    else if (scancode == SCAN_CODE_P) {
      name[indexName] = 'p';
      indexName++;
    }
    else if (scancode == SCAN_CODE_Q) {
      name[indexName] = 'q';
      indexName++;
    }
    else if (scancode == SCAN_CODE_R) {
      name[indexName] = 'r';
      indexName++;
    }
    else if (scancode == SCAN_CODE_S) {
      name[indexName] = 's';
      indexName++;
    }
    else if (scancode == SCAN_CODE_T) {
      name[indexName] = 't';
      indexName++;
    }
    else if (scancode == SCAN_CODE_U) {
      name[indexName] = 'u';
      indexName++;
    }
    else if (scancode == SCAN_CODE_V) {
      name[indexName] = 'v';
      indexName++;
    }
    else if (scancode == SCAN_CODE_W) {
      name[indexName] = 'w';
      indexName++;
    }
    else if (scancode == SCAN_CODE_X) {
      name[indexName] = 'x';
      indexName++;
    }
    else if (scancode == SCAN_CODE_Y) {
      name[indexName] = 'y';
      indexName++;
    }
    else if (scancode == SCAN_CODE_Z) {
      name[indexName] = 'z';
      indexName++;
    }
    else if (scancode == SCAN_CODE_COLON) {
        name[indexName] = ':'; 
        indexName++;
    }
  }
  if(scancode==SCAN_CODE_ENTER){
    currentState = MENU;
    save_name_score(name, score);
    score=0;
    multiplier=1.1;
  }
  if (scancode == SCAN_CODE_BACKSPACE) {
    if(indexName>0)
        indexName--;
    name[indexName] = '\0';
  }
  return 0;
}

void setMenuState(){
    currentState=MENU;
    
}
int playButton(int x, int y){
    if(440*info.XResolution/DEFAULT_X_RESOLUTION_14C < x && 710*info.XResolution/DEFAULT_X_RESOLUTION_14C> x && 360*info.YResolution/DEFAULT_Y_RESOLUTION_14C < y && 430*info.YResolution/DEFAULT_Y_RESOLUTION_14C > y){
        if(mouse_packet.lb){
            currentState = GAME;
            monsters_fly[0] = *createEnemy(6, 5, FLYMONS1_X*info.XResolution/DEFAULT_X_RESOLUTION_14C, FLYMONS1_Y*info.YResolution/DEFAULT_Y_RESOLUTION_14C, 3, 3, monster2, true);
            monsters_fly[1] = *createEnemy(6,5, FLYMONS2_X*info.XResolution/DEFAULT_X_RESOLUTION_14C, FLYMONS2_Y*info.YResolution/DEFAULT_Y_RESOLUTION_14C, 3, 3, monster2, true);
        }
    }
    return 0;
}
int leaderboardButton(int x, int y){
    if(440*info.XResolution/DEFAULT_X_RESOLUTION_14C < x && 710*info.XResolution/DEFAULT_X_RESOLUTION_14C> x && 450*info.YResolution/DEFAULT_Y_RESOLUTION_14C < y && 520*info.YResolution/DEFAULT_Y_RESOLUTION_14C > y){
        if(mouse_packet.lb){
            read_scores(names,scores, dates,7);
            currentState = LEADERBOARD;
        }
        }
    return 0;
}
int exitButton(int x, int y){
    if(440*info.XResolution/DEFAULT_X_RESOLUTION_14C < x && 710*info.XResolution/DEFAULT_X_RESOLUTION_14C> x && 540*info.YResolution/DEFAULT_Y_RESOLUTION_14C < y && 610*info.YResolution/DEFAULT_Y_RESOLUTION_14C > y){
        if(mouse_packet.lb)
            currentState = EXIT;
        }
    return 0;
}
int menuButton(int x, int y, int *score, double *multiplier){
    if(335*info.XResolution/DEFAULT_X_RESOLUTION_14C < x && 800*info.XResolution/DEFAULT_X_RESOLUTION_14C> x && 455*info.YResolution/DEFAULT_Y_RESOLUTION_14C < y && 600*info.YResolution/DEFAULT_Y_RESOLUTION_14C > y){
        if(mouse_packet.lb) {
          currentState = MENU;
            save_name_score(name, *score);
            *score=0;
            *multiplier=1.1;
        }
            
        }
    
    return 0;
}


int drawMenu(){
    draw_sprite(menu_full, 0, 0);
    return 0;
}

int drawLeaderBoard(){
    draw_sprite(LeaderBackGrnd, 0, 0);
    //int numberOfScores = 7;
    //char names[numberOfScores][10];
    //char dates[numberOfScores][18];
    //int scores[numberOfScores];
    
   
    
    for(int i = 0; i< 7;i++) {
      if (strcmp(names[i], "default") == 0) {
          return 0;
      }
      drawTxt(names[i],270*info.XResolution/DEFAULT_X_RESOLUTION_14C,(260+30*i)*info.YResolution/DEFAULT_Y_RESOLUTION_14C);
      drawTxt(dates[i],(270+18*10)*info.XResolution/DEFAULT_X_RESOLUTION_14C,(260+30*i)*info.YResolution/DEFAULT_Y_RESOLUTION_14C);
      drawScore(scores[i],850*info.XResolution/DEFAULT_X_RESOLUTION_14C,(260+30*i)*info.YResolution/DEFAULT_Y_RESOLUTION_14C);
    }
    
    return 0;
}

int menuButtonLeader(int x, int y) {
  if(270*info.XResolution/DEFAULT_X_RESOLUTION_14C < x && 890*info.XResolution/DEFAULT_X_RESOLUTION_14C> x &&  600*info.YResolution/DEFAULT_Y_RESOLUTION_14C < y && 740*info.YResolution/DEFAULT_Y_RESOLUTION_14C > y){
        if(mouse_packet.lb)
            currentState = MENU;
        }
    
    return 0;
}

int drawGame(player *player, int score){

    
   draw_sprite(background, 0, 0);
    uint16_t xpos=5;
    for(uint8_t i = 0; i<player->life;i++) {
        draw_sprite(hearthLife, xpos*info.XResolution/DEFAULT_X_RESOLUTION_14C,5*info.YResolution/DEFAULT_Y_RESOLUTION_14C);
        xpos+=25;
    }
    drawScore(score, 1100*info.XResolution/DEFAULT_X_RESOLUTION_14C, 38*info.YResolution/DEFAULT_Y_RESOLUTION_14C);
    
    return 0;
}

int drawScoreBoard(int score) {
    if(draw_sprite(ScoreBackGrnd, 0, 0)) {return 1;}
    if(drawTxt("your score:", 470*info.XResolution/DEFAULT_X_RESOLUTION_14C, 290*info.YResolution/DEFAULT_Y_RESOLUTION_14C)) {return 1;}
    if(drawScore(score, 585*info.XResolution/DEFAULT_X_RESOLUTION_14C, 330*info.YResolution/DEFAULT_Y_RESOLUTION_14C)) {return 1;}
    if(drawTxt("name:", 470*info.XResolution/DEFAULT_X_RESOLUTION_14C, 360*info.YResolution/DEFAULT_Y_RESOLUTION_14C)) {return 1;}
    if(drawTxt(name,(470+6*18)*info.XResolution/DEFAULT_X_RESOLUTION_14C, 360*info.YResolution/DEFAULT_Y_RESOLUTION_14C)) {return 1;}


    return 0;
}

int drawScore(int score, uint16_t x, uint16_t y) {   
    uint16_t pos=0;
    int widthLetters=0;
    if(mode == 0x11A) {
        widthLetters=20;
    } else if (mode == 0x110) {
        widthLetters=10;
    } else if (mode == 0x14C){
        widthLetters = 18;
    } else if (mode == 0x115){
        widthLetters=12;
    }
    while (score>0) {
        uint8_t rem = score%10;
        if(drawNum(rem, x-pos*widthLetters,y)) {return 1;} 
        pos++;
        score = score / 10;
    }
    
    
    return 0;
}
int drawResolution(){
  draw_sprite(ResChooseBckgrd, 0, 0);
  return 0;
}


int res14C(int x, int y){
    if(60 < x && 250> x && 130 < y && 210 > y){
        if(mouse_packet.lb){
          currentState = MENU;
          return 0;
        }
    }
    else{
      return 1;
    }
    return 1;
}
int res110(int x, int y){
    if(60 < x && 250> x && 270 < y && 350 > y){
        if(mouse_packet.lb){
          currentState = MENU;
          return 0;
        }
    }
    else{
      return 1;
    }
    return 1;
}
int res11A(int x, int y){
    if(390 < x && 580> x && 270 < y && 350 > y){
        if(mouse_packet.lb){
          currentState = MENU;
          return 0;
        }
    }
    else{
      return 1;
    }
    return 1;
}
int res115(int x, int y){
    if(390 < x && 580> x && 130 < y && 210 > y){
        if(mouse_packet.lb){
            currentState = MENU;
          return 0;
        }
    }
    else{
      return 1;
    }
    return 1;
}
