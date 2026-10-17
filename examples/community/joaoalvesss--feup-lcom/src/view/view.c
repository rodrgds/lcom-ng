#include "view.h"

uint8_t *firstBuffer;
uint8_t *secondBuffer;
uint8_t *thirdBuffer;
uint32_t bufferSize;
uint64_t table;
uint8_t card1 = 0;
uint8_t card2 = 0;

int frame_x_movment = 0;
int frame_y_movment = 0;
uint8_t cards_flipped[16];

extern vbe_mode_info_t m_info;
extern struct packet packet_mouse;
extern Menu menState;
extern Difficulty difState;
extern MouseHelper mouseHelper;
extern Frame frameState;
extern Setup setupState;
extern GamePhase gamePhase;
extern bool cardSelected;
extern int timer_count;

extern Sprite *back1;
extern Sprite *back2;
extern Sprite *back3;
extern Sprite *dino1;
extern Sprite *dino2;
extern Sprite *dino3;
extern Sprite *dino4;
extern Sprite *dino5;
extern Sprite *dino6;
extern Sprite *dino7;
extern Sprite *dino8;
extern Sprite *colon;
extern Sprite *digit0;
extern Sprite *digit1;
extern Sprite *digit2;
extern Sprite *digit3;
extern Sprite *digit4;
extern Sprite *digit5;
extern Sprite *digit6;
extern Sprite *digit7;
extern Sprite *digit8;
extern Sprite *digit9;
extern Sprite *mainmenu;
extern Sprite *menufinal;
extern Sprite *background;
extern Sprite *dino_mouse;
extern Sprite *frame;
extern Sprite *dino_cursor;

Sprite **backgroundFrames = NULL;
int numBackgroundFrames = 0;
int currentFrameIndex = 0;
Sprite **animation=NULL;
bool setupDone=false;
int frameCounter=0;

void setupAnimation(){
     if (animation != NULL) {
          free(animation);
          animation = NULL;
     }
     animation = (Sprite **)malloc(sizeof(Sprite *) * 3);
     animation[0]=back1;
     animation[1]=back2;
     animation[2]=back3;
     numBackgroundFrames=3;
}

void initializeBackgroundFrames(int size) {
     if (backgroundFrames != NULL) {
          free(backgroundFrames);
          backgroundFrames = NULL;
     }
    backgroundFrames = (Sprite **)malloc(sizeof(Sprite *) * size);
    for (int i = 0; i < size; i++) {
          backgroundFrames[i] = back1;
    }
}


void setupFrames(){
     if(difState == EASY){
          setupAnimation();
          initializeBackgroundFrames(8);
     }
     else if(difState == MEDIUM){
          setupAnimation();
          initializeBackgroundFrames(12);
     }
     else if(difState == HARD){
          setupAnimation();
          initializeBackgroundFrames(16);
     }
     setupDone=true;
}

void drawNewFrame(){
     if(menState == MENU_START){
          setupDone=false;
          drawStartMenu();
     } 
     else if(menState == MENU_INGAME) {
          if(!setupDone){
               setupFrames();
          }
          if(difState == EASY){
               drawEasyGame();
          }
          else if(difState == MEDIUM){
               drawMediumGame();
          }
          else if(difState == HARD){
               drawHardGame();
          }
     } 
     else if(menState == MENU_END) {
          drawEndMenu();
     } 
     drawMouse();
}

void drawStartMenu(){
     drawSpriteXpm(mainmenu, 0, 0);
}

