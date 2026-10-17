#include <lcom/lcf.h>
#include <stdint.h>
#include <stdio.h>
#include "src/include/controllers/kbc/mouse.h"
#include "src/include/Macros.h"
#include "src/include/utils.h"
#include "src/include/controllers/video/graphics.h"
#include "src/include/controllers/kbc/keyboard.h"
#include "src/include/controllers/kbc/KeyboardInput.h"
#include "src/include/controllers/timer/timer.h"
#include "src/include/controllers/kbc/MouseInput.h"
#include "src/include/controllers/rtc/rtc.h"
#include "src/include/states/states.h"
#include "src/include/states/GameState.h"
#include "src/include/states/MenuState.h"
#include "src/include/states/CharacterState.h"
#include "src/include/states/SettingsState.h"
#include "src/controllers/libkbc/i8042.h"
#include "src/controllers/libtimer/i8254.h"

extern uint8_t scancode;
static uint8_t keyboardHook;
static uint8_t mouseHook;
static uint8_t timerHook;
static uint8_t rtcHook;
static int timer_interrupts = 0;

int main(int argc, char *argv[]) {
    lcf_set_language("EN-US");
    lcf_trace_calls("/home/lcom/labs/proj/trace.txt");
    lcf_log_output("/home/lcom/labs/proj/output.txt");
    if (lcf_start(argc, argv))
      return 1;
    lcf_cleanup();
  
    return 0;
}

static int (startGameLoop)() {
  CHECK_RETURN(makeStates());

  CHECK_RETURN(map_memory(0x105));
  CHECK_RETURN(start_videomode(0x105));
  CHECK_RETURN(draw_map(getMapByDifficulty(selectedDifficulty)));
  CHECK_RETURN(mouseWriteCommand(ENABLE_DATA));
  CHECK_RETURN(mouseWriteCommand(ENABLE_STREAM));
  CHECK_RETURN(keyboardSubscribeInt(&keyboardHook));
  CHECK_RETURN(mouseSubscribeInt(&mouseHook));
  CHECK_RETURN(timer_subscribe_int(&timerHook));
  CHECK_RETURN(rtc_subscribe_int(&rtcHook));
  CHECK_RETURN(timer_set_frequency(0,60));
  CHECK_RETURN(rtc_init());
  
  return 0;
}

static int (endGameLoop)() {
  CHECK_RETURN(exit_videomode());
  CHECK_RETURN(timer_unsubscribe_int());
  CHECK_RETURN(keyboardUnsubscribeInt());
  CHECK_RETURN(mouseUnsubscribeInt());
  CHECK_RETURN(rtc_unsubscribe_int());
  CHECK_RETURN(mouseWriteCommand(DISABLE_DATA));
  free_buffer();
  destroyStates();
  destroyMap();
  return 0;
}

int(proj_main_loop)(int argc, char* argv[]) {
  if(startGameLoop()!=0) return endGameLoop();
  int ipc_status;
  message msg;
  KBCInput *input = NULL;
  
  while (gameState != GameStopping) {
    if (driver_receive(ANY, &msg, &ipc_status) != 0) {
      printf("Error");
      break;
    }
    if (is_ipc_notify(ipc_status)) {
      switch(_ENDPOINT_P(msg.m_source)) {
        case HARDWARE: 
          if (msg.m_notify.interrupts & keyboardHook){
            kbc_ih();
            if(keyboardFormattedInput) {
              input = kBCKeyboardConstructor(keyboardFormattedInput);
              keyboardFormattedInput->destroy(keyboardFormattedInput);
              keyboardFormattedInput = NULL;
            }
          }
          
          if (msg.m_notify.interrupts & mouseHook){
            mouse_ih();
            if(mouseFormattedInput) {
              if(mouseState) {
                int16_t currentX = mouseState->getMouseX(mouseState);
                int16_t currentY = mouseState->getMouseY(mouseState);
                int16_t newX = currentX + mouseFormattedInput->getX(mouseFormattedInput);
                int16_t newY = currentY - mouseFormattedInput->getY(mouseFormattedInput); 
                
                if(newX < 0) newX = 0;
                if(newX >= 1024) newX = 1023;
                if(newY < 0) newY = 0;
                if(newY >= 768) newY = 767;
                
                mouseState->setPosition(mouseState, newX, newY);
              }
              
              input = kBCMouseConstructor(mouseFormattedInput);
              mouseFormattedInput->destroy(mouseFormattedInput);
              mouseFormattedInput = NULL;
            }
          }

          if(input){
            updateKBCState(input);
            input->destroy(input);
            input = NULL;
          }
          if(msg.m_notify.interrupts & timerHook){
            timer_interrupts++;

            if (timer_interrupts % 60 == 0) {
              rtc_time_t current_time;
              rtc_get_time(&current_time);
              if (gameState == GameMenu && menuState != NULL) {
                menuState->draw(menuState);
              }
            }
            nextFrame();
          }
          if(msg.m_notify.interrupts & rtcHook){
            rtc_ih();
          }
          break;
        default:
          break;
      }
   }
  }
  CHECK_RETURN(endGameLoop());
  return 0;
}


