#include <lcom/lcf.h>
#include "controller/timer/timer.h"
#include "controller/video/graphics.h"
#include "controller/keyboard/keyboard.h"
#include "controller/mouse/mouse.h"
#include "controller/rtc/rtc.h"
#include "model/model.h"
#include "view/view.h"
#include "config.h"

extern SystemState systemState;
bool prev_pressed_space = false;
bool projectile_fired = false;
int air_time = 0;

/**
 * @brief This functions serves to configurate the lcf before starting the game loop.
 *
 * @param argc Number of arguments
 * @param argv Arguments
 * @return int 0 if no errors occurred, 1 otherwise
 */
int(main)(int argc, char *argv[])
{
  lcf_set_language("EN-US");
  lcf_trace_calls("/home/lcom/labs/g1/proj/src/debug/trace.txt");
  lcf_log_output("/home/lcom/labs/g1/proj/src/debug/output.txt");
  if (lcf_start(argc, argv))
    return 1;
  lcf_cleanup();
  return 0;
}

/**
 * @brief This function serves to setup the game before starting the game loop. It
 * configures the timer, the video card, the sprites and subscribes the
 * timer, keyboard, mouse and RTC interrupts. Then, it configures and enables
 * the mouse data reporting and starts the RTC.
 *
 * @return int 0 if no errors occurred, 1 otherwise
 */
int setup()
{

  rtc_setup();

  if (timer_set_frequency(TIMER, GAME_FREQUENCY) != 0)
    return 1;

  if (set_frame_buffers(VIDEO_MODE) != 0)
    return 1;

  if (set_graphic_mode(VIDEO_MODE) != 0)
    return 1;

  setup_sprites();

  if (timer_subscribe_interrupts() != 0)
    return 1;
  if (keyboard_subscribe_interrupts() != 0)
    return 1;
  if (mouse_subscribe_interrupts() != 0)
    return 1;
  if (rtc_subscribe_interrupts() != 0)
    return 1;

  if (mouse_write(ENABLE_STREAM_MODE) != 0)
    return 1;
  if (mouse_write(ENABLE_DATA_REPORT) != 0)
    return 1;

  rtc_setup();

  return 0;
}

/**
 * @brief This function serves to shutdown the game after the game loop ends. It
 * exits the graphic mode, destroys the sprites and unsubscribes the timer,
 * keyboard, mouse and RTC interrupts. Then, it disables the mouse data
 * reporting.
 *
 * @return int 0 if no errors occurred, 1 otherwise
 */
int teardown()
{

  if (vg_exit() != 0)
    return 1;

  destroy_sprites();

  if (timer_unsubscribe_interrupts() != 0)
    return 1;
  if (keyboard_unsubscribe_interrupts() != 0)
    return 1;
  if (mouse_unsubscribe_interrupts() != 0)
    return 1;
  if (rtc_unsubscribe_interrupts() != 0)
    return 1;

  if (mouse_write(DISABLE_DATA_REPORT) != 0)
    return 1;

  return 0;
}

/**
 * @brief This function serves to run the game loop. It updates the game state
 * whenever a timer, keyboard, mouse or RTC interrupt occurs. Then, it
 * shut downs the game when the loop control variable is set to EXIT.
 *
 * @param argc Number of arguments
 * @param argv Arguments
 *
 * @return int 0 if no errors occurred, 1 otherwise
 */
int(proj_main_loop)(int argc, char *argv[])
{

  if (setup() != 0)
    return teardown();

  draw_new_frame();

  int ipc_status;
  message msg;
  while (systemState == RUNNING)
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
        if (msg.m_notify.interrupts & TIMER_MASK)
          update_timer_state();
        if (msg.m_notify.interrupts & KEYBOARD_MASK)
          update_keyboard_state();
        if (msg.m_notify.interrupts & MOUSE_MASK)
          update_mouse_state();
        if (msg.m_notify.interrupts & RTC_MASK)
          update_rtc_state();
      }
    }
  }

  if (teardown() != 0)
    return 1;

  return 0;
}
