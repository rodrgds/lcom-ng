#include "control_base.h"

bool isMulti = false;
GameState state = MENU;

int accept_duel(){
  //sends 0xFE that will signal opponent that i am ready, this only happens if i have received challenge (só depois de serial_port já ter recebido oxFE)
  send_char(0xFE);
  state = GAME;
  isMulti = true;
  return 0;
}
