#ifndef _LCOM_KBC_H_
#define _LCOM_KBC_H_

#include <lcom/lcf.h>

int (kb_subscribe_int)(uint8_t *bit_no);

int (kb_unsubscribe_int)();

int (read_kbc_status)(uint8_t* kbc_status);

int (read_kbc_output)(uint8_t port, bool mouse, uint8_t* buffer_content);

int (write_kbc_input)(uint8_t port, uint8_t buffer_content);

void (kbc_ih)();

int (enable_kbc_interrupts)();

int8_t (kbc_get_scan_code)();

void (process_kb_interrupt)();

#endif /* _LCOM_KBC_H_ */
