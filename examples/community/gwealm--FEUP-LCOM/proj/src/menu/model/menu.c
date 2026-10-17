#include "menu/model/menu.h"

static menu m;

menu *(menu_get)() {
  return &m;
}

void (menu_reset)() {
  memset(&m, 0, sizeof(menu));
}
