#ifndef __PROJ_RTC_H
#define __PROJ_RTC_H

#include <stdbool.h>
#include <stdint.h>

/** @defgroup rtc rtc
 * @{
 *
 * Functions for using the Real-Time Clock (RTC)
 */

/* RTC hardware definitions */
#define RTC_IRQ 8         /**< @brief RTC IRQ line */
#define RTC_ADDR_REG 0x70 /**< @brief RTC address register */
#define RTC_DATA_REG 0x71 /**< @brief RTC data register */

/* RTC register addresses */
#define RTC_ADDR_SECONDS 0      /**< @brief Seconds register */
#define RTC_ADDR_SEC_ALARM 1    /**< @brief Seconds alarm register */
#define RTC_ADDR_MINUTES 2      /**< @brief Minutes register */
#define RTC_ADDR_MIN_ALARM 3    /**< @brief Minutes alarm register */
#define RTC_ADDR_HOUR 4         /**< @brief Hour register */
#define RTC_ADDR_HOUR_ALARM 5   /**< @brief Hour alarm register */
#define RTC_ADDR_DAY_OF_WEEK 6  /**< @brief Day of week register */
#define RTC_ADDR_DAY_OF_MONTH 7 /**< @brief Day of month register */
#define RTC_ADDR_MONTH 8        /**< @brief Month register */
#define RTC_ADDR_YEAR 9         /**< @brief Year register */

/* RTC control registers */
#define RTC_ADDR_A 10 /**< @brief Register A (rate selection) */
#define RTC_ADDR_B 11 /**< @brief Register B (control) */
#define RTC_ADDR_C 12 /**< @brief Register C (flags) */
#define RTC_ADDR_D 13 /**< @brief Register D (battery status) */

/* Register A bit definitions */
#define RTC_REG_A_UPDATE_IP BIT(7) /**< @brief Update in progress flag */
#define RTC_REG_A_RS3 BIT(3)       /**< @brief Rate selector bit 3 */
#define RTC_REG_A_RS2 BIT(2)       /**< @brief Rate selector bit 2 */
#define RTC_REG_A_RS1 BIT(1)       /**< @brief Rate selector bit 1 */
#define RTC_REG_A_RS0 BIT(0)       /**< @brief Rate selector bit 0 */

/* Register B bit definitions */
#define RTC_REG_B_INHIBIT_UPDATES BIT(7) /**< @brief Set bit to disable updates */
#define RTC_REG_B_PERIODIC_IE BIT(6)     /**< @brief Periodic interrupt enable */
#define RTC_REG_B_ALARM_IE BIT(5)        /**< @brief Alarm interrupt enable */
#define RTC_REG_B_UPDATE_IE BIT(4)       /**< @brief Update interrupt enable */
#define RTC_REG_B_SQW BIT(3)             /**< @brief Square wave enable */
#define RTC_REG_B_DM_BINARY BIT(2)       /**< @brief Data mode (1=binary, 0=BCD) */
#define RTC_REG_B_24_12 BIT(1)           /**< @brief 24/12 hour mode (1=24h, 0=12h) */
#define RTC_REG_B_DSE BIT(0)             /**< @brief Daylight savings enable */

/* Register C bit definitions */
#define RTC_REG_C_IRQF BIT(7) /**< @brief Interrupt request flag */
#define RTC_REG_C_PF BIT(6)   /**< @brief Periodic interrupt flag */
#define RTC_REG_C_AF BIT(5)   /**< @brief Alarm interrupt flag */
#define RTC_REG_C_UF BIT(4)   /**< @brief Update interrupt flag */

/* Register D bit definitions */
#define RTC_REG_D_VALID_RT BIT(7) /**< @brief Valid RAM and time flag */

/* Periodic interrupt rates (in Hz) */
#define RTC_RATE_NONE 0x00    /**< @brief No periodic interrupts */
#define RTC_RATE_8192_HZ 0x03 /**< @brief 8192 Hz */
#define RTC_RATE_4096_HZ 0x04 /**< @brief 4096 Hz */
#define RTC_RATE_2048_HZ 0x05 /**< @brief 2048 Hz */
#define RTC_RATE_1024_HZ 0x06 /**< @brief 1024 Hz */
#define RTC_RATE_512_HZ 0x07  /**< @brief 512 Hz */
#define RTC_RATE_256_HZ 0x08  /**< @brief 256 Hz */
#define RTC_RATE_128_HZ 0x09  /**< @brief 128 Hz */
#define RTC_RATE_64_HZ 0x0A   /**< @brief 64 Hz */
#define RTC_RATE_32_HZ 0x0B   /**< @brief 32 Hz */
#define RTC_RATE_16_HZ 0x0C   /**< @brief 16 Hz */
#define RTC_RATE_8_HZ 0x0D    /**< @brief 8 Hz */
#define RTC_RATE_4_HZ 0x0E    /**< @brief 4 Hz */
#define RTC_RATE_2_HZ 0x0F    /**< @brief 2 Hz */

