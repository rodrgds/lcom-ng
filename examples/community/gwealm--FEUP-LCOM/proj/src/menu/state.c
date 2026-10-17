#include "menu/state.h"

#include "state/state.h"

#include "menu/controller/menu.h"

void (menu_init)() {
  state_register(MENU, menu_state_start, menu_state_end);
}

void (menu_state_start)() {
  menu_start();
}

void (menu_state_end)() {
  menu_end();
}
