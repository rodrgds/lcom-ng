#ifndef _LCOM_RTC_MACROS_H_
#define _LCOM_RTC_MACROS_H_

#include <minix/sysutil.h>
#include <lcom/lcf.h>

#define RTC_IRQ 8
#define ADDR_REG 0x70
#define DATA_REG 0x71
#define REG_SEC_ALARM 0x01
#define REG_MIN_ALARM 0x03
#define REG_HOUR 0x04
#define REG_HOUR_ALARM 0x05
#define REG_A 0x0A
#define REG_B 0x0B
#define REG_C 0x0C
#define UIP BIT(7)
#define SET BIT(7)
#define AIE BIT(5)
#define UIE BIT(4)
#define AF BIT(5)
#define UF BIT(4)

#endif