/* Common values */
#define RTC_DONT_CARE_VALUE (BIT(7) | BIT(6)) /**< @brief Don't care value for alarms */

/**
 * @brief Structure to hold RTC time information
 */
typedef struct {
  uint8_t seconds; /**< @brief Seconds (0-59) */
  uint8_t minutes; /**< @brief Minutes (0-59) */
  uint8_t hours;   /**< @brief Hours (0-23 in 24h mode) */
  uint8_t day;     /**< @brief Day of month (1-31) */
  uint8_t month;   /**< @brief Month (1-12) */
  uint8_t year;    /**< @brief Year (0-99, represents 2000-2099) */
} rtc_time_t;

/**
 * @brief Enumeration for RTC interrupt types
 */
typedef enum {
  RTC_UPDATE_INT,  /**< @brief Update interrupt */
  RTC_ALARM_INT,   /**< @brief Alarm interrupt */
  RTC_PERIODIC_INT /**< @brief Periodic interrupt */
} rtc_interrupt_type_t;

/**
 * @brief Subscribes and enables RTC interrupts
 *
 * @param bit_no Address of memory to be initialized with the
 * bit number to be set in the mask returned upon an interrupt
 * @return Return 0 upon success and non-zero otherwise
 */
int(rtc_subscribe_int)(uint8_t *bit_no);

/**
 * @brief Unsubscribes RTC interrupts
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int(rtc_unsubscribe_int)(void);

/**
 * @brief RTC interrupt handler
 *
 * Reads register C to clear pending interrupts and handles the interrupt
 */
void(rtc_ih)(void);

/**
 * @brief Enables the specified type of RTC interrupt
 *
 * @param interrupt_type The type of interrupt to enable
 * @return Return 0 upon success and non-zero otherwise
 */
int(rtc_enable_interrupt)(rtc_interrupt_type_t interrupt_type);

/**
 * @brief Disables the specified type of RTC interrupt
 *
 * @param interrupt_type The type of interrupt to disable
 * @return Return 0 upon success and non-zero otherwise
 */
int(rtc_disable_interrupt)(rtc_interrupt_type_t interrupt_type);

/**
 * @brief Disables all RTC interrupts
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int(rtc_disable_all_interrupts)(void);

/**
 * @brief Sets up periodic interrupts at the specified rate
 *
 * @param rate The periodic interrupt rate (use RTC_RATE_* constants)
 * @return Return 0 upon success and non-zero otherwise
 */
int(rtc_set_periodic_rate)(uint8_t rate);

/**
 * @brief Reads the current time from the RTC
 *
 * @param time Pointer to rtc_time_t structure to store the current time
 * @return Return 0 upon success and non-zero otherwise
 */
int(rtc_read_time)(rtc_time_t *time);

/**
 * @brief Sets an alarm for a specific time
 *
 * @param alarm_time Pointer to rtc_time_t structure with alarm time
 *                   (only hours, minutes, and seconds are used)
 * @return Return 0 upon success and non-zero otherwise
 */
int(rtc_set_alarm)(const rtc_time_t *alarm_time);

/**
 * @brief Sets an alarm to trigger every second
 *
 * This is useful for creating timers with second precision.
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int(rtc_set_alarm_every_second)(void);

/**
 * @brief Checks if the RTC is in a valid state
 *
 * @return True if RTC is valid, false otherwise
 */
bool(rtc_is_valid)(void);

/**
 * @brief Waits until the RTC is not updating
 *
 * This function should be called before reading time values to ensure
 * they are not being updated while reading.
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int(rtc_wait_update_complete)(void);

/**
 * @brief Gets the last interrupt type that occurred
 *
 * This function reads register C to determine which type of interrupt
 * occurred and clears the interrupt flags.
 *
 * @return The interrupt type that occurred, or -1 if no interrupt
 */
int(rtc_get_interrupt_type)(void);

/**
 * @brief Reads a specific RTC register
 *
 * @param reg Register address to read from
 * @param value Pointer to store the read value
 * @return Return 0 upon success and non-zero otherwise
 */
int(rtc_read_register_value)(uint8_t reg, uint8_t *value);

/**
 * @brief Converts BCD value to binary
 *
 * @param bcd_value The BCD value to convert
 * @return The binary equivalent
 */
uint8_t(rtc_bcd_to_binary)(uint8_t bcd_value);

/**
 * @brief Converts binary value to BCD
 *
 * @param binary_value The binary value to convert
 * @return The BCD equivalent
 */
uint8_t(rtc_binary_to_bcd)(uint8_t binary_value);

/**@}*/

#endif /* __PROJ_RTC_H */
