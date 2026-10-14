/* A local copy of the FEUP timer contract, as found in student workspaces. */
#ifndef __TIMER_H
#define __TIMER_H
#include <stdint.h>
#include <stdbool.h>
enum timer_init { INVAL_val, LSB_only, MSB_only, MSB_after_LSB };
enum timer_status_field { tsf_all, tsf_initial, tsf_mode, tsf_base };
union timer_status_field_val { uint8_t byte; enum timer_init in_mode; uint8_t count_mode; bool bcd; };
int timer_set_frequency(uint8_t timer, uint32_t freq);
#endif
