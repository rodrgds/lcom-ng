// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>
#include <lcom/liblm.h>
#include <lcom/proj.h>

#include <stdbool.h>
#include <stdint.h>
#include <time.h>
#include <stdlib.h>

#include "video_gr.h"
#include "mouse.h"
#include "timer.h"
#include "keyboard.h"
#include "xpmHandler.h"
#include "RTC.h"
#include "highscores.h"
#include "../xpm/elementos/elementos.h"
#include "../xpm/abecedario/abecedario.h"
#include "../xpm/numeros/numeros.h"

extern uint8_t byte;
extern uint8_t key_scancode;
extern int xCursor, yCursor;
extern int global_counter;
extern Record records[3];
extern int nrRecords;

int start(){

  //loadRecords();
  createXPMs();
  
  //Initialize mode 115
  if (vg_init(0x115) == NULL){
      printf("Couldn't initialize mode 105!\n");
      return 1;
  }

  //Load xpm
  xpm_image_t img_background;
  xpm_load(roosterGame, XPM_8_8_8, &img_background);

  //Draws bakckground
  vg_draw_img(img_background, 0, 0);
  printDate();

  doubleBuffering();

  return 0;
}

int mainMenu(){

    /*---VARIABLES DECLARATION---*/
    //Menu variables
    int option = 0;
    bool exit = false;

    //Cursor variables
    xCursor=0, yCursor=0;

    //Mouse subscription variables
    int ipc_status, r;
    message msg;
    struct packet pp;
    unsigned int read_bytes = 0;
    uint8_t mouse_hookID = 0;
    uint32_t mouse_irq = BIT(mouse_hookID);

    /*---MOUSE---*/
    //Subscribe mouse interruptions
    if (mouse_subscribe_int(&mouse_hookID) == 1){
        printf("Mouse subscription failed!\n");
        return 1;
    }

    //Enable mouse data reporting
    enableDataReporting();

    /*---XPM---*/
    //Load xpm
    xpm_image_t img_menu;
    xpm_image_t img_cursor;

    xpm_load(menu, XPM_8_8_8, &img_menu);
    xpm_load(cursor, XPM_8_8_8, &img_cursor);
    //Draw menu and cursor(initial position);
    vg_draw_img(img_menu, 0, 0);
    vg_draw_img(img_cursor, xCursor, yCursor);

    //Double buffering
    doubleBuffering();


    /*---MAIN LOOP---*/

    while (!exit){
        /*Get a request message.*/
        if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0){
            printf("driver_receive failed with: %d", r);
            continue;
        }
        if (is_ipc_notify(ipc_status)){ /*received notification*/
            switch (_ENDPOINT_P(msg.m_source)){
            case HARDWARE: /*hardware interrupt notification*/
                if (msg.m_notify.interrupts & mouse_irq){ /*subscribed interrupt*/
                    mouse_ih();

                    if ((read_bytes == 0) && ((byte & BIT(3)) == 0)){
                        continue;
                    }
                    else
                    {
                        if (read_bytes == 0 || read_bytes == 1 || read_bytes == 2){
                            pp.bytes[read_bytes] = byte;
                            read_bytes++;
                        }
                        if (read_bytes == 3){
                            mouse_process_packets(&pp);
                            read_bytes = 0;

                            //xCursor and yCursor stores mouse coordinates
                            update_mouse_position(pp.delta_x, pp.delta_y);
                            //Redraws menu and cursor
                            vg_draw_img(img_menu, 0,0);
                            vg_draw_img(img_cursor, xCursor,yCursor);
                            //Double buffering
                            doubleBuffering();

                            /*---MENU OPTIONS---*/
                            //SinglePlayer
                            if (pp.lb && xCursor>=248 && xCursor<=550 && yCursor>=156 && yCursor<=233){
                                option = 1;
                                exit = true;
                            }
                            //Multiplayer
                            else if(pp.lb && xCursor>=248 && xCursor<=550 && yCursor>=264 && yCursor<=341){
                                option = 2;
                                exit=true;
                            }
                            //Highscores
                            else if(pp.lb && xCursor>=248 && xCursor<=550 && yCursor>=370 && yCursor<=446){
                                option = 3;
                                exit=true;
                            }
                            //Exit
                            else if(pp.lb && xCursor>=248 && xCursor<=550 && yCursor>=480 && yCursor<=556){
                                option = 4;
                                exit=true;
                            }
                        }
                    }
                }
                break;
            default:
                break; /*no other notifications expected: do nothing*/
            }
        }
        else
        { /*received a standard message, not a notification*/
            /*no standard messages expected: do nothing*/
        }
    }

    /*---MOUSE---*/

    //Disables mouse data reporting
    disableDataReporting();

    //Unsubscribe mouse interruptions
    if (mouse_unsubscribe_int() == 1){
        printf("Mouse unsubscription failed!\n");
        return 1;
    }

    return option;
}

