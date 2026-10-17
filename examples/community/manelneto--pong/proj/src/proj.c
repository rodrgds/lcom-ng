#include <lcom/lcf.h>

#include "model/model.h"

#include "view/view.h"

#include "controller/keyboard.h"
#include "controller/mouse.h"
#include "controller/video.h"
#include <lcom/timer.h>

#define FPS 60
#define MODE 0x115

extern State state;

uint8_t timer_irq_set;
uint8_t keyboard_irq_set;
uint8_t mouse_irq_set;

int main(int argc, char *argv[]) {
  lcf_set_language("PT-PT");

  lcf_trace_calls("/home/lcom/labs/proj/debug/trace.txt");

  lcf_log_output("/home/lcom/labs/proj/debug/output.txt");

  if (lcf_start(argc, argv))
    return 1;

  lcf_cleanup();

  return 0;
}

/**
 * @brief Configures the devices as needed and starts the project
 *
 * Sets timer frequency, subscribes interrupts from all devices and changes video graphics mode to the desired one.
 * Starts PONG and draws the first frame.
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int start() {
  if (timer_set_frequency(0, FPS)) {
    printf("%s: timer_set_frequency(0, FPS: %d) error\n", __func__, FPS);
    return 1;
  }

  if (timer_subscribe_int(&timer_irq_set)) {
    printf("%s: timer_subscribe_int(&timer_irq_set) error\n", __func__);
    return 1;
  }

  if (keyboard_subscribe_int(&keyboard_irq_set)) {
    printf("%s: keyboard_subscribe_int(&keyboard_irq_set) error\n", __func__);
    return 1;
  }

  if (mouse_data_reporting(true)) {
    printf("%s: mouse_data_reporting(true) error\n", __func__);
    return 1;
  }

  if (mouse_subscribe_int(&mouse_irq_set)) {
    printf("%s: mouse_subscribe_int(&mouse_irq_set) error\n", __func__);
    return 1;
  }

  if (!video_init(MODE)) {
    printf("%s: vg_init(mode: 0x%x) error\n", __func__, MODE);
    return 1;
  }

  if (start_pong()) {
    printf("%s: start_pong() error\n", __func__);
    return 1;
  }

  if (draw_frame()) {
    printf("%s: draw_frame() error\n", __func__);
    return 1;
  }

  return 0;
}

/**
 * @brief Receives and handles the subscribed interrupts, while in the game state
 */
void loop() {
  int ipc_status, r;
  message msg;

  while (state != END) {
    if ((r = driver_receive(ANY, &msg, &ipc_status))) {
      printf("%s: driver_receive failed with: %d\n", __func__, r);
      continue;
    }

    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          if (msg.m_notify.interrupts & BIT(timer_irq_set))
            timer_interrupt_handler();
          if (msg.m_notify.interrupts & BIT(keyboard_irq_set))
            keyboard_interrupt_handler();
          if (msg.m_notify.interrupts & BIT(mouse_irq_set))
            mouse_interrupt_handler();
      }
    }
  }
}

/**
 * @brief Unsubscribes interrupts from all devices and ends the project
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int end() {
  end_pong();

  if (vg_exit()) {
    printf("%s: vg_exit() error\n", __func__);
    return 1;
  }

  if (mouse_unsubscribe_int()) {
    printf("%s: mouse_unsubscribe_int() error\n", __func__);
    return 1;
  }

  if (mouse_data_reporting(false)) {
    printf("%s: mouse_data_reporting(false) error\n", __func__);
    return 1;
  }

  if (keyboard_unsubscribe_int()) {
    printf("%s: keyboard_unsubscribe_int() error\n", __func__);
    return 1;
  }

  if (timer_unsubscribe_int()) {
    printf("%s: timer_unsubscribe_int() error\n", __func__);
    return 1;
  }

  return 0;
}

/**
 * @brief LCOM Project main loop
 *
 * Runs the project.
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int(proj_main_loop)(int argc, char *argv[]) {
  if (start()) {
    printf("%s: start() error\n", __func__);
    if (end()) {
      printf("%s: end() error\n", __func__);
      return 1;
    }
    return 1;
  }

  loop();

  if (end()) {
    printf("%s: end() error\n", __func__);
    return 1;
  }

  return 0;
}
