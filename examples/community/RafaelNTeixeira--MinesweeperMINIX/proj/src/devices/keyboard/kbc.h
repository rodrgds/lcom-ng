#pragma once
#include <lcom/lcf.h>
#include "i8042_kbc.h"

extern uint8_t scan_bytes[2];
extern bool codecomplete_kbd;
extern int nrbytes;

/**
* @brief This function subscribes keyboard interrupts with policies REENABLE and EXCLUSIVE
* @param [out] bit_no All zeros except for the bit position of the keyboard's hook id
* @return Return 0 if case of success, 1 if otherwise
*/
int (kbc_subscribe_int)(uint8_t *bit_no);

/**
* @brief This function unsubscribes keyboard interrupts
* @return Return 0 if case of success, 1 if otherwise
*/
int (kbc_unsubscribe_int)();

/**
* @brief This function reads the status of the KBC
* @param [out] status The current status of the KBC
* @return Return 0 if case of success, 1 if otherwise
*/
int(kbd_get_status)(uint8_t *status);

/**
* @brief This function reads the content in the 0x64 address
* @param [out] output The data present in the 0x64 address
* @return Return 0 if case of success, 1 if otherwise
*/
int (kbd_read_out_buffer)(uint8_t *output);

/**
 * @brief This function verifys if it is possible to read the status of the KBC
 * @param status The current status of the KBC
 * @return Return 0 if case of success, 1 if otherwise
*/
bool (kbc_can_read)(uint8_t *st);

/**
* @brief This function reads the status of the KBC more completely 
* @param data The current status of the KBC
* @return Return 0 if case of success, 1 if otherwise
*/
int (kbc_read)(uint8_t *data);

/**
* @brief Keyboard's interrupt handler
*/
void (kbc_ih)();