bool endGame(int arr[],int x){
  // X == 0, Panda Wins
  if(x == 0){
    if(arr[0] == 1 && arr[1] == 1 && arr[2] == 1) return true;
    else if(arr[3] == 1 && arr[4] == 1 && arr[5] == 1) return true;
    else if(arr[6] == 1 && arr[7] == 1 && arr[8] == 1) return true;
    else if(arr[0] == 1 && arr[3] == 1 && arr[6] == 1) return true;
    else if(arr[1] == 1 && arr[4] == 1 && arr[7] == 1) return true;
    else if(arr[2] == 1 && arr[5] == 1 && arr[8] == 1) return true;
    else if(arr[0] == 1 && arr[4] == 1 && arr[8] == 1) return true;
    else if(arr[2] == 1 && arr[4] == 1 && arr[6] == 1) return true;
    else return false;
  }
  // X == 1, Tigre wins
  else if(x == 1){
    if(arr[0] == 2 && arr[1] == 2 && arr[2] == 2) return true;
    else if(arr[3] == 2 && arr[4] == 2 && arr[5] == 2) return true;
    else if(arr[6] == 2 && arr[7] == 2 && arr[8] == 2) return true;
    else if(arr[0] == 2 && arr[3] == 2 && arr[6] == 2) return true;
    else if(arr[1] == 2 && arr[4] == 2 && arr[7] == 2) return true;
    else if(arr[2] == 2 && arr[5] == 2 && arr[8] == 2) return true;
    else if(arr[0] == 2 && arr[4] == 2 && arr[8] == 2) return true;
    else if(arr[2] == 2 && arr[4] == 2 && arr[6] == 2) return true;
    else return false;
  }
  // EMPATE
  else{
    if((arr[0] == 1 ||  arr[0] == 2) && (arr[1] == 1 ||  arr[1] == 2) &&
      (arr[2] == 1 ||  arr[2] == 2) && (arr[3] == 1 ||  arr[3] == 2) &&
      (arr[4] == 1 ||  arr[4] == 2) && (arr[5] == 1 ||  arr[5] == 2) &&
      (arr[6] == 1 ||  arr[6] == 2) && (arr[7] == 1 ||  arr[7] == 2) &&
      (arr[8] == 1 ||  arr[8] == 2)) return true;
    else return false;
  }
}

int pc_play(int arr[]){
  int r;
  //Check possibility of tiger making a horizontal line
  if((arr[0] == 2 && arr[1] == 2 && arr[2] == 0)){
    r = 2;
  }
  else if((arr[1] == 2 && arr[2] == 2 && arr[0] == 0)){
    r = 0;
  }
  else if((arr[0] == 2 && arr[2] == 2 && arr[1] == 0) ){
    r = 1;
  }
  else if((arr[3] == 2 && arr[4] == 2 && arr[5] == 0)){
    r = 5;
  }
  else if((arr[4] == 2 && arr[5] == 2 && arr[3] == 0)){
    r = 3;
  }
  else if((arr[3] == 2 && arr[5] == 2 && arr[4] == 0)){
    r = 4;
  }
  else if((arr[6] == 2 && arr[7] == 2 && arr[8] == 0)){
    r = 8;
  }
  else if((arr[8] == 2 && arr[7] == 2 && arr[6] == 0)){
    r = 6;
  }
  else if((arr[6] == 2 && arr[8] == 2 && arr[7] == 0)){
    r = 7;
  }
  //Check possibility of tiger making a vertical line
  else if((arr[0] == 2 && arr[3] == 2 && arr[6] == 0)){
    r = 6;
  }
  else if((arr[6] == 2 && arr[3] == 2 && arr[0] == 0)){
    r = 0;
  }
  else if((arr[0] == 2 && arr[6] == 2 && arr[3] == 0)){
    r = 3;
  }
  else if((arr[1] == 2 && arr[4] == 2 && arr[7] == 0)){
    r = 7;
  }
  else if((arr[4] == 2 && arr[7] == 2 && arr[1] == 0)){
    r = 1;
  }
  else if((arr[1] == 2 && arr[7] == 2 && arr[4] == 0)){
    r = 4;
  }
  else if((arr[2] == 2 && arr[5] == 2 && arr[8] == 0)){
    r = 8;
  }
  else if((arr[5] == 2 && arr[8] == 2 && arr[2] == 0)){
    r = 2;
  }
  else if((arr[2] == 2 && arr[8] == 2 && arr[8] == 0)){
    r = 5;
  }
  //Check possibility of tiger making a diagonal line
  else if((arr[2] == 2 && arr[4] == 2 && arr[6] == 0)){
    r = 6;
  }
  else if((arr[6] == 2 && arr[4] == 2 && arr[2] == 0)){
    r = 2;
  }
  else if((arr[0] == 2 && arr[4] == 2 && arr[8] == 0)){
    r = 8;
  }
  else if((arr[8] == 2 && arr[4] == 2 && arr[0] == 0)){
    r = 0;
  }
  else if((arr[8] == 2 && arr[0] == 2 && arr[4] == 0)){
    r = 4;
  }
  else if((arr[2] == 2 && arr[6] == 2 && arr[4] == 0)){
    r = 4;
  }
  //Check possibility of panda making a horizontal line
  else if((arr[0] == 1 && arr[1] == 1 && arr[2] == 0)){
    r = 2;
  }
  else if((arr[1] == 1 && arr[2] == 1 && arr[0] == 0)){
    r = 0;
  }
  else if((arr[0] == 1 && arr[2] == 1 && arr[1] == 0)){
    r = 1;
  }
  else if((arr[3] == 1 && arr[4] == 1 && arr[5] == 0)){
    r = 5;
  }
  else if((arr[4] == 1 && arr[5] == 1 && arr[3] == 0)){
    r = 3;
  }
  else if((arr[3] == 1 && arr[5] == 1 && arr[4] == 0)){
    r = 4;
  }
  else if((arr[6] == 1 && arr[7] == 1 && arr[8] == 0)){
    r = 8;
  }
  else if((arr[8] == 1 && arr[7] == 1 && arr[6] == 0)){
    r = 6;
  }
  else if((arr[6] == 1 && arr[8] == 1 && arr[7] == 0)){
    r = 7;
  }
  //Check possibility of panda making a vertical line
  else if((arr[0] == 1 && arr[3] == 1 && arr[6] == 0)){
    r = 6;
  }
  else if((arr[6] == 1 && arr[3] == 1 && arr[0] == 0)){
    r = 0;
  }
  else if((arr[0] == 1 && arr[6] == 1 && arr[3] == 0)){
    r = 3;
  }
  else if((arr[1] == 1 && arr[4] == 1 && arr[7] == 0)){
    r = 7;
  }
  else if((arr[4] == 1 && arr[7] == 1 && arr[1] == 0)){
    r = 1;
  }
  else if((arr[1] == 1 && arr[7] == 1 && arr[4] == 0)){
    r = 4;
  }
  else if((arr[2] == 1 && arr[5] == 1 && arr[8] == 0)){
    r = 8;
  }
  else if((arr[5] == 1 && arr[8] == 1 && arr[2] == 0)){
    r = 2;
  }
  else if((arr[2] == 1 && arr[8] == 1 && arr[5] == 0)){
    r = 5;
  }
  //Check possibility of panda making a diagonal line
  else if((arr[2] == 1 && arr[4] == 1 && arr[6] == 0)){
    r = 6;
  }
  else if((arr[6] == 1 && arr[4] == 1 && arr[2] == 0)){
    r = 2;
  }
  else if((arr[0] == 1 && arr[4] == 1 && arr[8] == 0)){
    r = 8;
  }
  else if((arr[8] == 1 && arr[4] == 1 && arr[0] == 0)){
    r = 0;
  }
  else if((arr[0] == 1 && arr[8] == 1 && arr[4] == 0)){
    r = 4;
  }
  else if((arr[2] == 1 && arr[6] == 1 && arr[4] == 0)){
    r = 4;
  }
  else{
    srand(time(NULL));
    r = rand() % 9;
    bool flag = false;

    while(!flag){
      if(arr[r] == 1 || arr[r] == 2){
        r = rand() % 9;
      }
      else{
        flag = true;
      }
    }
  }
  return r;
}

