#ifndef LCOM_NG_FORWARD_LCOM_LAB3
#define LCOM_NG_FORWARD_LCOM_LAB3
#include <lcom/compat.h>
void kbc_ih(void);
void kbd_ih(void);
int kbd_test_scan(void);
int kbd_test_poll(void);
int kbd_test_timed_scan(uint8_t n);
#endif
