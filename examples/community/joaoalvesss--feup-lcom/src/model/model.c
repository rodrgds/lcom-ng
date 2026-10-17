#include "model.h"

System sysState = RUNNING;
Menu menState = MENU_START;
Difficulty difState = MEDIUM;
Frame frameState = NEW;
Setup setupState = DO;
GamePhase gamePhase = FIRST_CHOICE;
bool cardSelected = false;
extern uint8_t scancode;
extern MouseHelper mouseHelper;
extern int mouse_ih_counter;
extern vbe_mode_info_t m_info;
int timer_count = 0;
extern int frame_x_movment;
extern uint8_t mouse_data;
extern int frame_y_movment;
extern int selected_position;

// sprites
Sprite *back1;
Sprite *back2;
Sprite *back3;
Sprite *dino1;
Sprite *dino2;
Sprite *dino3;
Sprite *dino4;
Sprite *dino5;
Sprite *dino6;
Sprite *dino7;
Sprite *dino8;
Sprite *colon;
Sprite *digit0;
Sprite *digit1;
Sprite *digit2;
Sprite *digit3;
Sprite *digit4;
Sprite *digit5;
Sprite *digit6;
Sprite *digit7;
Sprite *digit8;
Sprite *digit9;
Sprite *mainmenu;
Sprite *menufinal;
Sprite *background;
Sprite *dino_mouse;
Sprite *frame;
Sprite *dino_cursor;

int updateTimerState(){
     timer_count++;
     if(flipBuffers() != 0) return 1;
     return 0;
}

void updateKeyboardState(){
     keyboard_mouse_read_output(0, KBC_OUT_CMD, &scancode);
     if(menState == MENU_START){
          if(scancode == PRESSED_E){
               difState = EASY;
               menState = MENU_INGAME;
               frameState = NEW;
               setupState = DO;
          }
          else if(scancode == PRESSED_M){
               difState = MEDIUM;
               menState = MENU_INGAME;
               frameState = NEW;
               setupState = DO;
          }
          else if(scancode == PRESSED_H){
               difState = HARD;
               menState = MENU_INGAME;
               frameState = NEW;
               setupState = DO;
          }
          else if(scancode == PRESSED_Q){
               sysState = EXIT;
          }
     }
     if(menState == MENU_INGAME){
          if (scancode == PRESSED_ENTER) {
                if (gamePhase == FIRST_CHOICE || gamePhase == SECOND_CHOICE) cardSelected = true;
          }
          else if(scancode == PRESSED_B){
               menState = MENU_START;
               frameState = NEW;
          }
          else if(scancode == PRESSED_Q){
               sysState = EXIT;
          }
          else if(scancode == PRESSED_D){
               if((frame_x_movment + 180) <= 540){
                    frame_x_movment += 180;
                    selected_position += 1;
                    frameState = NEW;
               }
          }
          else if(scancode == PRESSED_A){
               if((frame_x_movment - 180) >= 0){
                    frame_x_movment -= 180;
                    selected_position -= 1;
                    frameState = NEW;
               }
          }
          else if(scancode == PRESSED_W){
               if(difState == EASY){
                    if((frame_y_movment - 180) >= 0){
                        frame_y_movment -= 180;
                        selected_position -= 4;
                        frameState = NEW;
                    }
               }
               else if(difState == MEDIUM){
                    if((frame_y_movment - 180) > -180){
                        frame_y_movment -= 180;
                        selected_position -= 4;
                        frameState = NEW;
                    }
               }
               else if(difState == HARD){
                    if((frame_y_movment - 180) > -180){
                        frame_y_movment -= 180;
                        selected_position -= 4;
                        frameState = NEW;
                    }
               }
          }
          else if(scancode == PRESSED_S){
               if(difState == EASY){
                    if((frame_y_movment + 180) <= 310){
                        frame_y_movment += 180;
                        selected_position += 4;
                        frameState = NEW;
                    }
               }
               else if(difState == MEDIUM){
                    if((frame_y_movment + 180) <= 490){
                        frame_y_movment += 180;
                        selected_position += 4;
                        frameState = NEW;
                    }
               }
               else if(difState == HARD){
                    if((frame_y_movment + 180) <= 700){
                        frame_y_movment += 180;
                        selected_position += 4;
                        frameState = NEW;
                    }
               }
          }
     }
     if(menState == MENU_END){
          if(scancode == PRESSED_M){
               menState = MENU_START;
               frameState = NEW;
          }
          else if(scancode == PRESSED_P){
               menState = MENU_INGAME;
               frameState = NEW;
               setupState = DO;
          }
          else if(scancode == PRESSED_Q){
               sysState = EXIT;
          }
     }
}