int draw_images_getX(int arr[],int i){
  int x;
  if(i == 0 || i == 3 || i == 6){
    x = 60;
  }
  if(i == 1 || i == 4 || i == 7){
    x = 252;
  }
  if(i == 2 || i == 5 || i == 8){
    x = 444;
  }
  return x;
}

int draw_images_getY(int arr[],int i){
  int y;
  if(i == 0 || i == 1 || i == 2){
    y = 25;
  }
  if(i == 3 || i == 4 || i == 5){
    y = 221;
  }
  if(i == 6 || i == 7 || i == 8){
    y = 413;
  }
  return y;
}

bool isOccupied(int board_positions[], int i){
  if(board_positions[i] != 0)
    return true;
  else return false;
}

int gameOver(int positions[], int time){

  //records variables
  char name[10];
  //int nameSize=0;
  char jogador[] = "Jogador ";
  
  strcpy(name, jogador);
  char pos[2];
  int position = nrRecords+1;
  sprintf(pos, "%d", position);
  strcat(name, pos);


  //keyboard variables
 /* int ipc_status, r;
  message msg;
  uint8_t kbd_hookID = 0;
  uint8_t irq_set = BIT(kbd_hookID);*/

  //Verify if user wins
  if(!endGame(positions,0))
    return 1;
  else if(!verifyIsRecord(time)){
      return 1;
  }

  // if (kbc_subscribe_int(&kbd_hookID) != OK) {
  //   printf("Keyboard controler subscription failed!\n");
  //   return 1;
  // }

  // while (key_scancode != ENTER_KEY) { /*You may want to use a different condition*/
  //   /*Get a request message.*/
  //   if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
  //     printf("driver_receive failed with: %d", r);
  //     continue;
  //   }
  //   if (is_ipc_notify(ipc_status)) { /*received notification*/
  //     switch (_ENDPOINT_P(msg.m_source)) {
  //       case HARDWARE: /*hardware interrupt notification*/
  //         if (msg.m_notify.interrupts & irq_set) { /*subscribed interrupt*/
  //           kbc_ih();
  //           if(key_scancode==0){
  //             printf("Invalid scancode\n");
  //           }
  //           else {
  //             processKeyScancode(key_scancode);
  //             addRecordName(name,nameSize, key_scancode);
  //           }
  //         }
  //         break;
  //       default:
  //         break; /*no other notifications expected: do nothing*/
  //     }
  //   }
  //   else{ /*received a standard message, not a notification*/
  //     /*no standard messages expected: do nothing*/
  //   }
  // }

  // if(kbc_unsubscribe_int()==1){
  //   printf("kbc subscription failed\n");
  //   return 1;
  // }

  addRecord(name, time);

  return 0;
}

