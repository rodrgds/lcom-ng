#include "menu.h"
#include "../controllers/keyboard/keyboard.h"
#include "../controllers/keyboard/i8042.h"
#include "../controllers/graphicsCard/graphics.h"
#include "../sprites/sprites.h"
#include "../xpm/exitbuttons.xpm"
#include "../xpm/startbuttons.xpm"
#include "../xpm/spaceInvaders.xpm" 
#include "../xpm/levelbuttons.xpm"
#include "../global.h"
#include "../game/game.h"
#include "../menulevel/menulevel.h"
#include "../view/menu/menuView.h"


Sprite* levelButton = NULL;
Sprite* exitButton = NULL;
Sprite* startButton = NULL;
Sprite* SpaceInvaders= NULL;
MenuSelection current_button = MENU_START;


void updateButtonStates() {
    switch (current_button) {
        case MENU_START:
            startButton->map = xpm_load(drawStartpressed_xpm, XPM_INDEXED, &startButton->img);
            levelButton->map = xpm_load(drawLevel_xpm, XPM_INDEXED, &levelButton->img);
            exitButton->map = xpm_load(drawExit_xpm, XPM_INDEXED, &exitButton->img);
            break;
        case MENU_LEVEL:
            startButton->map = xpm_load(drawStart_xpm, XPM_INDEXED, &startButton->img);
            levelButton->map = xpm_load(drawLevelpressed_xpm, XPM_INDEXED, &levelButton->img);
            exitButton->map = xpm_load(drawExit_xpm, XPM_INDEXED, &exitButton->img);
            break;
        case MENU_EXIT:
            startButton->map = xpm_load(drawStart_xpm, XPM_INDEXED, &startButton->img);
            levelButton->map = xpm_load(drawLevel_xpm, XPM_INDEXED, &levelButton->img);
            exitButton->map = xpm_load(drawExitpressed_xpm, XPM_INDEXED, &exitButton->img);
            break;
    }
}


int controllMenuInterrupts(){
  int ipc_status, r;
  message msg;
  uint8_t bit_no_kbc;
  uint32_t irq_set_kbc;
  bool make;

  if (kbd_subscribe_int(&bit_no_kbc) != OK) return 1;
  irq_set_kbc= BIT(bit_no_kbc);

  while (state == MENU_MODE) {
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != OK) {
        printf("driver_receive failed with: %d\n", r);
        continue;
    }

    if (is_ipc_notify(ipc_status)) { 
        switch (_ENDPOINT_P(msg.m_source)) {
          case HARDWARE:
            if (msg.m_notify.interrupts & irq_set_kbc) {
              kbc_ih();
              uint8_t scancode = latest_scancode[latest_scancode[0] == TWO_BYTES ? 1 : 0];
              make = !(scancode & MAKE_CODE);

              if (done && make) {
                  switch (scancode) {
                      case ENTER_MAKECODE:
                          eraseSprite(exitButton);
                          eraseSprite(levelButton);
                          eraseSprite(startButton);
                          eraseSprite(SpaceInvaders);
                  
                          if (current_button == MENU_START){
                            state = PLAY_MODE;
                            continue;
                            }
                          else if (current_button == MENU_EXIT){state = EXIT_MODE;}
                          else if (current_button == MENU_LEVEL) {
                            kbd_unsubscribe_int();
                            level_menu_loop();
                            kbd_subscribe_int(&bit_no_kbc);                    
                                if (state == MENU_MODE) {
                                    drawMenu();
                                    updateButtonStates();
                                    drawMenu();
                                }
                          }
                          break;
                  
                      case ESCAPE_MAKECODE:
                          state = EXIT_MODE;
                          break;
                  
                      case SCANCODE_DOWN:
                          if (current_button == MENU_EXIT) current_button = MENU_START;
                          else current_button++;
                          updateButtonStates();
                          drawMenu();
                          break;
                  
                      case SCANCODE_UP:
                          if (current_button == MENU_START) current_button = MENU_EXIT;
                          else current_button--;
                          updateButtonStates();
                          drawMenu();
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


  if (kbd_unsubscribe_int() != OK) {
      return 1;
  }

    return 0;
}
  


int menu_loop() {
  
    SpaceInvaders = create_static(drawSpaceInvaders_xpm, 180, 100); 
    if (!SpaceInvaders) {
        return 1;
    }

    startButton = create_static(drawStart_xpm, 320, 350);
    if (!startButton) {
        return 1;
    }

    levelButton = create_static(drawLevel_xpm, 320, 450); 
    if (!levelButton) {
        return 1;
    }
    
    exitButton = create_static(drawExit_xpm, 320, 550); 
    if (!exitButton) {
        return 1;
    }

    updateButtonStates();
    drawMenu();
    controllMenuInterrupts();


    destroy_sprite(startButton);
    destroy_sprite(SpaceInvaders);
    destroy_sprite(levelButton);
    destroy_sprite(exitButton);
    
    return 0;
}
