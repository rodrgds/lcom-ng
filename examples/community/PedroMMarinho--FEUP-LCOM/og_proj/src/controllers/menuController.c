#include "menuController.h"
#include "../labs/graphics.h"
#include "../model/table.h"
#include "../labs/scancodes.h"
#include "../labs/serialPort.h"


STATE handleMainMenu(Menu *menu, int option) {
  printf("option stuff: %d\n", option);

  switch (option) { // 0 - Play, 1 - Instructions, 2 - Exit, 3 - Options
    case 0:
      resetMenu(menu);
      setMenuType(menu, GAME_MODE_MENU);
      return MENU;
      break;
    case 1:
      resetMenu(menu);
      setMenuType(menu, INSTRUCTIONS_MENU);
      return MENU;
      break;
    case 2:
      resetMenu(menu);
      destroyMenu(menu);
      return OVER;
      break;
    case 3:
      resetMenu(menu);
      setMenuType(menu, OPTIONS_MENU);
      return MENU;
      break;
    default:
      return OVER;
  }
}
STATE handleGameModeMenu(Menu *menu, int option) {
  switch (option) { // 0 - Local, 1 - Online, 2 - Back (local and online not implemented)
    case 0:
      resetMenu(menu);
      destroyMenu(menu);
      set_game_time(0);
      set_round_time(40);
      return PLAYING;
      break;
    case 1:
      // Needs UART implementation 
      resetMenu(menu);
      setMenuType(menu,ONLINE_MENU);

      sp_ih();
      return handleMultiplayerConnection();
      break;
    case 2:
      resetMenu(menu);
      setMenuType(menu, MAIN_MENU);
      return MENU;
      break;
    default:
      return OVER;
  }
}
STATE handleInstructionsMenu(Menu *menu, int option) {
  switch (option) { // 0 - Back
    case 0:
      resetMenu(menu);
      setMenuType(menu, MAIN_MENU);
      return MENU;
      break;
    default:
      return OVER;
  }
}

STATE handleOptionsMenu(Menu *menu, int option) {
  switch (option) { // 0 - Back
    case 0:
      resetMenu(menu);
      destroyFont(menu->font);
      setMenuType(menu, MAIN_MENU);
      return MENU;
      break;
    default:
      return OVER;
  }
}

STATE handleGameOverMenu(Menu *menu, int option) {
  switch (option) { // 0 - Back
    case 0:
      resetMenu(menu);
      setMenuType(menu, MAIN_MENU);
      return MENU;
      break;
    default:
      return OVER;
  }
}

STATE handleOnlineMenu(Menu *menu, int option) {
  switch (option) { // 0 - Back
    case 0:
      resetMenu(menu);
      setMenuType(menu, GAME_MODE_MENU);
      resetMultiplayer();
      return MENU;
      break;
    default:
      return OVER;
  }
}

