// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>
#include <lcom/liblm.h>
#include <lcom/proj.h>
#include <stdbool.h>
#include <stdint.h>

// Any header files included below this line should have been created by you
#include "game.h"
#include "graphics.h"
#include "i8042.h"
#include "i8254.h"
#include "kbd.h"
#include "mouse.h"
#include "rtc.h"
#include "timer.h"

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/g09/proj/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/g09/proj/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int menuLoop() {
  int ipc_status, r;
  message msg;

  uint8_t kbd_irq = KBD_IRQ;
  kbd_subscribe_int(&kbd_irq);

  uint32_t kbd_code = 1;
  while (kbd_code != BREAK_CODE(KEYCODE_ENTER)) {
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          if (msg.m_notify.interrupts & BIT(kbd_irq)) {
            //Keyboard interrupt
            kbd_code = kbd_read_code();
          }
          break;
        default:
          break;
      }
    }
    else {
    }
  }

  kbd_unsubscribe_int();

  return 0;
}

int gameLoop() {
  int ipc_status, r;
  message msg;
  uint8_t kbd_irq = KBD_IRQ, mouse_irq = MOUSE_IRQ, timer_irq = TIMER0_IRQ;

  timer_set_frequency(1, 30);

  if (enableMouseDataReporting() != OK) {
    printf("Error enabling mouse data Reporting.\n");
  }

  rtc_act_periodic_interrupts(0xF);

  //Sample rate ?
  writeByteToMouse(0xF3);
  writeByteToMouse(0xA);
  kbd_subscribe_int(&kbd_irq);
  mouse_subscribe_int(&mouse_irq);
  timer_subscribe_int(&timer_irq);
  rtc_subscribe_int();

  Game *game = (Game *) malloc(sizeof(Game));
  initGame(game);
  uint32_t kbd_code = 1;
  uint8_t exit = 0;
  while (!exit) {
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          if (msg.m_notify.interrupts & BIT(RTC_IRQ)) {
            uint8_t interrupts = rtc_ih();
            if(interrupts & BIT(0)){
              receivePeriodicInterrupt(game);
            }
            //rtc_create_alarm();
          }
          else {
            exit = kbd_code == BREAK_CODE(KEYCODE_ESC);
          }
          if (msg.m_notify.interrupts & BIT(kbd_irq)) {
            //Keyboard interrupt
            kbd_code = kbd_read_code();
            receiveKeyboardInterrupt(game, kbd_code);
            if (kbd_code == BREAK_CODE(KEYCODE_ESC)) {
              rtc_disable_periodic();
              rtc_delay();
            }
            //exit = kbd_code != BREAK_CODE(KEYCODE_ESC);
          }
          if (msg.m_notify.interrupts & BIT(mouse_irq)) {
            //Mouse interrupt
            mouse_ih();
            if (isMouseDataPacketReady()) {
              struct packet dataPacket = readMouseDataPacket();
              receiveMouseInterrupt(game, dataPacket);
            }
          }
          if (msg.m_notify.interrupts & BIT(timer_irq)) {

            //Timer interrupt
            if (receiveTimerInterrupt(game) != OK) {
              printf("Error in timer interrupt handler\n");
            }
          }

          break;
        default:
          break;
      }
    }
  }
  printf("Exited loop");
  //rtc_disable_periodic();

  kbd_unsubscribe_int();
  mouse_unsubscribe_int();
  timer_unsubscribe_int();
  disableMouseDataReporting();
  rtc_unsubscribe_int();

  return 0;
}

int(proj_main_loop)(int argc, char *argv[]) {
  activateGraphicsMode(GM_800x600);

  printf("\n");

  //Press Enter to exit menu loop
  //menuLoop();

  gameLoop();

  activateTextMode();
  return 0;
}
