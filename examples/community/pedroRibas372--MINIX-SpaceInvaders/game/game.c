#include "game.h"
#include "../sprites/sprites.h"
#include "../controllers/graphicsCard/graphics.h"
#include "../xpm/LVL.xpm"
#include "../xpm/SCR.xpm"
#include "../xpm/HP.xpm"
#include "../global.h"
#include "../xpm/hero.xpm"
#include "../xpm/youlose.xpm"
#include "../xpm/youwin.xpm"
#include "../controllers/timer/i8254.h"
#include "../controllers/keyboard/i8042.h"
#include "../controllers/keyboard/keyboard.h"
#include "../view/game/gameView.h"

GameState state;


void loadGame(){
  create_hero(drawHero_xpm, 480, 700);
  
  if (selected_level == LEVEL1){
    loadEnemiesLevel1();
    level = 1;
  }
  else if (selected_level == LEVEL2){
    loadEnemiesLevel2();
    level =2;
  }
  else if (selected_level == LEVEL3){
    loadEnemiesLevel3();
    level = 3;
  }
}

void destroygame(){
  destroyEnemies();
  destroyBullets();
  destroy_sprite(hero);
  indexEnemies = 0;
  totalEnemies = 0;
  indexBullets = 0;
  moveSelecter = 0;
    
  memset(double_buffer,0,get_frame_size()); 
  update_screen();
}


int controllGameInterrupts(){
  if (state != PLAY_MODE){
    return 0;
  }
  
  counter = 0;
  int ipc_status, r;
  message msg;
  uint8_t bit_no_kbc, bit_no_timer;
  uint32_t irq_set_kbc, irq_set_timer;
  bool make;

  if (kbd_subscribe_int(&bit_no_kbc) != OK) return 1;
  if (timer_subscribe_int(&bit_no_timer)!=OK) return 1;
  timer_set_frequency(TIMER_0, 60);
  irq_set_kbc= BIT(bit_no_kbc);
  irq_set_timer = BIT(bit_no_timer);

  while (state == PLAY_MODE) {
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != OK) {
        printf("driver_receive failed with: %d\n", r);
        continue;
    }

    if (is_ipc_notify(ipc_status)) { 
        switch (_ENDPOINT_P(msg.m_source)) {
          case HARDWARE:
          if (msg.m_notify.interrupts & irq_set_timer) { 
            timer_int_handler();

            if(counter % 120 == 0){
              enemyShoot();
            }

            if (counter % 60 == 0){
              moveEnemies();
            }
          
              moveBullets();
          
              for (int i = 0; i < totalBullets; i++) {
                if (bullets[i] == NULL) continue;

                if (check_collision(bullets[i], hero)) {
                  health--;
                  eraseSprite(bullets[i]);
                  bullets[i]->x = 1025;
                  if (health ==0){
                    memset(double_buffer, 0, get_frame_size());
                    score =0;
                    health=3;
                    state = LOST_MODE;
                    break;
                  }

                }

                for (int j = 0; j < indexEnemies; j++) {
                  if (enemies[j] == NULL || enemies[j]->dead) continue;
          
                  if (check_collision(bullets[i], enemies[j])) {
                    eraseSprite(bullets[i]);
                    destroy_sprite(bullets[i]);
                    bullets[i] = NULL;
          
                    eraseSprite(enemies[j]);
                    enemies[j]->dead = 1;     
                    totalEnemies--;

                    if (level == 3){
                      score+=3;
                    }
                    else if (level == 2){
                      score+=2;
                    }
                    else{score+=1;}

                    if (score>=100){
                      if (health<9){
                        health++;
                      }
                      score = score-100;
                    }
                    break; 
                  }
                }
              }
              if (totalEnemies == 0){
                state = WON_MODE;
                break;
              }
              if (drawHeader() != OK){return 1;}
            update_screen();
          }
            if (msg.m_notify.interrupts & irq_set_kbc) {
              kbc_ih();
              if (done == 1) {
                make = !(latest_scancode[latest_scancode[0] == TWO_BYTES ? 1: 0] & MAKE_CODE);

              switch(latest_scancode[0]){
                case D_MAKECODE:
                  moveRight(hero);
                  break;

                case A_MAKECODE:
                  moveLeft(hero);
                  break;
                
                case ESCAPE:
                  memset(double_buffer, 0 ,get_frame_size());
                  update_screen();
                  state = MENU_MODE;
                  score =0;
                  health =3;
                  break;
                
                case SPACE_MAKECODE:
                  heroShoot();
                  break;
              }
            }
          }
        break;
      default:
        break;

      }
    }
  }

  if(timer_unsubscribe_int()!=OK){return 1;}

  if (kbd_unsubscribe_int() != OK) {
      printf("Failed to unsubscribe keyboard interrupts\n");
      return 1;
  }

    return 0;
}


