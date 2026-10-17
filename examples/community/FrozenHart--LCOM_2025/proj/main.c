#include <lcom/lcf.h>
#include <lcom/proj.h>
#include "src/drivers/Keyboard_and_Mouse/keyboard.h"
#include <lcom/timer.h>
#include "src/game/app_controller.h"
#include "src/game/IO_controller.h"
#include "src/drivers/Graphics/sprite.h"

// Any header files included below this line should have been created by you

uint8_t irq_set_TIMER, irq_set_KBC, irq_set_MOUSE, irq_set_RTC;
extern application_state_t app_state;
extern game_action_state_t game_action_state;
int timer_interrupts = 0;
extern struct packet mouse_packet;
extern Sprite cursor_sprite;

int main(int argc, char *argv[])
{
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/Lcom2025/proj/src/log/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/Lcom2025/proj/src/log/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int start()
{
  // init app
  if (app_controller_init() != 0)
  {
    printf("IO_controller_init failed\n");
    return 1;
  }
  // init vidio
  if (IO_controller_init() != 0)
  {
    printf("IO_controller_init failed\n");
    return 1;
  }
  // subscribes
  if (keyboard_subscribe_int(&irq_set_KBC) != 0)
  {
    printf("keyboard_subscribe_int failed\n");
    return 1;
  }

  if (mouse_subscribe_int(&irq_set_MOUSE) != 0)
  {
    printf("timer_subscribe_int failed\n");
    return 1;
  }

  if (timer_subscribe_int(&irq_set_TIMER) != 0)
  {
    printf("timer_subscribe_int failed\n");
    return 1;
  }
  return 0;
}

int stop()
{
  // close vidio
  if (IO_controller_close() != 0)
  {
    printf("IO_controller_init failed\n");
    return 1;
  }

  // unsubscribe
  if (keyboard_unsubscribe_int() != 0)
  {
    printf("keyboard_unsubscribe_int failed\n");
    return 1;
  }

  if (timer_unsubscribe_int() != 0)
  {
    printf("timer_unsubscribe_int failed\n");
    return 1;
  }

  if( mouse_unsubscribe_int() != 0)
  {
    printf("mouse_unsubscribe_int failed\n");
    return 1;
  }
  return 0;
}

int(app_main_loop)()
{
  int ipc_status;
  message msg;

  while (app_state != EXIT)
  {
    if (driver_receive(ANY, &msg, &ipc_status) != 0)
    {
      printf("Error");
      continue;
    }

    if (is_ipc_notify(ipc_status))
    {
      switch (_ENDPOINT_P(msg.m_source))
      {
      case HARDWARE:
        if (msg.m_notify.interrupts & irq_set_KBC)
        {
          keyboard_ih();
          if (IO_controller_keyboard_handler() != 0)
          {
            printf("Failed IO_controller_keyboard_handler\n");
          }
        }
        if (msg.m_notify.interrupts & irq_set_TIMER)
        {
          timer_interrupts++;
          if (timer_interrupts % (60 / PPS) == 0)
          {
            if (app_controller_handle_application_state() != 0)
            {
              printf("app_controller_handle_application_state failed\n");
              return 1;
            }
          }
          if (timer_interrupts % (60 / FPS) == 0)
          {
            if (IO_controller_display_frame() != 0)
            {
              printf("IO_controller_display_frame failed\n");
              return 1;
            }
          }
        }
        if (msg.m_notify.interrupts & irq_set_MOUSE)
        {
          mouse_ih();
          if (IO_controller_mouse_handler() != 0)
          {
            printf("IO_controller_mouse_handler failed\n");
            return 1;
          }

        }
        // if (msg.m_notify.interrupts & irq_set_RTC)
        // {

        // }
      }
    }
  }
  return 0;
}

int(proj_main_loop)(int argc, char *argv[])
{
  printf("STARTING PROGRAM \n");
  if (start() != 0)
  {
    printf("Failed to start the application\n");
    return 1;
  }

  if (app_main_loop() != 0)
  {
    printf("Application main loop failed\n");
    if (stop() != 0)
    {
      printf("Failed to stop the application\n");
      return 1;
    }
    return 1;
  }

  if (stop() != 0){
    printf("Failed to stop the application\n");
    return 1;
  }
  printf("CLOSING PROGRAM\n");
  return 0;
}
