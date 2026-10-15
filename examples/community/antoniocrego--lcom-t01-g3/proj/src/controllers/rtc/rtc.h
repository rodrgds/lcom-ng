#ifndef _LCOM_RTC_H_
#define _LCOM_RTC_H_

#include <lcom/lcf.h>
#include <minix/sysutil.h>

/// @brief Sends a command to RTC
/// @param command Command to send
/// @param rtc RTC's output
/// @return Returns 0 on success and 1 otherwise
int get_rtc(uint8_t command, uint8_t *rtc);

#endif
