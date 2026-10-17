#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_


#include <lcom/lcf.h>
#include <lcom/utils.h>

#include <stdint.h>

#include "i8042.h"


int kb_hook_id, found_err, data_read_to;
uint8_t kb_data;

int (kbc_subscribe_int)(uint8_t * bit_no);

int (kbc_unsubscribe_int)();

void (kbc_ih)(); 

int (kbc_issue_cmd)(uint8_t kbc_cmd);


#endif /* _KEYBOARD_H_ */