void drawFrames(int i){
     if(i==8){
          drawSpriteXpm(backgroundFrames[0], 224, 317);
          drawSpriteXpm(backgroundFrames[1], 404, 317);
          drawSpriteXpm(backgroundFrames[2], 584, 317);
          drawSpriteXpm(backgroundFrames[3], 764, 317);
          drawSpriteXpm(backgroundFrames[4], 224, 497);
          drawSpriteXpm(backgroundFrames[5], 404, 497);
          drawSpriteXpm(backgroundFrames[6], 584, 497);
          drawSpriteXpm(backgroundFrames[7], 764, 497);
     }else if(i==12){
          drawSpriteXpm(backgroundFrames[0], 224, 137);
          drawSpriteXpm(backgroundFrames[1], 404, 137);
          drawSpriteXpm(backgroundFrames[2], 584, 137);
          drawSpriteXpm(backgroundFrames[3], 764, 137);
          drawSpriteXpm(backgroundFrames[4], 224, 317);
          drawSpriteXpm(backgroundFrames[5], 404, 317);
          drawSpriteXpm(backgroundFrames[6], 584, 317);
          drawSpriteXpm(backgroundFrames[7], 764, 317);
          drawSpriteXpm(backgroundFrames[8], 224, 497);
          drawSpriteXpm(backgroundFrames[9], 404, 497);
          drawSpriteXpm(backgroundFrames[10], 584, 497);
          drawSpriteXpm(backgroundFrames[11], 764, 497);
     }else if(i==16){
          drawSpriteXpm(backgroundFrames[0], 224, 137);
          drawSpriteXpm(backgroundFrames[1], 404, 137);
          drawSpriteXpm(backgroundFrames[2], 584, 137);
          drawSpriteXpm(backgroundFrames[3], 764, 137);
          drawSpriteXpm(backgroundFrames[4], 224, 317);
          drawSpriteXpm(backgroundFrames[5], 404, 317);
          drawSpriteXpm(backgroundFrames[6], 584, 317);
          drawSpriteXpm(backgroundFrames[7], 764, 317);
          drawSpriteXpm(backgroundFrames[8], 224, 497);
          drawSpriteXpm(backgroundFrames[9], 404, 497);
          drawSpriteXpm(backgroundFrames[10], 584, 497);
          drawSpriteXpm(backgroundFrames[11], 764, 497);
          drawSpriteXpm(backgroundFrames[12], 224, 677);
          drawSpriteXpm(backgroundFrames[13], 404, 677);
          drawSpriteXpm(backgroundFrames[14], 584, 677);
          drawSpriteXpm(backgroundFrames[15], 764, 677);
     }

}

void changeFrame(int i){
     if (cards_flipped[i]==0){
          backgroundFrames[i]=animation[currentFrameIndex];          
     } 
}

void drawEasyGame() {
    drawSpriteXpm(background, 0, 0);
    drawSpriteXpm(frame, 216 + frame_x_movment, 310 + frame_y_movment);
    if (mouseHelper.x_pos >= 224 && mouseHelper.x_pos <= 374 && mouseHelper.y_pos >= 317 && mouseHelper.y_pos <= 467) {
        changeFrame(0);
    } else if (mouseHelper.x_pos >= 404 && mouseHelper.x_pos <= 554 && mouseHelper.y_pos >= 317 && mouseHelper.y_pos <= 467) {
        changeFrame(1);
    } else if (mouseHelper.x_pos >= 584 && mouseHelper.x_pos <= 734 && mouseHelper.y_pos >= 317 && mouseHelper.y_pos <= 467) {
        changeFrame(2);
    } else if (mouseHelper.x_pos >= 764 && mouseHelper.x_pos <= 914 && mouseHelper.y_pos >= 317 && mouseHelper.y_pos <= 467) {
        changeFrame(3);
    } else if (mouseHelper.x_pos >= 224 && mouseHelper.x_pos <= 374 && mouseHelper.y_pos >= 497 && mouseHelper.y_pos <= 647) {
        changeFrame(4);
    } else if (mouseHelper.x_pos >= 404 && mouseHelper.x_pos <= 554 && mouseHelper.y_pos >= 497 && mouseHelper.y_pos <= 647) {
        changeFrame(5);
    } else if (mouseHelper.x_pos >= 584 && mouseHelper.x_pos <= 734 && mouseHelper.y_pos >= 497 && mouseHelper.y_pos <= 647) {
        changeFrame(6);
    } else if (mouseHelper.x_pos >= 764 && mouseHelper.x_pos <= 914 && mouseHelper.y_pos >= 497 && mouseHelper.y_pos <= 647) {
        changeFrame(7);
    }
     drawFrames(8);
     display_time(450,150);
    currentFrameIndex = (currentFrameIndex + 1) % numBackgroundFrames;
}

