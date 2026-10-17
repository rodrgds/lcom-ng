#include "multiplayerController.h"
#include "../labs/serialPort.h"
#include "../labs/rtc.h"

bool multiplayer = false;

STATE handleMultiplayerConnection() {
  if(establish_connection()){
    printf("Connection established\n");
    multiplayer = true;
    set_game_time(0);
    set_round_time(40);
    return PLAYING;
  }
  return MENU;
}


bool get_multiplayer(){
  return multiplayer;
}