void convertScanCodeToChar(char* playerName, uint8_t scanCode){
  if(scanCode == SCANCODE_BACKSPACE){
    if(strlen(playerName) > 0){
      playerName[strlen(playerName)-1] = '\0';
    }
  }
  if(strlen(playerName)< 20){
    switch (scanCode)
    {
    case SCANCODE_0:
      strcat(playerName, "0");
      break;
    case SCANCODE_1:
      strcat(playerName, "1");
      break;
    case SCANCODE_2:
      strcat(playerName, "2");
      break;
    case SCANCODE_3:
      strcat(playerName, "3");
      break;
    case SCANCODE_4:
      strcat(playerName, "4");
      break;
    case SCANCODE_5:
      strcat(playerName, "5");
      break;
    case SCANCODE_6:
      strcat(playerName, "6");
      break;
    case SCANCODE_7:
      strcat(playerName, "7");
      break;
    case SCANCODE_8:
      strcat(playerName, "8");
      break;
    case SCANCODE_9:
      strcat(playerName, "9");
      break;
    case SCANCODE_A:
      strcat(playerName, "a");
      break;
    case SCANCODE_B:
      strcat(playerName, "b");
      break;
    case SCANCODE_C:
      strcat(playerName, "c");
      break;
    case SCANCODE_D:
      strcat(playerName, "d");
      break;
    case SCANCODE_E:
      strcat(playerName, "e");
      break;
    case SCANCODE_F:
      strcat(playerName, "f");
      break;
    case SCANCODE_G:
      strcat(playerName, "g");
      break;
    case SCANCODE_H:
      strcat(playerName, "h");
      break;
    case SCANCODE_I:
      strcat(playerName, "i");
      break;
    case SCANCODE_J:
      strcat(playerName, "j");
      break;
    case SCANCODE_K:
      strcat(playerName, "k");
      break;
    case SCANCODE_L:
      strcat(playerName, "l");
      break;
    case SCANCODE_M:
      strcat(playerName, "m");
      break;
    case SCANCODE_N:
      strcat(playerName, "n");
      break;
    case SCANCODE_O:
      strcat(playerName, "o");
      break;
    case SCANCODE_P:
      strcat(playerName, "p");
      break;
    case SCANCODE_Q:
      strcat(playerName, "q");
      break;
    case SCANCODE_R:
      strcat(playerName, "r");
      break;
    case SCANCODE_S:
      strcat(playerName, "s");
      break;
    case SCANCODE_T:
      strcat(playerName, "t");
      break;
    case SCANCODE_U:
      strcat(playerName, "u");
      break;
    case SCANCODE_V:
      strcat(playerName, "v");
      break;
    case SCANCODE_W:
      strcat(playerName, "w");
      break;
    case SCANCODE_X:
      strcat(playerName, "x");
      break;
    case SCANCODE_Y:
      strcat(playerName, "y");
      break;
    case SCANCODE_Z:
      strcat(playerName, "z");
      break;
    default:
      break;
    }
  }
}

void handleKeyboardOptionsMenu(char* playerName, uint8_t scanCode) {
  convertScanCodeToChar(playerName, scanCode);
}

STATE menuControllerHandle(Menu *menu,char* playerName,DEVICE interruptType, const struct packet *packet, uint8_t scanCode, unsigned elapsed) {
  switch (interruptType) {
    case TIMER:
      if (elapsed % (sys_hz() / 25) == 0) {
        if (drawMenu(menu,elapsed,playerName)) {
          printf("Error drawing menu\n");
          return OVER;
        }
        if(swap_buffers()){
          printf("Error swapping buffers\n");
          return OVER;
        }
         
        
      }
      break;
    case MOUSE:
      // Update mouse position
      menu->mouse->pos.x += packet->delta_x;
      menu->mouse->pos.y -= packet->delta_y;
      if(menu->mouse->pos.x < 0) menu->mouse->pos.x = 0;
      if(menu->mouse->pos.y < 0) menu->mouse->pos.y = 0;
      if(menu->mouse->pos.x > 1024) menu->mouse->pos.x = 1024;
      if(menu->mouse->pos.y > 768) menu->mouse->pos.y = 768;
      int option;

      // TODO: join this two functions into one, so that we don't have to iterate over the buttons twice
      seeIfMouseIsOverOption(menu, &option);
      updateSelectedOption(menu, option);
      
      if (packet->lb) {
        if (menu->selectedOption != -1) {
          switch (menu->type) {
            case MAIN_MENU:
              return handleMainMenu(menu, menu->selectedOption);
              break;
            case GAME_MODE_MENU:
              return handleGameModeMenu(menu, menu->selectedOption);
              break;
            case INSTRUCTIONS_MENU:
              return handleInstructionsMenu(menu, menu->selectedOption);
              break;
            case OPTIONS_MENU:
              return handleOptionsMenu(menu, menu->selectedOption);
              break;
            case GAME_OVER_MENU:
              return handleGameOverMenu(menu, menu->selectedOption);
              break;
            case ONLINE_MENU:
              return handleOnlineMenu(menu, menu->selectedOption);
              break;
            default:
              printf("Invalid menu type\n");
              return OVER;
          }
        }
      }
      break;
    case KEYBOARD:
      if (scanCode == SCANCODE_ESC){
        return OVER;
      }
      switch(menu->type){
        case OPTIONS_MENU:
          handleKeyboardOptionsMenu(playerName, scanCode);
          break;
        default:
          break;
      }
      break;
    default:
      return OVER;
      break;

  }
  return MENU;
}