void updateMouseState(){
     keyboard_mouse_read_output(1, KBC_OUT_CMD, &mouse_data);
     assemble_packet();
     if(mouse_ih_counter == 3){
          parse_packet();
          updateStates();
          frameState = NEW;
          mouse_ih_counter = 0;
     }
}

void updateStates(){
     if(menState == MENU_START && mouseHelper.click_left_button){
          if(mouseHelper.x_pos > 493 && mouseHelper.x_pos < 659 && mouseHelper.y_pos > 430 && mouseHelper.y_pos < 473){
               difState = EASY;
               menState = MENU_INGAME;
               frameState = NEW;
               setupState = DO;
          }
          else if(mouseHelper.x_pos > 467 && mouseHelper.x_pos < 684 && mouseHelper.y_pos > 504 && mouseHelper.y_pos < 546){
               difState = MEDIUM;
               menState = MENU_INGAME;
               frameState = NEW;
               setupState = DO;
          }
          else if(mouseHelper.x_pos > 495 && mouseHelper.x_pos < 661 && mouseHelper.y_pos > 577 && mouseHelper.y_pos < 618){
               difState = HARD;
               menState = MENU_INGAME;
               frameState = NEW;
               setupState = DO;
          }
          if(mouseHelper.x_pos > 489 && mouseHelper.x_pos < 665 && mouseHelper.y_pos > 713 && mouseHelper.y_pos < 756){
               sysState = EXIT;
          }
     }

     if(menState == MENU_INGAME && (mouseHelper.click_middle_button || mouseHelper.click_right_button)){
          menState = MENU_START;
     }

     if(menState == MENU_END && (mouseHelper.x_pos > 413 && mouseHelper.x_pos < 739 && mouseHelper.y_pos > 508 && mouseHelper.y_pos < 557) && mouseHelper.click_left_button){
          menState = MENU_INGAME;
          frameState = NEW;
          setupState = DO;
     }
     else if(menState == MENU_END && (mouseHelper.x_pos > 420 && mouseHelper.x_pos < 734 && mouseHelper.y_pos > 642 && mouseHelper.y_pos < 704) && mouseHelper.click_left_button){
          menState = MENU_START;
     }
     
     else if (menState == MENU_INGAME && (mouseHelper.click_left_button) && (difState == EASY) && (
               (mouseHelper.x_pos >= 224 && mouseHelper.x_pos <= 374 && mouseHelper.y_pos >= 317 && mouseHelper.y_pos <= 467) ||
               (mouseHelper.x_pos >= 404 && mouseHelper.x_pos <= 554 && mouseHelper.y_pos >= 317 && mouseHelper.y_pos <= 467) ||
               (mouseHelper.x_pos >= 584 && mouseHelper.x_pos <= 734 && mouseHelper.y_pos >= 317 && mouseHelper.y_pos <= 467) ||
               (mouseHelper.x_pos >= 764 && mouseHelper.x_pos <= 914 && mouseHelper.y_pos >= 317 && mouseHelper.y_pos <= 467) ||
               (mouseHelper.x_pos >= 224 && mouseHelper.x_pos <= 374 && mouseHelper.y_pos >= 497 && mouseHelper.y_pos <= 647) ||
               (mouseHelper.x_pos >= 404 && mouseHelper.x_pos <= 554 && mouseHelper.y_pos >= 497 && mouseHelper.y_pos <= 647) ||
               (mouseHelper.x_pos >= 584 && mouseHelper.x_pos <= 734 && mouseHelper.y_pos >= 497 && mouseHelper.y_pos <= 647) ||
               (mouseHelper.x_pos >= 764 && mouseHelper.x_pos <= 914 && mouseHelper.y_pos >= 497 && mouseHelper.y_pos <= 647)
               )) {
               if (gamePhase == FIRST_CHOICE || gamePhase == SECOND_CHOICE){
                    cardSelected = true;
                    int x = (mouseHelper.x_pos - 224) / 180;
                    int y = (mouseHelper.y_pos - 317) / 180;
                    selected_position = x + 4 * y;  
                    frame_x_movment = x * 180;
                    frame_y_movment = y * 180;
                    frameState = NEW;            
               }
          }
     else if (menState == MENU_INGAME && (mouseHelper.click_left_button) && (difState == MEDIUM) && (
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
               if (gamePhase == FIRST_CHOICE || gamePhase == SECOND_CHOICE){
                     cardSelected = true;
                    int x = (mouseHelper.x_pos - 224) / 180;
                    int y = (mouseHelper.y_pos - 137) / 180;
                    selected_position = x + 4 * y;
                    frame_x_movment = x * 180;
                    frame_y_movment = y * 180;
                    frameState = NEW;     
               }  
          }
     else if (menState == MENU_INGAME && (mouseHelper.click_left_button) && (difState == HARD) && (
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
               if (gamePhase == FIRST_CHOICE || gamePhase == SECOND_CHOICE){
                     cardSelected = true;
                    int x = (mouseHelper.x_pos - 224) / 180;
                    int y = (mouseHelper.y_pos - 137) / 180;
                    selected_position = x + 4 * y;
                    frame_x_movment = x * 180;
                    frame_y_movment = y * 180;
                    frameState = NEW;     
               }    
          }
}

void setupSprites(){
     back1 = createSprite((xpm_map_t)back1_xpm);
     back2 = createSprite((xpm_map_t)back2_xpm);
     back3 = createSprite((xpm_map_t)back3_xpm);
     dino1 = createSprite((xpm_map_t)dino1_xpm);
     dino2 = createSprite((xpm_map_t)dino2_xpm);
     dino3 = createSprite((xpm_map_t)dino3_xpm);
     dino4 = createSprite((xpm_map_t)dino4_xpm);
     dino5 = createSprite((xpm_map_t)dino5_xpm);
     dino6 = createSprite((xpm_map_t)dino6_xpm);
     dino7 = createSprite((xpm_map_t)dino7_xpm);
     dino8 = createSprite((xpm_map_t)dino8_xpm);
     colon = createSprite((xpm_map_t) colon_xpm);
     digit0 = createSprite((xpm_map_t) digit0_xpm);
     digit1 = createSprite((xpm_map_t) digit1_xpm);
     digit2 = createSprite((xpm_map_t) digit2_xpm);
     digit3 = createSprite((xpm_map_t) digit3_xpm);
     digit4 = createSprite((xpm_map_t) digit4_xpm);
     digit5 = createSprite((xpm_map_t) digit5_xpm);
     digit6 = createSprite((xpm_map_t) digit6_xpm);
     digit7 = createSprite((xpm_map_t) digit7_xpm);
     digit8 = createSprite((xpm_map_t) digit8_xpm);
     digit9 = createSprite((xpm_map_t) digit9_xpm);
     mainmenu = createSprite((xpm_map_t)mainmenu_xpm);
     menufinal = createSprite((xpm_map_t)menufinal_xpm);
     background = createSprite((xpm_map_t)background_xpm);
     dino_mouse = createSprite((xpm_map_t)dino_mouse_xpm);
     frame = createSprite((xpm_map_t)frame_xpm);
     dino_cursor = createSprite((xpm_map_t)dino_cursor_xpm);
}

void destroySprites(){
     destroySprite(back1);
     destroySprite(back2);
     destroySprite(back3);
     destroySprite(dino1);
     destroySprite(dino2);
     destroySprite(dino3);
     destroySprite(dino4);
     destroySprite(dino5);
     destroySprite(dino6);
     destroySprite(dino7);
     destroySprite(dino8);
     destroySprite(colon);
     destroySprite(digit0);
     destroySprite(digit1);
     destroySprite(digit2);
     destroySprite(digit3);
     destroySprite(digit4);
     destroySprite(digit5);
     destroySprite(digit6);
     destroySprite(digit7);
     destroySprite(digit8);
     destroySprite(digit9);
     destroySprite(mainmenu);
     destroySprite(menufinal);
     destroySprite(dino_mouse);
     destroySprite(frame);
     destroySprite(dino_cursor);
}

