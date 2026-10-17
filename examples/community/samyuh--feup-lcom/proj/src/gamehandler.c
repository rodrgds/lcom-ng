#include "graphics.h"
#include "time.h"
#include "keyboard.h"
#include "i8042.h"
#include "mouse.h"
#include "gamehandler.h"
#include "game.h"
#include "rtc.h"

//Keyboard Global Variables
extern uint8_t scanCode[2];
bool check = false;

//Mouse global Variables
extern uint8_t mouseByte[3];
extern bool mouseSync;
extern int mouseByteCounter;

//Time Elapsed
extern uint32_t timerCounter;

uint8_t generateKey(ImageLibrary *image,  xpm_image_t *dinamicImg) {
  uint8_t makeCodes[26] = {A_MAKECODE, B_MAKECODE, C_MAKECODE, D_MAKECODE, E_MAKECODE, F_MAKECODE, G_MAKECODE, H_MAKECODE,
                          I_MAKECODE, J_MAKECODE, K_MAKECODE, L_MAKECODE, M_MAKECODE, N_MAKECODE, O_MAKECODE, P_MAKECODE,
                          Q_MAKECODE, R_MAKECODE, S_MAKECODE, T_MAKECODE, U_MAKECODE, V_MAKECODE, W_MAKECODE, X_MAKECODE,
                          Y_MAKECODE, Z_MAKECODE};

  char characters[26] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h',
                         'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p',
                         'q', 'r','s', 't', 'u', 'v', 'w', 'x',
                         'y', 'z'};
  int rand = rand() % 26;
  *dinamicImg = charToDraw(image, characters[rand]);
  return makeCodes[rand];
}



