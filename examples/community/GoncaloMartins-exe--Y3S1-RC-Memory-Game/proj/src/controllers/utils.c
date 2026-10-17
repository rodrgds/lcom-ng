/**
 * @file utils.c
 * @brief Utility functions for LCF-based keyboard and cursor handling.
 */

 #include <lcom/lcf.h>
 #include <stdint.h>
 #include <minix/drivers.h>
 #include <minix/sysutil.h>
 
 #include "utils.h"
 #include "../sprites/sprites.h"
 #include "video/graphic.h"
 
 #define CURSOR_SPEED 10 ///< Pixels to move per key press
 
 extern uint32_t counter_KBC;
 uint8_t scancode;
 extern uint8_t last_keycode;
 extern int cursor_x, cursor_y; ///< Global cursor position
 
 int cursor_x = 400; ///< Initial X position of cursor
 int cursor_y = 400; ///< Initial Y position of cursor
 
 /**
  * @brief Get least significant byte of a 16-bit value.
  * 
  * @param val 16-bit value
  * @param lsb Pointer to store least significant byte
  * @return 0 on success
  */
 int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
   *lsb = (uint8_t)(val & 0xFF);
   return 0;
 }
 
 /**
  * @brief Get most significant byte of a 16-bit value.
  * 
  * @param val 16-bit value
  * @param msb Pointer to store most significant byte
  * @return 0 on success
  */
 int(util_get_MSB)(uint16_t val, uint8_t *msb) {
   *msb = (uint8_t)(val >> 8);
   return 0;
 }
 
 /**
  * @brief Read a byte from a given port using sys_inb.
  * 
  * @param port Port to read from
  * @param value Pointer to store read byte
  * @return 0 on success, 1 on failure
  */
 int (util_sys_inb)(int port, uint8_t *value) {
   uint32_t temp = 0;
   if (sys_inb(port, &temp) != OK) {
     printf("Error in sys_inb for port 0x%x\n", port);
     return 1;
   }
   *value = (uint8_t)(temp & 0xFF);
   counter_KBC++;
   return 0;
 }
 
 /**
  * @brief Poll the keyboard controller and read output if available.
  * 
  * @param data Pointer to store the read byte
  * @return 0 on success, 1 on failure or no data
  */
 int poll_kbc(uint8_t *data) {
   uint8_t status;
   if (util_sys_inb(STATUS_REG, &status) != 0) return 1;
 
   if (status & OBF) {
     if ((status & (PAR_ERR | TO_ERR)) == 0) {
       return util_sys_inb(OUT_BUF, data);
     }
   }
   return 1;
 }
 
 /**
  * @brief Enable keyboard interrupts by setting the appropriate bit in the command byte.
  * 
  * @return 0 on success, 1 on failure
  */
 int enable_kbd_interrupts() {
   uint8_t cmd_byte;
 
   // Request the KBC command byte
   if (kbc_send_cmd(KBC_CMD_REG, KBC_READ_CMD) != 0) return 1;
   if (util_sys_inb(OUT_BUF, &cmd_byte) != 0) return 1;
 
   // Enable keyboard interrupts (bit 0)
   cmd_byte |= BIT(0);
 
   // Write the modified command byte back to the KBC
   if (kbc_send_cmd(KBC_CMD_REG, KBC_WRITE_CMD) != 0) return 1;
   return sys_outb(OUT_BUF, cmd_byte);
 }
 
 /**
  * @brief Move the cursor position based on arrow key input.
  * 
  * @param keycode Scancode of the pressed key
  */
 void update_cursor_position(uint8_t keycode) {
   int step = 10; // Movement step in pixels
 
   switch (keycode) {
       case 0x48: // Up arrow
           cursor_y -= step;
           if (cursor_y < 0) cursor_y = 0;
           break;
       case 0x50: // Down arrow
           cursor_y += step;
           if (cursor_y > mode_info.YResolution - mouseCursorSprite->height)
               cursor_y = mode_info.YResolution - mouseCursorSprite->height;
           break;
       case 0x4B: // Left arrow
           cursor_x -= step;
           if (cursor_x < 0) cursor_x = 0;
           break;
       case 0x4D: // Right arrow
           cursor_x += step;
           if (cursor_x > mode_info.XResolution - mouseCursorSprite->width)
               cursor_x = mode_info.XResolution - mouseCursorSprite->width;
           break;
       default:
           break;
   }
 }
 
 /**
  * @brief Waits for the ESC key to be pressed using keyboard interrupts.
  * 
  * @return 0 when ESC is pressed, 1 on failure
  */
 int ESC_wait() {
   int ipc_status;
   message msg;
   uint8_t irq_set;
 
   // Subscribe to keyboard interrupts
   if (kbd_subscribe_int(&irq_set) != 0) {
     printf("Error: Failed to subscribe to keyboard interrupts\n");
     return 1;
   }
 
   printf("Waiting for ESC key... (IRQ set: %d)\n", irq_set);
 
   while (last_keycode != BREAK_ESC) {
 
     if (driver_receive(ANY, &msg, &ipc_status)!= 0) {
       continue;
     }
 
     if (is_ipc_notify(ipc_status)) {
       switch (_ENDPOINT_P(msg.m_source)) {
         case HARDWARE:
           if (msg.m_notify.interrupts & irq_set) {
             // Call keyboard interrupt handler
             kbd_ih();
           }
           break;
         default:
           continue;
       }
     } else {
       // Handle other types of messages if needed
       printf("Received non-notification message from endpoint %d\n", msg.m_source);
     }
   }
 
   // Unsubscribe from keyboard interrupts
   if (kbd_unsubscribe_int() != 0){
     printf("Error: Failed to unsubscribe from keyboard interrupts\n");
     return 1;
   }
 
   return 0;
 }
