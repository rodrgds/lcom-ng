#include <lcom/lcf.h>
#include <stdio.h>

#include "Controller/Devices/macros.h"
#include "Controller/Devices/Timer/timer.h"
#include "Controller/Devices/Mouse_Keyboard/Mouse/mouse.h"
#include "Controller/Devices/Mouse_Keyboard/Keyboard/keyboard.h"
#include "Controller/Devices/VideoCard/video_graphics.h"
#include "Controller/MouseController/mouseController.h"
#include "Controller/MainMenuController/mainMenuController.h"
#include "Controller/GameOverController/gameOverController.h"
#include "Model/Mouse/mouseModel.h"
#include "View/MouseView/mouseView.h"
#include "View/MainMenuView/mainMenuView.h"
#include "Model/Background/background.h"
#include "View/InstructionsView/instructionsView.h"
#include "Model/SelectorMine/selectorMine.h"
#include "Controller/state.h"
#include "Controller/GameController/gameController.h"
#include "View/GameView/gameView.h"
#include "View/GameOverView/gameOverView.h"
#include "generalMacros.h"

extern bool packet_complete;
extern uint8_t scan_code;
extern uint8_t* background;
extern int game_counter;
extern bool board_cleared;

int game_time = 0;
uint8_t mouse_mask;
uint8_t time_mask;
uint8_t keyboard_mask;
Mouse mouse;
SMine mine_sel;
game_state state = MAIN_MEU;


int (handle_interrupt_loop)() {
  int ipc_status;
  message msg;
  while( scan_code != ESC_BREAK && !board_cleared && !exploded) { 
    if( driver_receive(ANY, &msg, &ipc_status) != 0 ) {
      printf("Error");
      continue;
    }
    if (is_ipc_notify(ipc_status)) { 
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          if (msg.m_notify.interrupts & mouse_mask) {
            mouse_ih();
            if (packet_complete) {
              handlePacket(&mouse);
            }
          }
          if (msg.m_notify.interrupts & time_mask) {
            timer_int_handler();
            if (game_counter % 60 == 0) {
              game_time++;
            }
            if (draw_background() != 0) return 1;
            if (drawMouse(mouse) != 0) return 1;
            DrawTime(game_time);
            DrawFlagCounter();
            if (flip_page() != 0) return 1;
          }
          if (msg.m_notify.interrupts & keyboard_mask) {
            kbc_ih();
          }
      }
    }
  }
  return 0;
}

int subscribe_int() {
  if (timer_subscribe_int(&time_mask) != 0) return 1;
  if (sub_mouse_int(&mouse_mask) != 0) return 1;
  if (kb_sub_int(&keyboard_mask) != 0) return 1;
  if (write_mouse_command(ENABLE_DR) != 0) return 1;
  return 0;
}

int unsubscribe_int() {
  if (timer_unsubscribe_int() != 0) return 1;
  if (unsub_mouse_int() != 0) return 1;
  if (kb_unsub_int() != 0) return 1;
  return 0; 
}

int start_video() {
  if (get_mode_conf(DIRECT_800X600_MODE)!= 0) return 1;
  if (set_double_frame_buffer() != 0) return 1;
  if (init_video(DIRECT_800X600_MODE) != 0) return 1;
  return 0;
}


void createModels() {
  mouse = createMouse(MOUSE_INI_X, MOUSE_INI_Y);
  mine_sel = createSMine();
}

int (main)(int argc, char *argv[]) {

  lcf_set_language("EN-US");
  lcf_trace_calls("/home/lcom/labs/g8/proj/trace.txt");
  lcf_log_output("/home/lcom/labs/g8/proj/output.txt");
  if (lcf_start(argc, argv)) return 1;

  lcf_cleanup();

  return 0;
}

int (proj_main_loop)(int argc, char **argv) {  
  createModels();    
  if(subscribe_int() != 0) return 1;
  if(start_video() != 0) return 1;
  createBackground();
  loadGameSprites();
  buildGameBoard();
  drawGameBoardToBackground();
  drawGameSideBar();
  if(handle_interrupt_loop() != 0) return 1;
  if(vg_exit() != 0) return 1;
  if(unsubscribe_int() != 0) return 1;
  free_buffers();
  destroyMouse(mouse);
  return 0;
}

