#include "keyboard.h"

/** @defgroup keyboard keyboard
  * @{
 */

int keyboard_hook_id = 1;
uint8_t Kscancode = 0;
uint8_t lista[2];
bool isTwoBytes = false;

/**
 * @brief Subscribes keyboard interrupts.
 * @param bit_no Pointer to bit number to be set with the IRQ line.
 * @return 0 on success, 1 otherwise.
 */
int (keyboard_subscribe_int) (uint8_t *bit_no) {
  if (bit_no == NULL) return 1;
  *bit_no = BIT(keyboard_hook_id);
  return sys_irqsetpolicy(KEYBOARD_IRQ, IRQ_REENABLE|IRQ_EXCLUSIVE, &keyboard_hook_id);
}

/**
 * @brief Unsubscribes keyboard interrupts.
 * @return 0 on success, 1 otherwise.
 */
int (keyboard_unsubscribe_int) () {
  return sys_irqrmpolicy(&keyboard_hook_id);
}

/**
 * @brief Keyboard interrupt handler.
 * Reads the status register and the output buffer.
 */
void (kbd_ih)(){
  uint8_t status;
  if (read_KBC_status(&status) != 0 || status & ST_TIME_OUT_ERROR || 
  status & ST_PARITY_ERROR || status & ST_MOUSE_DATA){
    printf("Error in status");
    return;
  }
  if (status & ST_OUT_FULL_BUFFER){
    if (util_sys_inb(OUT_KBC_CMD, &Kscancode) != 0) return;
  }
}

/**
 * @brief Enables keyboard interrupts.
 * @return 0 on success, 1 otherwise.
 */
int (enableInterrupt)(){
  uint8_t status;
  if (write_KBC_command(IN_KBC_CMD, READ_CMD) != 0) return 1;    
  if (read_KBC_output(OUT_KBC_CMD, &status, false) != 0) return 1;
  if (write_KBC_command(IN_KBC_CMD, WRITE_CMD) != 0) return 1;   
  return (write_KBC_command(IN_KBC_CMD, status | BIT(0)) != 0);
}

/**
 * @brief Packages the keyboard scan code.
 * @return 0 if not to print, 1 to print 1 byte, 2 to print 2 bytes.
 */
int (package_KScanCode)() {
  if (isTwoBytes) {
    lista[0] = TWO_BYTES;
    lista[1] = Kscancode;
    isTwoBytes = false;
    return 2;
  } else if (Kscancode != TWO_BYTES) {
    lista[0] = Kscancode;
    return 1;
  }
  isTwoBytes = true;
  return 0;
}

/**
 * @brief Waits for the ESC key to be pressed, keyboard interrupt handler
 */
void (waitForEsc)() {
  uint8_t kbdIRQ;
  int ipc_status;
  message msg;
  if (keyboard_subscribe_int(&kbdIRQ) != 0) return;
  while (!(Kscancode == ESC_BREAK && IS_BREAK_CODE(Kscancode))) {
    if (driver_receive(ANY, &msg, &ipc_status) != 0) {
      printf("Error");
      continue;
    }
    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          if (msg.m_notify.interrupts & kbdIRQ) {
            kbd_ih();
          }
          break;
        default:
          break;
      }
    }
  }
  if (keyboard_unsubscribe_int() != 0) return;
  return;
}

/**@}*/

