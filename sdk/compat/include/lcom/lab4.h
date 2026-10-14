#ifndef LCOM_NG_FORWARD_LCOM_LAB4
#define LCOM_NG_FORWARD_LCOM_LAB4
#include <lcom/compat.h>
void mouse_ih(void);
int mouse_enable_data_reporting(void);
int mouse_test_packet(uint32_t count);
int mouse_test_remote(uint16_t period, uint8_t count);
int mouse_test_async(uint8_t idle);
int mouse_test_gesture(uint8_t x_len, uint8_t tolerance);
#endif