void drawMediumGame(){
     drawSpriteXpm(background, 0, 0);
     drawSpriteXpm(frame, 216 + frame_x_movment, 130 + frame_y_movment);

     if (mouseHelper.x_pos >= 224 && mouseHelper.x_pos <= 374 && mouseHelper.y_pos >= 137 && mouseHelper.y_pos <= 287) {
          changeFrame(0);
    } else if (mouseHelper.x_pos >= 404 && mouseHelper.x_pos <= 554 && mouseHelper.y_pos >= 137 && mouseHelper.y_pos <= 287) {
          changeFrame(1);
    } else if (mouseHelper.x_pos >= 584 && mouseHelper.x_pos <= 734 && mouseHelper.y_pos >= 137 && mouseHelper.y_pos <= 287) {
          changeFrame(2);
    } else if (mouseHelper.x_pos >= 764 && mouseHelper.x_pos <= 914 && mouseHelper.y_pos >= 137 && mouseHelper.y_pos <= 287) {
          changeFrame(3);
    } else if (mouseHelper.x_pos >= 224 && mouseHelper.x_pos <= 374 && mouseHelper.y_pos >= 317 && mouseHelper.y_pos <= 467) {
          changeFrame(4);
    } else if (mouseHelper.x_pos >= 404 && mouseHelper.x_pos <= 554 && mouseHelper.y_pos >= 317 && mouseHelper.y_pos <= 467) {
          changeFrame(5);
    } else if (mouseHelper.x_pos >= 584 && mouseHelper.x_pos <= 734 && mouseHelper.y_pos >= 317 && mouseHelper.y_pos <= 467) {
          changeFrame(6);
    } else if (mouseHelper.x_pos >= 764 && mouseHelper.x_pos <= 914 && mouseHelper.y_pos >= 317 && mouseHelper.y_pos <= 467) {
          changeFrame(7);
     } else if (mouseHelper.x_pos >= 224 && mouseHelper.x_pos <= 374 && mouseHelper.y_pos >= 497 && mouseHelper.y_pos <= 647) {
          changeFrame(8);
     } else if (mouseHelper.x_pos >= 404 && mouseHelper.x_pos <= 554 && mouseHelper.y_pos >= 497 && mouseHelper.y_pos <= 647) {
          changeFrame(9);
     } else if (mouseHelper.x_pos >= 584 && mouseHelper.x_pos <= 734 && mouseHelper.y_pos >= 497 && mouseHelper.y_pos <= 647) {
          changeFrame(10);
     } else if (mouseHelper.x_pos >= 764 && mouseHelper.x_pos <= 914 && mouseHelper.y_pos >= 497 && mouseHelper.y_pos <= 647) {
          changeFrame(11);
    }
     
     drawFrames(12);
     display_time(450,50);
     currentFrameIndex = (currentFrameIndex + 1) % numBackgroundFrames;
}

