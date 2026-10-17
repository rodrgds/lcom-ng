#include "Game.h"
#include "labs/graphics.h"
#include "labs/kbc.h"
#include "labs/keyboard.h"
#include "labs/mouse.h"
#include "labs/rtc.h"
#include "labs/serialPort.h"
#include "labs/timer.h"
#include "model/vector.h"
#include "xpms/ball.xpm"

#include "viewer/cueViewer.h"
#include "viewer/lineViewer.h"

#include "States/states.h"

#include "controllers/menuController.h"
#include "controllers/multiplayerController.h"
#include "controllers/playingController.h"

#include "resources.h"

int initGame() {
  Resources *resources = loadResources();
  printf("created resources\n");

  return gameLoop(resources);
}

int gameLoop(Resources *resources) {
  mapMemory(0x118);
  changeMode(0x118);

  int r;
  message msg;
  int ipc_status;

  mouse_enable_data_reporting();
  uint8_t mouse_iqr;
  if (mouse_subscribe(&mouse_iqr))
    return 1;
  uint8_t keyboard_iqr;
  if (keyboard_subscribe(&keyboard_iqr))
    return 1;
  uint8_t timer_iqr;
  if (timer_subscribe_int(&timer_iqr))
    return 1;
  uint8_t rtc_iqr;
  if (rtc_subscribe(&rtc_iqr))
    return 1;
  uint8_t sp_iqr;
  if (sp_subscribe(&sp_iqr))
    return 1;

  while (resources->state != OVER) {
    STATE state = resources->state;
    /* Get a request message. */
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }

    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:

          if (msg.m_notify.interrupts & timer_iqr) {
            timer_int_handler();
            switch (state) {
              case MENU:
                resources->state = menuControllerHandle(resources->menu, resources->playerName, TIMER, NULL, 0, get_elapsed());
                break;
              case PLAYING:
                resources->state = playingControllerHandle(resources->table, TIMER, NULL, 0, get_elapsed());
                if (resources->state == MENU) {
                  printf("MENU REsturns\n");
                  resources->menu = newGameOverMenu(resources->table->winningPlayerName);
                  destroyTable(resources->table);
                }
                break;
              default:
                break;
            }
          }

          if (msg.m_notify.interrupts & mouse_iqr) {
            mouse_ih();
            if (mouseError())
              return 1;
            if (isPacketComplete()) {
              struct packet packet = getMousePacket();
              switch (state) {
                case MENU:
                  resources->state = menuControllerHandle(resources->menu, resources->playerName, MOUSE, &packet, 0, 0);
                  if (resources->state == PLAYING) {
                    resources->table = newTable();
                  }
                  break;
                case PLAYING:
                  resources->state = playingControllerHandle(resources->table, MOUSE, &packet, 0, 0);
                  if (resources->state == MENU) {
                    resources->menu = newGameOverMenu(resources->table->winningPlayerName);
                    destroyTable(resources->table);
                  }
                  break;
                default:
                  break;
              }
            }
          }
          if (msg.m_notify.interrupts & keyboard_iqr) {
            kbc_ih();
            if (keyboardIHError())
              return 1;
            uint8_t scancode = getKeyboardScancode();
            printf("The received scanCode is: %x\n", scancode);
            switch (state) {
              case MENU:
                resources->state = menuControllerHandle(resources->menu, resources->playerName, KEYBOARD, NULL, scancode, 0);
                break;
              case PLAYING:
                resources->state = playingControllerHandle(resources->table, KEYBOARD, NULL, scancode, 0);
                break;
              default:
                break;
            }
          }

          if (msg.m_notify.interrupts & rtc_iqr) {
            rtc_ih();
            switch (state) {
              case PLAYING:
                resources->state = playingControllerHandle(resources->table, RTC, NULL, 0, 0);
                break;
              default:
                break;
            }
          }

          if (msg.m_notify.interrupts & sp_iqr) {
            sp_ih();
            printf("Interrupt received\n");
            switch (state) {
              case MENU:
                if (resources->menu->type == ONLINE_MENU) {
                  resources->state = handleMultiplayerConnection();
                  printf("The state is %d\n", resources->state);
                  if (resources->state == PLAYING) {
                    printf("Creating new table\n");
                    resources->table = newTable();
                  }
                }
                else {
                  printf("Clearing serial port\n");
                  sp_clear();
                }
                break;
              case PLAYING:
                break;
              default:
                sp_clear();
                break;
            }
          }

        default:
          break; /* no other notifications expected: do nothing */
      }
    }
  }
  printf("Closing lab stuff\n");
  if (sp_unsubscribe())
    return 1;

  if (mouse_unsubscribe())
    return 1;
  if (disableDataReporting())
    return 1;
  if (keyboard_unsubscribe())
    return 1;
  if (timer_unsubscribe_int())
    return 1;

  if (rtc_unsubscribe())
    return 1;

  printf("Exiting video mode\n");
  if (vg_exit())
    return 1;
  printf("Cleaning resources\n");
  destroyResources(resources);
  printf("Everything finished\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
  return 0;
}