int mainMenu(){
  //Informação sobre o rato
  //Init game
  GameInfo game = initGame();
  bool updateMenu = true;
  bool changeStateNext = true;
  //Interrupts
  int ipc_status, r;
  message msg;

  uint8_t kbc_bit_no = 0, mouse_bit_no = 1, timer_bit_no = 2;
  uint32_t kbc_irq_set, mouse_irq_set, timer_irq_set;

  if(mouse_enable_data_report() != OK) return 1;

  if(mouse_subscribe_int(&mouse_bit_no)!= OK) return 1;
  mouse_irq_set = BIT(mouse_bit_no);

  if(keyboard_subscribe_int(&kbc_bit_no)!= 0) return 1;
  kbc_irq_set = BIT(kbc_bit_no);

  if(timer_subscribe_int(&timer_bit_no)!= 0) return 1;
  timer_irq_set = BIT(timer_bit_no);

  //Interrupts loop
  while(game.state != EXIT) {
    /* Get a request message. */
    if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
        printf("driver_receive failed with: %d", r);
        continue;
    }
    if (is_ipc_notify(ipc_status)) { // received notification
        switch (_ENDPOINT_P(msg.m_source)) {
            case HARDWARE: // hardware interrupt notification
                if (msg.m_notify.interrupts & kbc_irq_set) { // subscribed interrupt
                  // Keyboard Interrupts
                  kbc_ih();
                  if (scanCode[0] == 0xE0) {
                    if (check) check = false;
                    else check = true;
                  }

                  //Meter isto tudo numa função com ifs
                  if(game.state == GAME) {
                    if (scanCode[0] == game.race.key && game.race.start) {
                      game.race.key = 0;
                      if(!game.race.nitro) {
                        if(game.race.position.speed <= game.car.maxSpeed) game.race.position.speed += 3;
                        else game.race.position.speed = game.car.maxSpeed;
                      }
                      else {
                        if(game.race.position.speed <= game.car.maxSpeed+10) game.race.position.speed += 5;
                        else game.race.position.speed = game.car.maxSpeed+10;
                      }
                      game.race.key = generateKey(game.image, &game.race.dinamicImg);
                      game.race.combo++;
                    }
                    else if ((scanCode[0] & BIT(7)) == 0){
                        game.race.combo = 0;
                        if(game.race.position.speed >= 3) game.race.position.speed -= 3;
                      }
                    }
                  //Acaba aqui
                  }
                if (msg.m_notify.interrupts & mouse_irq_set) { // subscribed interrupt
                  mouse_ih();
                  if (mouseByteCounter == 3) {
                    struct packet info;
                    read_packet(&info);
                    mouseByteCounter = 0;

                    if(game.mousePlayer->x + info.delta_x <= 1010 && game.mousePlayer->x + info.delta_x >= 0) game.mousePlayer->x += info.delta_x;
                    if(game.mousePlayer->y - info.delta_y <= 752 && game.mousePlayer->y - info.delta_y >= 0) game.mousePlayer->y -= info.delta_y;
                    game.mousePlayer->deltax = info.delta_x;
                    game.mousePlayer->deltay = info.delta_y;
                    game.mousePlayer->press = info.lb;
                    game.mousePlayer->ev = mouse_event_handler(&info);

                    //Meter isto tudo numa função com if

                    if(game.state == LEVEL_SELECT) {
                      if(160 < game.mousePlayer->y && game.mousePlayer->y < 350 && (game.mousePlayer->ev->type == LB_PRESSED)) {
                        if(40 < game.mousePlayer->x && game.mousePlayer->x < 250 && (game.highestLevel >= 0)){
                          game.racenum = 0;
                          numberRace(&game, game.racenum);
                          game.state = GAME;
                          updateMenu = true;
                        }
                        if(280 < game.mousePlayer->x && game.mousePlayer->x < 490 && (game.highestLevel >= 1)) {
                          game.racenum = 1;
                          numberRace(&game, game.racenum);
                          game.state = GAME;
                          updateMenu = true;
                        }
                        if(520 < game.mousePlayer->x && game.mousePlayer->x < 740 && (game.highestLevel >= 2)) {
                          game.racenum = 2;
                          numberRace(&game, game.racenum);
                          game.state = GAME;
                          updateMenu = true;
                        }
                        if(770 < game.mousePlayer->x && game.mousePlayer->x < 980 && (game.highestLevel >= 3)) {
                          game.racenum = 3;
                          numberRace(&game, game.racenum);
                          game.state = GAME;
                          updateMenu = true;
                        }
                      }

                      if(520 < game.mousePlayer->y && game.mousePlayer->y < 710 && (game.mousePlayer->ev->type == LB_PRESSED)) {
                        if(40 < game.mousePlayer->x && game.mousePlayer->x < 250 && (game.highestLevel >= 4)){
                          game.racenum = 4;
                          numberRace(&game, game.racenum);
                          game.state = GAME;
                          updateMenu = true;
                        }
                        if(280 < game.mousePlayer->x && game.mousePlayer->x < 490 && (game.highestLevel >= 5)){
                          game.racenum = 5;
                          numberRace(&game, game.racenum);
                          game.state = GAME;
                          updateMenu = true;
                        }
                        if(520 < game.mousePlayer->x && game.mousePlayer->x < 740 && (game.highestLevel >= 6)){
                          game.racenum = 6;
                          numberRace(&game, game.racenum);
                          game.state = GAME;
                          updateMenu = true;
                        }
                        if(770 < game.mousePlayer->x && game.mousePlayer->x < 980 && (game.highestLevel >= 7)){
                          game.racenum = 7;
                          numberRace(&game, game.racenum);
                          game.state = GAME;
                          updateMenu = true;
                        }
                      }
                      if(0 < game.mousePlayer->y && game.mousePlayer->y < 60 && (game.mousePlayer->ev->type == LB_PRESSED)) {
                        if(950 < game.mousePlayer->x && game.mousePlayer->x < 1020) {
                          game.state = MAIN_MENU;
                          updateMenu = true;
                        }
                      }
                    }
                    if(game.state == HIGHSCORE_MENU) {
                      if(game.mousePlayer->ev->type == LB_PRESSED) {
                        if(changeStateNext){
                          game.state = MAIN_MENU;
                          changeStateNext = false;
                        }
                      }
                    }

                    if(game.state == END_GAME) {
                      if(480 < game.mousePlayer->y && game.mousePlayer->y < 550 && (game.mousePlayer->ev->type == LB_PRESSED)) {
                        if(120 < game.mousePlayer->x && game.mousePlayer->x < 380)
                        if(changeStateNext) {
                          game.state = MAIN_MENU;
                          changeStateNext = false;
                        }
                        if(460 < game.mousePlayer->x && game.mousePlayer->x < 560){
                          if(changeStateNext) {
                            updateMenu = true;
                            repeatRace(&game);
                            game.state = GAME;
                            changeStateNext = false;
                          }
                        }
                        if(640 < game.mousePlayer->x && game.mousePlayer->x < 890) {
                          if(game.highscore[game.racenum] <= game.targetTime[game.racenum]) {
                            if(changeStateNext && (game.racenum != 7)) {
                              updateMenu = true;
                              nextRace(&game);
                              game.state = GAME;
                              changeStateNext = false;
                            }
                          }
                        }
                      }
                    }
                    if(game.state == GAME) {
                      if(!game.race.nitro) {
                        if(4*64 < game.mousePlayer->y && game.mousePlayer->y < 9*64 && game.mousePlayer->press) {
                          if(13*64+32 < game.mousePlayer->x && game.mousePlayer->x < 15*64+32) {
                            if(game.race.combo >= 5) {
                              if(game.race.nitroh >= 250) //Barra full
                                game.race.nitro = true; //Ativar nitro
                              game.race.nitroh += game.mousePlayer->deltay;
                              }
                            }
                        }
                    }
                    }

                    if(game.state == MAIN_MENU) {
                        if(30 < game.mousePlayer->x && game.mousePlayer->x < 230 && (game.mousePlayer->ev->type == LB_PRESSED)) {
                          if(360 < game.mousePlayer->y && game.mousePlayer->y < 420) {
                            //Fazer para aparecer uma janela para escolher o nivel
                            if(changeStateNext) {
                              game.state = LEVEL_SELECT;
                              changeStateNext = false;
                            }
                          }

                          if(450 < game.mousePlayer->y && game.mousePlayer->y < 515) {
                          if(changeStateNext) {
                            updateMenu = true;
                            game.state = HIGHSCORE_MENU;
                            changeStateNext = false;
                          }
                          }
                          if(535 < game.mousePlayer->y && game.mousePlayer->y < 600) {
                          if(changeStateNext) {
                            game.state = EXIT;
                            changeStateNext = false;
                          }
                          }
                      }
                    }
                    //Acaba aqui
                  }
                }

                if(msg.m_notify.interrupts & timer_irq_set) {
                  timer_int_handler();
                  changeStateNext = true;
                  //Meter isto tudo numa função com if

                  if(game.state == END_GAME) {
                    if(updateMenu) {
                      draw_xpm(0, 0, game.image->endGame);

                      //Time
                      drawNumber(game.image, 650, 80, game.race.time, 0.4);

                      //Targetme
                      drawNumber(game.image, 650, 190, game.targetTime[game.racenum], 0.4);

                      //highscore
                      if(game.race.time < game.highscore[game.racenum] || game.highscore[game.racenum] == -1) {
                        setHighscore(&game, game.racenum);
                        game.highscore[game.racenum] = game.race.time;
                      }

                      if(game.highscore[game.racenum] <= game.targetTime[game.racenum]){
                          if(game.racenum != 7){
                            if(game.highestLevel <= game.racenum) {
                              game.highestLevel = game.racenum + 1;
                            }
                          }
                      }

                      //rtc
                      drawNumber(game.image, 400, 400, game.highscoreDate[game.racenum].year, 0.3);
                      draw_xpm_resize(485, 400, game.image->slash, 0, 0.3);
                      drawNumber(game.image, 500, 400, game.highscoreDate[game.racenum].month, 0.3);
                      draw_xpm_resize(585, 400, game.image->slash, 0, 0.3);
                      drawNumber(game.image, 600, 400, game.highscoreDate[game.racenum].day, 0.3);
                      drawNumber(game.image, 700, 400, game.highscoreDate[game.racenum].hour, 0.3);
                      draw_xpm_resize(780, 400, game.image->twoDots, 0, 0.3);
                      drawNumber(game.image, 800, 400, game.highscoreDate[game.racenum].min, 0.3);
                      drawNumber(game.image, 650, 300, game.highscore[game.racenum], 0.4);
                    }
                    updateMenu = false;
                    double_buffering();
                    draw_xpm_mem(game.mousePlayer->x,game.mousePlayer->y, game.image->arrow);


                  }
                  if(game.state == LEVEL_SELECT) {
                    draw_xpm(0, 0, game.image->menu_LevelSelect);
                    int i = game.highestLevel + 1;
                    while(i < 8) {
                      if(i < 4) {
                        draw_xpm(30+240*i, 120, game.image->locked);
                      }
                      else {
                        draw_xpm(30+240*(i%4), 480, game.image->locked);
                      }
                      i++;
                    }
                    draw_xpm(game.mousePlayer->x,game.mousePlayer->y, game.image->arrow);
                    double_buffering();
                  }
                  if(game.state == GAME) {
                        //State Machine
                        if(timerCounter == 0)
                          game.race.time++;
                        game.race.runTrack(&game.race.position);
                        if ((game.race.position.lapnum) == game.race.totalLaps){ //Change this num to fast laps
                          if(changeStateNext) {
                            game.state = END_GAME;
                            changeStateNext = false;
                          }
                        }

                        if(timerCounter == 0) {
                          if(game.race.position.speed > 0) game.race.position.speed--;
                          game.race.countdown++;
                        }

                        if(game.race.countdown < 3)
                          game.race.dinamicImg = numberToDraw(game.image, 3 - game.race.countdown);
                        else if (game.race.countdown == 3) {
                          game.race.key = generateKey(game.image, &game.race.dinamicImg);
                          game.race.start = true;
                          game.race.countdown++;
                        }
                        //Pista
                        draw_xpm(0, 0, game.race.track);

                        //Combo
                        drawNumber(game.image, 8*64, 32, game.race.combo, 0.5);
                        //lapnum
                        drawNumber(game.image, 16, 36, game.race.position.lapnum, 0.5);
                        draw_xpm_resize(170, 10, game.image->slash, 0, 1);
                        drawNumber(game.image, 200, 36, game.race.totalLaps - 1, 0.5);
                        //Letra para pressionar
                        draw_xpm_resize(64 * 12, 32,  game.race.dinamicImg,  0, 0.5);
                        //Barra de nitro
                        if (game.race.nitro)  {
                          draw_xpm(14*64, 4*64, game.image->nitroFullBar);
                        }
                        else {
                          draw_xpm(14*64, 4*64, game.image->nitroBar);
                          for(int i = 0; i < game.race.nitroh; i += 10)
                            draw_xpm(14*64+5, (9*64)-i, game.image->fuel);
                          draw_xpm(14*64+5, (9*64)-game.race.nitroh, game.image->fuel);
                        }
                        //Carro
                        draw_xpm_rotated(game.race.position.x, game.race.position.y, game.car.img, game.race.position.angle);
                        //Rato
                        if(game.race.nitro) draw_xpm(14*64, 32, game.image->nitroActive);
                        draw_xpm( game.mousePlayer->x,game.mousePlayer->y, game.image->arrow);
                        double_buffering();
                    }

                  if(game.state == MAIN_MENU) {
                      draw_xpm(0, 0, game.image->menu);
                      draw_xpm(game.mousePlayer->x,game.mousePlayer->y, game.image->arrow);
                      double_buffering();
                  }
                  if(game.state == HIGHSCORE_MENU) {
                    if(updateMenu) {
                      draw_xpm(0, 0, game.image->menu_highscores);
                      int x = 300, y = 100;
                      for(size_t i = 0; i < 8; i++) {
                        if(game.highscore[i] != -1) {
                          drawNumber(game.image, x, y, game.highscore[i], 0.3);
                          drawNumber(game.image, 500, y, game.highscoreDate[i].year, 0.3);
                          draw_xpm_resize(585, y, game.image->slash, 0, 0.3);
                          drawNumber(game.image, 600, y, game.highscoreDate[i].month, 0.3);
                          draw_xpm_resize(685, y, game.image->slash, 0, 0.3);
                          drawNumber(game.image, 700, y, game.highscoreDate[i].day, 0.3);
                          drawNumber(game.image, 800, y, game.highscoreDate[i].hour, 0.3);
                          draw_xpm_resize(880, y, game.image->twoDots, 0, 0.3);
                          drawNumber(game.image, 900, y, game.highscoreDate[i].min, 0.3);
                        }
                        else
                          drawNumber(game.image, x, y, 0, 0.3);
                        y += 83;
                      }
                    }
                    updateMenu = false;
                    double_buffering();
                    draw_xpm_mem(game.mousePlayer->x,game.mousePlayer->y, game.image->arrow);
                  }

                }

                break;
            default:
                break; // no other notifications expected: do nothing
        }
    } else { // received a standard message, not a notification
        // no standard messages expected: do nothing
    }
  }

  exitGame(&game);

  if(timer_unsubscribe_int()!= OK) return 1;

  if(keyboard_unsubscribe_int() != OK) return 1;

  if(mouse_unsubscribe_int() != OK) return 1;

  if(mouse_disable_data_report() != OK) return 1;

  return 0;
}