void drawHardGame(){
     drawSpriteXpm(background, 0, 0);
     drawSpriteXpm(frame, 216 + frame_x_movment, 130 + frame_y_movment);

     if (mouseHelper.x_pos >= 224 && mouseHelper.x_pos <= 374 && mouseHelper.y_pos >= 137 && mouseHelper.y_pos <= 287) {
          changeFrame(0);
    } else if (mouseHelper.x_pos >= 404 && mouseHelper.x_pos <= 554 && mouseHelper.y_pos >= 137 && mouseHelper.y_pos <= 287) {
          changeFrame(1);
    } else if (mouseHelper.x_pos >= 584 && mouseHelper.x_pos <= 734 && mouseHelper.y_pos >= 137 && mouseHelper.y_pos <= 287) {
          changeFrame(2);
    } else if (mouseHelper.x_pos >= 764 && mouseHelper.x_pos <= 914 && mouseHelper.y_pos >= 137 && mouseHelper.y_pos <= 287) {
          changeFrame(3);
    } else if (mouseHelper.x_pos >= 224 && mouseHelper.x_pos <= 374 && mouseHelper.y_pos >= 317 && mouseHelper.y_pos <= 467) {
          changeFrame(4);
    } else if (mouseHelper.x_pos >= 404 && mouseHelper.x_pos <= 554 && mouseHelper.y_pos >= 317 && mouseHelper.y_pos <= 467) {
          changeFrame(5);
    } else if (mouseHelper.x_pos >= 584 && mouseHelper.x_pos <= 734 && mouseHelper.y_pos >= 317 && mouseHelper.y_pos <= 467) {
          changeFrame(6);
    } else if (mouseHelper.x_pos >= 764 && mouseHelper.x_pos <= 914 && mouseHelper.y_pos >= 317 && mouseHelper.y_pos <= 467) {
          changeFrame(7);
     } else if (mouseHelper.x_pos >= 224 && mouseHelper.x_pos <= 374 && mouseHelper.y_pos >= 497 && mouseHelper.y_pos <= 647) {
          changeFrame(8);
     } else if (mouseHelper.x_pos >= 404 && mouseHelper.x_pos <= 554 && mouseHelper.y_pos >= 497 && mouseHelper.y_pos <= 647) {
          changeFrame(9);
     } else if (mouseHelper.x_pos >= 584 && mouseHelper.x_pos <= 734 && mouseHelper.y_pos >= 497 && mouseHelper.y_pos <= 647) {
          changeFrame(10);
     } else if (mouseHelper.x_pos >= 764 && mouseHelper.x_pos <= 914 && mouseHelper.y_pos >= 497 && mouseHelper.y_pos <= 647) {
          changeFrame(11);
     } else if (mouseHelper.x_pos >= 224 && mouseHelper.x_pos <= 374 && mouseHelper.y_pos >= 677 && mouseHelper.y_pos <= 827) {
          changeFrame(12);
     } else if (mouseHelper.x_pos >= 404 && mouseHelper.x_pos <= 554 && mouseHelper.y_pos >= 677 && mouseHelper.y_pos <= 827) {
          changeFrame(13);
     } else if (mouseHelper.x_pos >= 584 && mouseHelper.x_pos <= 734 && mouseHelper.y_pos >= 677 && mouseHelper.y_pos <= 827) {
          changeFrame(14);
     } else if (mouseHelper.x_pos >= 764 && mouseHelper.x_pos <= 914 && mouseHelper.y_pos >= 677 && mouseHelper.y_pos <= 827) {
          changeFrame(15);
    }

     drawFrames(16);
     display_time(450,50);
     currentFrameIndex = (currentFrameIndex + 1) % numBackgroundFrames;
}

void drawEndMenu(){
     drawSpriteXpm(menufinal, 0, 0);
}