int singlePlayer(){

  /*---VARIABLES DECLARATION---*/
  //Cursor variables
  xCursor=0, yCursor=0;

  //XPM variables
  xpm_image_t img_board;
  xpm_image_t img_cursor;
  xpm_image_t img_panda;
  xpm_image_t img_tigre;
  xpm_image_t panda_sp;
  xpm_image_t tiger_sp;
  xpm_image_t tie_img;

  //Game variables
  int board_positions[9] = {0};
  bool exit = false;
  bool pc = false;

  int time=0;

  //Mouse subscription variables
  int ipc_status, r;
  message msg;
  struct packet pp;
  unsigned int read_bytes = 0;
  uint8_t mouse_hookID = 0;
  uint32_t mouse_irq = BIT(mouse_hookID);

  //Timer variables
  uint8_t timer_hookID = 1;
  uint32_t timer_irq = BIT(timer_hookID);

  //Keyboard variables
  uint8_t kbd_hookID = 2;
  uint32_t kbd_irq = BIT(kbd_hookID);


  /*---TIMER---*/
  //Subscribe timer interruptions
  if(timer_subscribe_int(&timer_hookID)==1){
    printf("Timer subscription failed!\n");
    return 1;
  }

  /*---KEYBOARD---*/
  if(kbc_subscribe_int(&kbd_hookID) ==1){
    return 1;
  }

  /*---MOUSE---*/
  //Subscribe mouse interruptions
  if (mouse_subscribe_int(&mouse_hookID) == 1){
      printf("Mouse subscription failed!\n");
      return 1;
  }
  //Enable mouse data reporting
  enableDataReporting();


  /*---XPM---*/
  //Load xpm
  xpm_load(boardNrs, XPM_8_8_8, &img_board);
  xpm_load(cursor, XPM_8_8_8, &img_cursor);
  xpm_load(panda, XPM_8_8_8, &img_panda);
  xpm_load(tigre, XPM_8_8_8, &img_tigre);
  xpm_load(panda_single, XPM_8_8_8, &panda_sp);
  xpm_load(tiger_single, XPM_8_8_8, &tiger_sp);
  xpm_load(tie, XPM_8_8_8, &tie_img);
  
  createXPMs();

  //Draws bakckground and cursor(initial position)
  vg_draw_img(img_board, 0, 0);
  vg_draw_img(img_cursor, xCursor,yCursor);
  //Double buffering
  doubleBuffering();

  /*---MAIN LOOP---*/
  while (!exit){
      /*Get a request message.*/
      if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0){
          printf("driver_receive failed with: %d", r);
          continue;
      }
      if (is_ipc_notify(ipc_status)){ /*received notification*/
          switch (_ENDPOINT_P(msg.m_source)){
          case HARDWARE: /*hardware interrupt notification*/
              if (msg.m_notify.interrupts & mouse_irq){ /*subscribed mouse interrupt*/
                mouse_ih();
                if ((read_bytes == 0) && ((byte & BIT(3)) == 0)){
                    continue;
                }
                else{
                  if (read_bytes == 0 || read_bytes == 1 || read_bytes == 2){
                      pp.bytes[read_bytes] = byte;
                      read_bytes++;
                  }
                  if (read_bytes == 3){
                    mouse_process_packets(&pp);
                    read_bytes = 0;
                    //xCursor and yCursor stores mouse coordinates
                    update_mouse_position(pp.delta_x, pp.delta_y);

                    /*PLAYERS MOVES*/
                    if(!pc){
                      //1st row
                      if (pp.lb && xCursor>=43 && xCursor<=230 && yCursor>=12 && yCursor<=200){
                        if(!isOccupied(board_positions,0)){
                          board_positions[0]=1;
                          vg_draw_img(img_panda, 60,25);
                          vg_draw_img(img_cursor, xCursor,yCursor);
                          doubleBuffering();
                          pc = true;
                        }
                      }
                      if (pp.lb && xCursor>=240 && xCursor<=423 && yCursor>=12 && yCursor<=200){
                        if(!isOccupied(board_positions,1)){
                          board_positions[1]=1;
                          vg_draw_img(img_panda, 252,25);
                          vg_draw_img(img_cursor, xCursor,yCursor);
                          doubleBuffering();
                          pc = true;
                        }
                      }
                      if (pp.lb && xCursor>=432 && xCursor<=619 && yCursor>=12 && yCursor<=200){
                        if(!isOccupied(board_positions,2)){
                          board_positions[2]=1;
                          vg_draw_img(img_panda, 444,25);
                          vg_draw_img(img_cursor, xCursor,yCursor);
                          doubleBuffering();
                          pc = true;
                        }
                      }
                      //2nd row
                      if (pp.lb && xCursor>=43 && xCursor<=230 && yCursor>=208 && yCursor<=390){
                        if(!isOccupied(board_positions,3)){
                          board_positions[3]=1;
                          vg_draw_img(img_panda, 60,221);
                          vg_draw_img(img_cursor, xCursor,yCursor);
                          doubleBuffering();
                          pc = true;
                        }
                      }
                      if (pp.lb && xCursor>=240 && xCursor<=423 && yCursor>=208 && yCursor<=390){
                        if(!isOccupied(board_positions,4)){
                          board_positions[4]=1;
                          vg_draw_img(img_panda, 252,221);
                          vg_draw_img(img_cursor, xCursor,yCursor);
                          doubleBuffering();
                          pc = true;
                        }
                      }
                      if (pp.lb && xCursor>=432 && xCursor<=619 && yCursor>=208 && yCursor<=390){
                        if(!isOccupied(board_positions,5)){
                          board_positions[5]=1;
                          vg_draw_img(img_panda, 444,221);
                          vg_draw_img(img_cursor, xCursor,yCursor);
                          doubleBuffering();
                          pc = true;
                        }
                      }
                      //3rd row
                      if (pp.lb && xCursor>=43 && xCursor<=230 && yCursor>=400 && yCursor<=588){
                        if(!isOccupied(board_positions,6)){
                          board_positions[6]=1;
                          vg_draw_img(img_panda, 60,413);
                          vg_draw_img(img_cursor, xCursor,yCursor);
                          doubleBuffering();
                          pc = true;
                        }
                      }
                      if (pp.lb && xCursor>=240 && xCursor<=423 && yCursor>=400 && yCursor<=588){
                        if(!isOccupied(board_positions,7)){
                          board_positions[7]=1;
                          vg_draw_img(img_panda, 252,413);
                          vg_draw_img(img_cursor, xCursor,yCursor);
                          doubleBuffering();
                          pc = true;
                        }
                      }
                      if (pp.lb && xCursor>=432 && xCursor<=619 && yCursor>=400 && yCursor<=588){
                        if(!isOccupied(board_positions,8)){
                          board_positions[8]=1;
                          vg_draw_img(img_panda, 444,413);
                          vg_draw_img(img_cursor, xCursor,yCursor);
                          doubleBuffering();
                          pc = true;
                        }
                      }
                    }
                    else{
                      int i = pc_play(board_positions);
                      board_positions[i] = 2;
                      int x = draw_images_getX(board_positions,i);
                      int y = draw_images_getY(board_positions,i);
                      vg_draw_img(img_tigre,x,y);
                      vg_draw_img(img_cursor, xCursor,yCursor);
                      //Double buffering
                      doubleBuffering();
                      pc = false;
                    }
                    if(endGame(board_positions,0) || endGame(board_positions,1) || endGame(board_positions,2)){
                      time=(global_counter%3600)/60;
                      gameOver(board_positions,time);
                      global_counter = 0;
                      if(endGame(board_positions, 0))
                        vg_draw_img(panda_sp, 0, 0);                          
                      if(endGame(board_positions,1))
                        vg_draw_img(tiger_sp, 0, 0);
                      if(endGame(board_positions, 2))
                        vg_draw_img(tie_img, 0, 0);
                      doubleBuffering();
                      sleep(3);
                      exit = true;
                    }
                  }
                }
              }
              if (msg.m_notify.interrupts & kbd_irq){ /*subscribed interrupt*/
                kbc_ih();
                if(key_scancode==0){
                  printf("Invalid scancode\n");
                }
                else {
                  processKeyScancode(key_scancode);
                  /*PLAYERS MOVES*/
                  if(!pc){
                    //1st row
                    if(key_scancode==ONE_KEY){
                      if(!isOccupied(board_positions,0)){
                        board_positions[0]=1;
                        vg_draw_img(img_panda, 60,25);
                        vg_draw_img(img_cursor, xCursor,yCursor);
                        doubleBuffering();
                        pc = true;
                      }
                    }
                    if (key_scancode==TWO_KEY){
                      if(!isOccupied(board_positions,1)){
                        board_positions[1]=1;
                        vg_draw_img(img_panda, 252,25);
                        vg_draw_img(img_cursor, xCursor,yCursor);
                        doubleBuffering();
                        pc = true;
                      }
                    }
                    if (key_scancode==THREE_KEY){
                      if(!isOccupied(board_positions,2)){
                        board_positions[2]=1;
                        vg_draw_img(img_panda, 444,25);
                        vg_draw_img(img_cursor, xCursor,yCursor);
                        doubleBuffering();
                        pc = true;
                      }
                    }
                    //2nd row
                    if (key_scancode==FOUR_KEY){
                      if(!isOccupied(board_positions,3)){
                        board_positions[3]=1;
                        vg_draw_img(img_panda, 60,221);
                        vg_draw_img(img_cursor, xCursor,yCursor);
                        doubleBuffering();
                        pc = true;
                      }
                    }
                    if (key_scancode==FIVE_KEY){
                      if(!isOccupied(board_positions,4)){
                        board_positions[4]=1;
                        vg_draw_img(img_panda, 252,221);
                        vg_draw_img(img_cursor, xCursor,yCursor);
                        doubleBuffering();
                        pc = true;
                      }
                    }
                    if (key_scancode==SIX_KEY){
                      if(!isOccupied(board_positions,5)){
                        board_positions[5]=1;
                        vg_draw_img(img_panda, 444,221);
                        vg_draw_img(img_cursor, xCursor,yCursor);
                        doubleBuffering();
                        pc = true;
                      }
                    }
                    //3rd row
                    if (key_scancode==SEVEN_KEY){
                      if(!isOccupied(board_positions,6)){
                        board_positions[6]=1;
                        vg_draw_img(img_panda, 60,413);
                        vg_draw_img(img_cursor, xCursor,yCursor);
                        doubleBuffering();
                        pc = true;
                      }
                    }
                    if (key_scancode==EIGHT_KEY){
                      if(!isOccupied(board_positions,7)){
                        board_positions[7]=1;
                        vg_draw_img(img_panda, 252,413);
                        vg_draw_img(img_cursor, xCursor,yCursor);
                        doubleBuffering();
                        pc = true;
                      }
                    }
                    if (key_scancode==NINE_KEY){
                      if(!isOccupied(board_positions,8)){
                        board_positions[8]=1;
                        vg_draw_img(img_panda, 444,413);
                        vg_draw_img(img_cursor, xCursor,yCursor);
                        doubleBuffering();
                        pc = true;
                      }
                    }
                  }
                  else{
                    int i = pc_play(board_positions);
                    board_positions[i] = 2;
                    int x = draw_images_getX(board_positions,i);
                    int y = draw_images_getY(board_positions,i);
                    vg_draw_img(img_tigre,x,y);
                    vg_draw_img(img_cursor, xCursor,yCursor);
                    //Double buffering
                    doubleBuffering();
                    pc = false;
                  }
                  if(endGame(board_positions,0) || endGame(board_positions,1) || endGame(board_positions,2)){
                    time=(global_counter%3600)/60;
                    gameOver(board_positions,time);
                    global_counter = 0;
                    if(endGame(board_positions, 0))
                      vg_draw_img(panda_sp, 0, 0);                          
                    if(endGame(board_positions,1))
                      vg_draw_img(tiger_sp, 0, 0);
                    if(endGame(board_positions, 2))
                      vg_draw_img(tie_img, 0, 0);
                    doubleBuffering();                   
                    sleep(3);                       
                    exit = true;
                  }                    
                }
              }
              if (msg.m_notify.interrupts & timer_irq){ /*subscribed interrupt*/
                timer_int_handler();
                vg_draw_img(img_board,0,0);
                printCounter();

                for(int i = 0; i < 9;i++){
                  if(board_positions[i] == 1){
                    int x = draw_images_getX(board_positions,i);
                    int y = draw_images_getY(board_positions,i);
                    vg_draw_img(img_panda,x,y);
                  }
                  if(board_positions[i] == 2){
                    int x = draw_images_getX(board_positions,i);
                    int y = draw_images_getY(board_positions,i);
                    vg_draw_img(img_tigre,x,y);
                  }
                }
                vg_draw_img(img_cursor, xCursor,yCursor);
                //Double buffering
                doubleBuffering();
              }
              break;
          default:
              break; /*no other notifications expected: do nothing*/
          }
      }
      else
      { /*received a standard message, not a notification*/
          /*no standard messages expected: do nothing*/
      }
  }

  //Disables mouse data reporting
  disableDataReporting();

  if (mouse_unsubscribe_int() == 1){
      printf("Mouse unsubscription failed!\n");
      return 1;
  }

  if(kbc_unsubscribe_int() == 1){
    return 1;
  }

  if (timer_unsubscribe_int() == 1)
    return 1;

  return 0;
}

