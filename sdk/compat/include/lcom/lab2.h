#ifndef LCOM_NG_FORWARD_LCOM_LAB2
#define LCOM_NG_FORWARD_LCOM_LAB2
#include <lcom/compat.h>
int timer_test_read_config(uint8_t timer, enum timer_status_field field);
int timer_test_time_base(uint8_t timer, uint32_t freq);
int timer_test_int(uint8_t time);
#endif