void drawMouse() {
    if (menState == MENU_START) {
        if ((mouseHelper.x_pos > 493 && mouseHelper.x_pos < 659 && mouseHelper.y_pos > 430 && mouseHelper.y_pos < 473)
            || (mouseHelper.x_pos > 467 && mouseHelper.x_pos < 684 && mouseHelper.y_pos > 504 && mouseHelper.y_pos < 546)
            || (mouseHelper.x_pos > 495 && mouseHelper.x_pos < 661 && mouseHelper.y_pos > 577 && mouseHelper.y_pos < 618)
            || (mouseHelper.x_pos > 489 && mouseHelper.x_pos < 665 && mouseHelper.y_pos > 713 && mouseHelper.y_pos < 756)) {
            drawSpriteXpm(dino_mouse, mouseHelper.x_pos, mouseHelper.y_pos);
        }
        else {
            drawSpriteXpm(dino_cursor, mouseHelper.x_pos, mouseHelper.y_pos);
        }
    }
    else if (menState == MENU_INGAME) {
        if (difState == EASY && (
            (mouseHelper.x_pos >= 224 && mouseHelper.x_pos <= 374 && mouseHelper.y_pos >= 317 && mouseHelper.y_pos <= 467) ||
            (mouseHelper.x_pos >= 404 && mouseHelper.x_pos <= 554 && mouseHelper.y_pos >= 317 && mouseHelper.y_pos <= 467) ||
            (mouseHelper.x_pos >= 584 && mouseHelper.x_pos <= 734 && mouseHelper.y_pos >= 317 && mouseHelper.y_pos <= 467) ||
            (mouseHelper.x_pos >= 764 && mouseHelper.x_pos <= 914 && mouseHelper.y_pos >= 317 && mouseHelper.y_pos <= 467) ||
            (mouseHelper.x_pos >= 224 && mouseHelper.x_pos <= 374 && mouseHelper.y_pos >= 497 && mouseHelper.y_pos <= 647) ||
            (mouseHelper.x_pos >= 404 && mouseHelper.x_pos <= 554 && mouseHelper.y_pos >= 497 && mouseHelper.y_pos <= 647) ||
            (mouseHelper.x_pos >= 584 && mouseHelper.x_pos <= 734 && mouseHelper.y_pos >= 497 && mouseHelper.y_pos <= 647) ||
            (mouseHelper.x_pos >= 764 && mouseHelper.x_pos <= 914 && mouseHelper.y_pos >= 497 && mouseHelper.y_pos <= 647)
            )) {
            drawSpriteXpm(dino_mouse, mouseHelper.x_pos, mouseHelper.y_pos);
        }
        else if (difState == MEDIUM && (
            (mouseHelper.x_pos >= 224 && mouseHelper.x_pos <= 374 && mouseHelper.y_pos >= 317 && mouseHelper.y_pos <= 467) ||
            (mouseHelper.x_pos >= 404 && mouseHelper.x_pos <= 554 && mouseHelper.y_pos >= 317 && mouseHelper.y_pos <= 467) ||
            (mouseHelper.x_pos >= 584 && mouseHelper.x_pos <= 734 && mouseHelper.y_pos >= 317 && mouseHelper.y_pos <= 467) ||
            (mouseHelper.x_pos >= 764 && mouseHelper.x_pos <= 914 && mouseHelper.y_pos >= 317 && mouseHelper.y_pos <= 467) ||
            (mouseHelper.x_pos >= 224 && mouseHelper.x_pos <= 374 && mouseHelper.y_pos >= 497 && mouseHelper.y_pos <= 647) ||
            (mouseHelper.x_pos >= 404 && mouseHelper.x_pos <= 554 && mouseHelper.y_pos >= 497 && mouseHelper.y_pos <= 647) ||
            (mouseHelper.x_pos >= 584 && mouseHelper.x_pos <= 734 && mouseHelper.y_pos >= 497 && mouseHelper.y_pos <= 647) ||
            (mouseHelper.x_pos >= 764 && mouseHelper.x_pos <= 914 && mouseHelper.y_pos >= 497 && mouseHelper.y_pos <= 647) ||
            (mouseHelper.x_pos >= 224 && mouseHelper.x_pos <= 374 && mouseHelper.y_pos >= 137 && mouseHelper.y_pos <= 287) ||
            (mouseHelper.x_pos >= 404 && mouseHelper.x_pos <= 554 && mouseHelper.y_pos >= 137 && mouseHelper.y_pos <= 287) ||
            (mouseHelper.x_pos >= 584 && mouseHelper.x_pos <= 734 && mouseHelper.y_pos >= 137 && mouseHelper.y_pos <= 287) ||
            (mouseHelper.x_pos >= 764 && mouseHelper.x_pos <= 914 && mouseHelper.y_pos >= 137 && mouseHelper.y_pos <= 287)
            )) {
            drawSpriteXpm(dino_mouse, mouseHelper.x_pos, mouseHelper.y_pos);
        }
        else if (difState == HARD && (
            (mouseHelper.x_pos >= 224 && mouseHelper.x_pos <= 374 && mouseHelper.y_pos >= 317 && mouseHelper.y_pos <= 467) ||
            (mouseHelper.x_pos >= 404 && mouseHelper.x_pos <= 554 && mouseHelper.y_pos >= 317 && mouseHelper.y_pos <= 467) ||
            (mouseHelper.x_pos >= 584 && mouseHelper.x_pos <= 734 && mouseHelper.y_pos >= 317 && mouseHelper.y_pos <= 467) ||
            (mouseHelper.x_pos >= 764 && mouseHelper.x_pos <= 914 && mouseHelper.y_pos >= 317 && mouseHelper.y_pos <= 467) ||
            (mouseHelper.x_pos >= 224 && mouseHelper.x_pos <= 374 && mouseHelper.y_pos >= 497 && mouseHelper.y_pos <= 647) ||
            (mouseHelper.x_pos >= 404 && mouseHelper.x_pos <= 554 && mouseHelper.y_pos >= 497 && mouseHelper.y_pos <= 647) ||
            (mouseHelper.x_pos >= 584 && mouseHelper.x_pos <= 734 && mouseHelper.y_pos >= 497 && mouseHelper.y_pos <= 647) ||
            (mouseHelper.x_pos >= 764 && mouseHelper.x_pos <= 914 && mouseHelper.y_pos >= 497 && mouseHelper.y_pos <= 647) ||
            (mouseHelper.x_pos >= 224 && mouseHelper.x_pos <= 374 && mouseHelper.y_pos >= 137 && mouseHelper.y_pos <= 287) ||
            (mouseHelper.x_pos >= 404 && mouseHelper.x_pos <= 554 && mouseHelper.y_pos >= 137 && mouseHelper.y_pos <= 287) ||
            (mouseHelper.x_pos >= 584 && mouseHelper.x_pos <= 734 && mouseHelper.y_pos >= 137 && mouseHelper.y_pos <= 287) ||
            (mouseHelper.x_pos >= 764 && mouseHelper.x_pos <= 914 && mouseHelper.y_pos >= 137 && mouseHelper.y_pos <= 287) ||
            (mouseHelper.x_pos >= 224 && mouseHelper.x_pos <= 374 && mouseHelper.y_pos >= 677 && mouseHelper.y_pos <= 827) ||
            (mouseHelper.x_pos >= 404 && mouseHelper.x_pos <= 554 && mouseHelper.y_pos >= 677 && mouseHelper.y_pos <= 827) ||
            (mouseHelper.x_pos >= 584 && mouseHelper.x_pos <= 734 && mouseHelper.y_pos >= 677 && mouseHelper.y_pos <= 827) ||
            (mouseHelper.x_pos >= 764 && mouseHelper.x_pos <= 914 && mouseHelper.y_pos >= 677 && mouseHelper.y_pos <= 827)
            )) {
            drawSpriteXpm(dino_mouse, mouseHelper.x_pos, mouseHelper.y_pos);
        }
        else {
            drawSpriteXpm(dino_cursor, mouseHelper.x_pos, mouseHelper.y_pos);
        }
    }
    else if (menState == MENU_END) {
        if ((mouseHelper.x_pos > 413 && mouseHelper.x_pos < 739 && mouseHelper.y_pos > 508 && mouseHelper.y_pos < 557) ||
            (mouseHelper.x_pos > 420 && mouseHelper.x_pos < 734 && mouseHelper.y_pos > 642 && mouseHelper.y_pos < 704)) {
            drawSpriteXpm(dino_mouse, mouseHelper.x_pos, mouseHelper.y_pos);
        }
        else {
            drawSpriteXpm(dino_cursor, mouseHelper.x_pos, mouseHelper.y_pos);
        }
    }
}