int multiplayer(){
  /*---VARIABLES DECLARATION---*/
  //Cursor variables
  xCursor=0, yCursor=0;

  //XPM variables
  xpm_image_t img_board;
  xpm_image_t img_cursor;
  xpm_image_t img_panda;
  xpm_image_t img_tigre;
  xpm_image_t panda_mp;
  xpm_image_t tiger_mp;
  xpm_image_t tie_img;

  //Game variables
  int board_positions[9] = {0}, inc = 0;
  bool exit = false, player1 = false, player2 = false;

  //Mouse subscription variables
  int ipc_status, r;
  message msg;
  struct packet pp;
  unsigned int read_bytes = 0;
  uint8_t mouse_hookID = 0;
  uint32_t mouse_irq = BIT(mouse_hookID);

  //Timer variables
  uint8_t timer_hookID = 1;
  uint32_t timer_irq = BIT(timer_hookID);

  //Keyboard variables
  uint8_t kbd_hookID = 2;
  //uint32_t kbd_irq = BIT(kbd_hookID);


  /*---TIMER---*/
  //Subscribe timer interruptions
  if(timer_subscribe_int(&timer_hookID)==1){
    printf("Timer subscription failed!\n");
    return 1;
  }

  /*---KEYBOARD---*/
  if(kbc_subscribe_int(&kbd_hookID) ==1){
    return 1;
  }

  /*---MOUSE---*/
  //Subscribe mouse interruptions
  if (mouse_subscribe_int(&mouse_hookID) == 1){
      printf("Mouse subscription failed!\n");
      return 1;
  }
  //Enable mouse data reporting
  enableDataReporting();

  /*---XPM---*/
  //Load xpm
  xpm_load(board, XPM_8_8_8, &img_board);
  xpm_load(cursor, XPM_8_8_8, &img_cursor);
  xpm_load(panda, XPM_8_8_8, &img_panda);
  xpm_load(tigre, XPM_8_8_8, &img_tigre);
  xpm_load(panda_multi, XPM_8_8_8, &panda_mp);
  xpm_load(tiger_multi, XPM_8_8_8, &tiger_mp);
  xpm_load(tie, XPM_8_8_8, &tie_img);
  
  createXPMs();

  //Draws bakckground and cursor(initial position)
  vg_draw_img(img_board, 0, 0);
  vg_draw_img(img_cursor, xCursor,yCursor);

  //Double buffering
  doubleBuffering();

  /*---MAIN LOOP---*/
  while (!exit){
      /*Get a request message.*/
      if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0){
          printf("driver_receive failed with: %d", r);
          continue;
      }
      if (is_ipc_notify(ipc_status)){ /*received notification*/
        switch (_ENDPOINT_P(msg.m_source)){
        case HARDWARE: /*hardware interrupt notification*/
          if (msg.m_notify.interrupts & mouse_irq){ /*subscribed interrupt*/
            mouse_ih();
            if ((read_bytes == 0) && ((byte & BIT(3)) == 0)){
                continue;
            }
            else{
              if (read_bytes == 0 || read_bytes == 1 || read_bytes == 2){
                  pp.bytes[read_bytes] = byte;
                  read_bytes++;
              }
              if (read_bytes == 3){
                mouse_process_packets(&pp);
                read_bytes = 0;
                if((inc%2) == 0){
                  player1 = true;
                  player2 = false;
                }
                else{
                  player1 = false;
                  player2 = true;
                }
                //xCursor and yCursor stores mouse coordinates
                update_mouse_position(pp.delta_x, pp.delta_y);

                /*PLAYERS MOVES*/
                //1st row
                if (pp.lb && xCursor>=43 && xCursor<=230 && yCursor>=12 && yCursor<=200){
                  if(player1 && !player2 && !isOccupied(board_positions,0)){
                    board_positions[0]=1;
                    vg_draw_img(img_panda, 60,25);
                    inc++;
                  }
                  if(player2 && !player1 && !isOccupied(board_positions,0)){
                    board_positions[0]=2;
                    vg_draw_img(img_tigre, 60,25);
                    inc++;
                  }
                  vg_draw_img(img_cursor, xCursor,yCursor);
                  doubleBuffering();
                }
                if (pp.lb && xCursor>=240 && xCursor<=423 && yCursor>=12 && yCursor<=200){
                  if(player1 && !player2 && !isOccupied(board_positions,1)){
                    board_positions[1]=1;
                    vg_draw_img(img_panda, 252,25);
                    inc++;
                  }
                  if(player2 && !player1 && !isOccupied(board_positions,1)){
                    board_positions[1]=2;
                    vg_draw_img(img_tigre, 252,25);
                    inc++;
                  }
                    vg_draw_img(img_cursor, xCursor,yCursor);
                    doubleBuffering();
                }
                if (pp.lb && xCursor>=432 && xCursor<=619 && yCursor>=12 && yCursor<=200){
                  if(player1 && !player2 && !isOccupied(board_positions,2)){
                    board_positions[2]=1;
                    vg_draw_img(img_panda, 444,25);
                    inc++;
                  }
                  if(player2 && !player1 && !isOccupied(board_positions,2)){
                    board_positions[2]=2;
                    vg_draw_img(img_tigre, 444,25);
                    inc++;
                  }
                    vg_draw_img(img_cursor, xCursor,yCursor);
                    doubleBuffering();
                }
                //2nd row
                if (pp.lb && xCursor>=43 && xCursor<=230 && yCursor>=208 && yCursor<=390){
                  if(player1 && !player2 && !isOccupied(board_positions,3)){
                    board_positions[3]=1;
                    vg_draw_img(img_panda, 60,221);
                    inc++;
                  }
                  if(player2 && !player1 && !isOccupied(board_positions,3)){
                    board_positions[3]=2;
                    vg_draw_img(img_tigre, 60,221);
                    inc++;
                  }
                    vg_draw_img(img_cursor, xCursor,yCursor);
                    doubleBuffering();
                }
                if (pp.lb && xCursor>=240 && xCursor<=423 && yCursor>=208 && yCursor<=390){
                  if(player1 && !player2 && !isOccupied(board_positions,4)){
                    board_positions[4]=1;
                    vg_draw_img(img_panda, 252,221);
                    inc++;
                  }
                  if(player2 && !player1 && !isOccupied(board_positions,4)){
                    board_positions[4]=2;
                    vg_draw_img(img_tigre, 252,221);
                    inc++;
                  }
                    vg_draw_img(img_cursor, xCursor,yCursor);
                    doubleBuffering();
                }
                if (pp.lb && xCursor>=432 && xCursor<=619 && yCursor>=208 && yCursor<=390){
                  if(player1 && !player2 && !isOccupied(board_positions,5)){
                    board_positions[5]=1;
                    vg_draw_img(img_panda, 444,221);
                    inc++;
                  }
                  if(player2 && !player1 && !isOccupied(board_positions,5)){
                    board_positions[5]=2;
                    vg_draw_img(img_tigre, 444,221);
                    inc++;
                  }
                    vg_draw_img(img_cursor, xCursor,yCursor);
                    doubleBuffering();
                }
                //3rd row
                if (pp.lb && xCursor>=43 && xCursor<=230 && yCursor>=400 && yCursor<=588){
                  if(player1 && !player2 && !isOccupied(board_positions,6)){
                    board_positions[6]=1;
                    vg_draw_img(img_panda, 60,413);
                    inc++;
                  }
                  if(player2 && !player1 && !isOccupied(board_positions,6)){
                    board_positions[6]=2;
                    vg_draw_img(img_tigre, 60,413);
                    inc++;
                  }
                    vg_draw_img(img_cursor, xCursor,yCursor);
                    doubleBuffering();
                }
                if (pp.lb && xCursor>=240 && xCursor<=423 && yCursor>=400 && yCursor<=588){
                  if(player1 && !player2 && !isOccupied(board_positions,7)){
                    board_positions[7]=1;
                    vg_draw_img(img_panda, 252,413);
                    inc++;
                  }
                  if(player2 && !player1 && !isOccupied(board_positions,7)){
                    board_positions[7]=2;
                    vg_draw_img(img_tigre, 252,413);
                    inc++;
                  }
                    vg_draw_img(img_cursor, xCursor,yCursor);
                    doubleBuffering();
                }
                if (pp.lb && xCursor>=432 && xCursor<=619 && yCursor>=400 && yCursor<=588){
                  if(player1 && !player2 && !isOccupied(board_positions,8)){
                    board_positions[8]=1;
                    vg_draw_img(img_panda, 444,413);
                    inc++;
                  }
                  if(player2 && !player1 && !isOccupied(board_positions,8)){
                    board_positions[8]=2;
                    vg_draw_img(img_tigre, 444,413);
                    inc++;
                  }
                    vg_draw_img(img_cursor, xCursor,yCursor);
                    doubleBuffering();
                }
                if(endGame(board_positions,0)){
                  global_counter=0;
                  vg_draw_img(panda_mp, 0, 0);
                  doubleBuffering();
                  sleep(3);
                  exit=true;
                }
                else if(endGame(board_positions,1)){
                  global_counter=0;
                  vg_draw_img(tiger_mp, 0, 0);
                  doubleBuffering();
                  sleep(3);
                  exit=true;
                }
                else if(endGame(board_positions,2)){
                  global_counter=0;
                  vg_draw_img(tie_img, 0, 0);
                  doubleBuffering();
                  sleep(3);
                  exit=true;
                }
              }
            }
          }
          if (msg.m_notify.interrupts & timer_irq){ /*subscribed interrupt*/
            timer_int_handler();
            //Redraws menu and cursor
            vg_draw_img(img_board,0,0);
            for(int i = 0; i < 9;i++){
              if(board_positions[i] == 1){
                int x = draw_images_getX(board_positions,i);
                int y = draw_images_getY(board_positions,i);
                vg_draw_img(img_panda,x,y);
              }
              if(board_positions[i] == 2){
                int x = draw_images_getX(board_positions,i);
                int y = draw_images_getY(board_positions,i);
                vg_draw_img(img_tigre,x,y);
              }
            }
            printCounter();
            vg_draw_img(img_cursor, xCursor,yCursor);
            //Double buffering
            doubleBuffering();
          }
          break;
        default:
          break; /*no other notifications expected: do nothing*/
        }
      }
      else
      { /*received a standard message, not a notification*/
          /*no standard messages expected: do nothing*/
      }
  }

  //Disables mouse data reporting
  disableDataReporting();

  if (mouse_unsubscribe_int() == 1){
      printf("Mouse unsubscription failed!\n");
      return 1;
  }

  if(kbc_unsubscribe_int() == 1){
    return 1;
  }

  if (timer_unsubscribe_int() == 1)
    return 1;


  return 0;
}

