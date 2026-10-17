#ifndef KBD_H
#define KBD_H

#pragma once

/**
 * @brief Subscribes and enables Kbd interrupts
 *
 * @param bit_no address of memory to be initialized with the
 *	  bit number to be set in the mask returned upon an interrupt
 * @return Return 0 upon success and non-zero otherwise
 */
int kbd_subscribe_int(uint8_t *bit_no);

/**
 * @brief Unsubscribes and enables Kbd interrupts
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int (kbd_unsubscribe_int)(void);

/**
 * @brief Keyboard Interrupt Handler
 *  kbc_ih in c
 *  kbc_asm_in un assembly
 *
 */
void (kbc_ih)(void); 

/*void kbc_asm_ih();*/

/**
 * @brief Current make or break code
 * 
 */
 uint32_t kbc_code;

 typedef struct pollReturn{
   bool receive;
   u_int32_t kbc_code;
 } pollReturn;

 pollReturn pret;

 /**
  * 
  */
  void kbd_scan_loop(void);

#endif