int flipBuffers(){
     if(memcpy(firstBuffer, secondBuffer, bufferSize) == NULL) return 1;
     return 0;
}

int drawSpriteXpm(Sprite *sprite, int x, int y){
     for(int h = 0; h < sprite->height; h++){
          for(int w = 0; w < sprite->width; w++){
               uint32_t color = sprite->bitmap[(h * sprite->width) + w];
               if(color == 0xFFFFFE) continue;
               if(draw_pixel(x + w, y + h, color, thirdBuffer) != 0) return 1;
          }
     }
     return 0;
}

int setBuffers(uint16_t mode){
     if(map_phys_vir_mem(mode, &firstBuffer) != 0) return 1;

     bufferSize = m_info.XResolution * m_info.YResolution * ((m_info.BitsPerPixel + 7) / 8); 
     secondBuffer = (uint8_t *) malloc(bufferSize);
     thirdBuffer = secondBuffer;

     return 0;
}

int flip_card(uint8_t card){
     uint8_t card_id = (card & (BIT(2) | BIT(1) | BIT(0))) + 1;
     
     int x = (card >> 6) & 0x03;
     int y = (card >> 4) & 0x03;
     int position = x + y*4;

     cards_flipped[position]=card_id;
     switch(card_id){
          case 1:
               backgroundFrames[position] = dino1; 
               break;
          case 2:
               backgroundFrames[position] = dino2; 
               break;
          case 3:
               backgroundFrames[position] = dino3; 
               break;
          case 4:
               backgroundFrames[position] = dino4;
               break; 
          case 5:
               backgroundFrames[position] = dino5; 
               break;
          case 6:
               backgroundFrames[position] = dino6; 
               break;
          case 7:
               backgroundFrames[position] = dino7; 
               break;
          case 8:
               backgroundFrames[position] = dino8;    
               break;
          default:
               return 1;                                     
     }
     return 0;
}

