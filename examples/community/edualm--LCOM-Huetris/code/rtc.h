#ifndef RTC_H_
#define RTC_H_

#define RTC_ADDR_REG            0x70
#define RTC_DATA_REG            0x71

#define RTC_LOC_SECONDS			0x0
#define RTC_LOC_MINUTES			0x2
#define RTC_LOC_HOURS			0x4

#define RTC_REG_A               0xA
#define RTC_REG_B               0xB

#define RTC_IRQ					0x8

#define REG_UIP_BIT				7

/* Bits for Reg B */

#define RTC_SQWE_BIT			3
#define RTC_UIE_BIT				4

/** @defgroup rtc rtc
 * @{
 * Functions for controlling the real-time clock.
 */

/**
 * @brief Waits for a valid response from the RTC.
 */

void rtc_wait_valid(void);

/**
 * @brief Reads from the RTC.
 *
 * @param reg The register to read from.
 * @param value The response from the KBC.
 * @return 0 on successful execution, undefined if otherwise.
 */

int rtc_read_register(unsigned long reg, unsigned long* value);

/**
 * @brief Writes to the RTC.
 *
 * @param reg The register to read from.
 * @param value The value to write.
 * @return 0 on successful execution, undefined if otherwise.
 */

int rtc_write_register(unsigned long reg, unsigned long value);

/**@}*/

#endif /* RTC_H_ */