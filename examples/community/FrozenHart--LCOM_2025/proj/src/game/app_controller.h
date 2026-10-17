#ifndef _LCOM_APP_CONTROLER_H
#define _LCOM_APP_CONTROLER_H

#include "game_config.h"
#include "game_controller.h"
#include "src/drivers/Keyboard_and_Mouse/keyboard.h"
#include "src/drivers/Keyboard_and_Mouse/mouse.h"
#include <lcom/lcf.h>

int app_controller_handle_application_state();
int app_controller_init();

#endif // APP_CONTROLER_H