int unflip_card(uint8_t card){
     int x;
     int y;
     uint8_t bx = (card >> 6) & 0x03;
     uint8_t by = (card >> 4) & 0x03;
     if (bx == 0) x = 224;
     else if (bx == 1) x = 404;
     else if (bx == 2) x = 584;
     else x = 764;
     if (difState == EASY) {
          if (by == 0) y = 317;
          else if (by == 1) y = 497;
          else return 1;
     }
     else if (difState == MEDIUM) {
          if (by == 0) y = 137;
          else if (by == 1) y = 317;
          else if (by == 2) y = 497;
          else return 1;
     }
     else {
          if (by == 0) y = 137;
          else if (by == 1) y = 317;
          else if (by == 2) y = 497;
          else y = 677;
     }

     int pos = bx + 4 * by;
     backgroundFrames[pos] = back1;
     cards_flipped[pos] = 0;
     return 0;
}


uint32_t start_time;
uint32_t pause_time;
uint8_t pairs;
uint8_t pairs_completed;
int selected_position;
int first_card_position;
int gameTimer=0;

int game() {
     
     if (frameCounter == 3) {
          drawNewFrame();
          frameCounter = 0;
     }
     frameCounter++;
     if (menState == MENU_INGAME) {
          if (setupState == DO) {
               if (game_setup()) return 1;
               setupState = DONE;
          }
          if((timer_count-start_time)%60==0){
               gameTimer++;
          }
          if (gamePhase == FIRST_CHOICE) {
               if (cardSelected) {
                    uint64_t c = table >> (4*selected_position);
                    uint8_t type = (uint8_t)(c) & 0x0F;
                    if (type != 0) {
                         if (select_card(1)) return 1;
                         gamePhase = SECOND_CHOICE;
                         first_card_position = selected_position;
                         if (flip_card(card1)) return 1;
                    }
                    
                    cardSelected = false;
               }
          }
          else if (gamePhase == SECOND_CHOICE) {
               if (cardSelected && first_card_position != selected_position) {
                    uint64_t c = table >> (4*selected_position);
                    uint8_t type = (uint8_t)(c) & 0x0F;
                    if (type != 0) {
                         if (select_card(2)) return 1;
                         gamePhase = PAUSE;
                         pause_time = timer_count;
                         if (flip_card(card2)) return 1;
                    }
                    cardSelected = false;
               }               
          }
          else if (gamePhase == PAUSE) {
               if (timer_count - pause_time >= 50) gamePhase = AFTER_PAUSE;
          }
          else if (gamePhase == AFTER_PAUSE) {
               uint8_t cv1 = card1 & 0x07;
               uint8_t cv2 = card2 & 0x07;
               if (cv1 == cv2) {
                    uint8_t pos1 = card1 >> 4;
                    uint8_t cx = (pos1 >> 2) & 0x03;
                    uint8_t cy = pos1 & 0x03;
                    uint64_t deleted_card = 15;
                    deleted_card = deleted_card << ((cy * 4 + cx)*4);
                    uint64_t mask = ~deleted_card;
                    table = table & mask;


                    uint8_t pos2 = card2 >> 4;
                    cx = (pos2 >> 2) & 0x03;
                    cy = pos2 & 0x03;
                    deleted_card = 15;
                    deleted_card = deleted_card << ((cy * 4 + cx)*4);
                    mask = ~deleted_card;
                    table = table & mask;
                    pairs_completed++;
               }
               else {
                    if (unflip_card(card1)) return 1;
                    if (unflip_card(card2)) return 1;
               }
               gamePhase = FIRST_CHOICE;
               if (pairs_completed == pairs) {
                    frameState = NEW;
                    menState = MENU_END;
               }
               card1 = 0; card2 = 0;
          }
     }
    return 0;
}