int highscores(){

  /*---VARIABLES DECLARATION---*/
  //Cursor variables
  xCursor=0, yCursor=0;

  //Loop variable
  bool exit=false;

  //XPM variables
  xpm_image_t img_cursor;
  xpm_image_t highscores_table;


  //Mouse subscription variables
  int ipc_status, r;
  message msg;
  struct packet pp;
  unsigned int read_bytes = 0;
  uint8_t mouse_hookID = 0;
  uint32_t mouse_irq = BIT(mouse_hookID);


  /*---MOUSE---*/
  //Subscribe mouse interruptions
  if (mouse_subscribe_int(&mouse_hookID) == 1){
      printf("Mouse subscription failed!\n");
      return 1;
  }
  //Enable mouse data reporting
  enableDataReporting();


  /*---XPM---*/
  //Load xpm
  xpm_load(cursor, XPM_8_8_8, &img_cursor);
  xpm_load(table, XPM_8_8_8, &highscores_table);

  createXPMs();

  //Draws bakckground and cursor(initial position)
  vg_draw_img(highscores_table, 0, 0);
  vg_draw_img(img_cursor, xCursor,yCursor);


  //Double buffering
  doubleBuffering();

  /*---MAIN LOOP---*/
  while (!exit){
      /*Get a request message.*/
      if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0){
          printf("driver_receive failed with: %d", r);
          continue;
      }
      if (is_ipc_notify(ipc_status)){ /*received notification*/
          switch (_ENDPOINT_P(msg.m_source)){
          case HARDWARE: /*hardware interrupt notification*/
            if (msg.m_notify.interrupts & mouse_irq){ /*subscribed mouse interrupt*/
              mouse_ih();
              if ((read_bytes == 0) && ((byte & BIT(3)) == 0)){
                  continue;
              }
              else{
                if (read_bytes == 0 || read_bytes == 1 || read_bytes == 2){
                    pp.bytes[read_bytes] = byte;
                    read_bytes++;
                }
                if (read_bytes == 3){
                    mouse_process_packets(&pp);
                    read_bytes = 0;

                    //xCursor and yCursor stores mouse coordinates
                    update_mouse_position(pp.delta_x, pp.delta_y);
                    //Redraws table and cursor
                    vg_draw_img(highscores_table, 0,0);
                    vg_draw_img(img_cursor, xCursor, yCursor);

                    //Print highscores
                    printRecords();
                    doubleBuffering();

                    //Return button
                    if (pp.lb && xCursor>=27 && xCursor<=151 && yCursor>=527 && yCursor<=571){
                          exit = true;
                    }
                  }
                }
            }
            break;
          default:
              break; /*no other notifications expected: do nothing*/
          }
      }
      else
      { /*received a standard message, not a notification*/
          /*no standard messages expected: do nothing*/
      }
  }


  /*---MOUSE---*/

  //Disables mouse data reporting
  disableDataReporting();

  //Unsubscribe mouse interruptions
  if (mouse_unsubscribe_int() == 1){
      printf("Mouse unsubscription failed!\n");
      return 1;
  }

  return 0;
}

int finish(){
  //saveRecords();
  if (vg_exit() != OK)
      return 1;
  return 0;
}