int game_setup() {
    gamePhase = FIRST_CHOICE;
    selected_position = 0;
    frame_x_movment = 0;
    frame_y_movment = 0;
    for (int i = 0; i < 16; i++) cards_flipped[i] = 0;
    int cards[8] = {2, 2, 2, 2, 0, 0, 0, 0};
    table = 0;
    pairs_completed = 0;
    start_time = timer_count;
    gameTimer=0;
    setupDone=false;
    if (difState == EASY) {
        pairs = 4;
    }
    else if (difState == MEDIUM) {
        pairs = 6;
        cards[4] = 2;
        cards[5] = 2;
    }
    else {
        pairs = 8;
        cards[4] = 2;
        cards[5] = 2;
        cards[6] = 2;
        cards[7] = 2;
    }
    for (int i = 0; i < pairs*2; i++) {
        int s = rand() % pairs;
        while (cards[s] == 0) {
          s = rand() % pairs;
        }
        cards[s] = cards[s] - 1;
        uint64_t card = s + 8;
        card = card << i*4;
        table = table | card;
    }
    return 0;
}

int select_card(int n) {
     uint64_t c = table >> (4*selected_position);
     uint8_t pos = 0;
     uint8_t type = (uint8_t)(c) & 0x0F;

     if (3 < selected_position && selected_position < 8) 
          pos |= 0x10;
     else if (7 < selected_position && selected_position < 12) 
          pos |= 0x20;
     else if (11 < selected_position) 
          pos |= 0x30;

     if (selected_position == 1 || selected_position == 5 || selected_position == 9 || selected_position == 13) 
          pos |= 0x40;
     else if (selected_position == 2 || selected_position == 6 || selected_position == 10 || selected_position == 14) 
          pos |= 0x80;
     else if (selected_position == 3 || selected_position == 7 || selected_position == 11 || selected_position == 15) 
          pos |= 0xC0;
     if (n == 1) {
          if (type == 0) card1 = 0;     
          else card1 = type | pos;
     }
     else if (n == 2) {
          if (type == 0) card2 = 0;     
          else card2 = type | pos;
     }
     else return 1;
     return 0;
}

int display_time(int x,int y){
     uint32_t convertedTimeSeconds = (uint32_t)gameTimer;
     uint32_t seconds=convertedTimeSeconds%60;
     uint32_t minutes=convertedTimeSeconds/60;
     uint32_t secondsDigitUnit=seconds%10;
     uint32_t secondsDigitDozens=seconds/10;
     uint32_t minutesDigitUnit=minutes%10;
     uint32_t minutesDigitDozens=minutes/10;
     if(displayDigit(minutesDigitDozens,x,y)||
     displayDigit(minutesDigitUnit,x+50,y)
     ){
          return 1;
     }
     drawSpriteXpm(colon,x+100,y);
     if(displayDigit(secondsDigitDozens,x+150,y)||
     displayDigit(secondsDigitUnit,x+200,y)
     ){
          return 1;
     }
     return 0;
}

int displayDigit(uint32_t digit, int x , int y){
     switch(digit){
          case 0:
               drawSpriteXpm(digit0,x,y);
               break;
          case 1:
               drawSpriteXpm(digit1,x,y);
               break;
          case 2:
               drawSpriteXpm(digit2,x,y);
               break;
          case 3:
               drawSpriteXpm(digit3,x,y);
               break;
          case 4:
               drawSpriteXpm(digit4,x,y);
               break;
          case 5:
               drawSpriteXpm(digit5,x,y);
               break;
          case 6:
               drawSpriteXpm(digit6,x,y);
               break;
          case 7:
               drawSpriteXpm(digit7,x,y);  
               break;
          case 8:
               drawSpriteXpm(digit8,x,y);
               break;
          case 9:
               drawSpriteXpm(digit9,x,y);   
               break;
          default:
               return 1;                                             
     }
     return 0;
}


