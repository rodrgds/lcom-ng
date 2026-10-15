#ifndef _LCOM_RTC_H_
#define _LCOM_RTC_H_

#include <lcom/lcf.h>
#include <stdint.h>

/* --- IRQ --- */
#define RTC_MASK            5
#define IRQ_RTC             8

/* --- I/O Ports --- */
#define REGISTER_INPUT      0x70  // Address / command port
#define REGISTER_OUTPUT     0x71  // Data port

/* --- RTC Register Indices --- */
#define SECONDS             0
#define MINUTES             2
#define HOURS               4
#define DAY                 7
#define MONTH               8
#define YEAR                9
#define REGISTER_UPDATING   10    // Register A: UIP flag
#define REGISTER_COUNTING   11    // Register B: Data Mode flag, interrupt enables
#define REGISTER_C          12    // Must be read on every IRQ to re-arm the line

/* --- Status Bits --- */
#define BINARY              BIT(2)  // Data Mode bit in Register B: 1=binary, 0=BCD
#define UPDATING            BIT(7)  // UIP bit in Register A: 1=busy, do not read

/* --- Structs --- */

/** Full date+time snapshot produced by rtc_get_time(). */
typedef struct {
    uint8_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hours;
    uint8_t minutes;
    uint8_t seconds;
} real_time_info;

/** Compatibility struct kept for the original lab1 test program. */
typedef struct {
    uint8_t day;
    uint8_t month;
    uint8_t year;
} rtc_date;

/* --- Public API --- */

/**
 * @brief Detects binary/BCD mode and reads the initial time into the internal buffer.
 * Must be called once before rtc_get_time().
 */
void rtc_setup(void);

/**
 * @brief Subscribes RTC periodic interrupts.
 * @return 0 on success, non-zero otherwise.
 */
int rtc_subscribe_interrupts(void);

/**
 * @brief Unsubscribes RTC periodic interrupts.
 * @return 0 on success, non-zero otherwise.
 */
int rtc_unsubscribe_interrupts(void);

/**
 * @brief RTC interrupt handler. Must be called on every RTC IRQ to read
 * Register C and re-arm the interrupt line.
 */
void rtc_ih(void);

/**
 * @brief Writes a register index and reads back the corresponding value.
 * @param command  Register index to select (e.g. SECONDS, REGISTER_COUNTING).
 * @param output   Pointer to store the byte read from REGISTER_OUTPUT.
 * @return 0 on success, 1 on I/O error.
 */
int rtc_output(uint8_t command, uint8_t *output);

/** @return Non-zero while the RTC is updating its internal registers. */
int rtc_is_updating(void);

/** @return Non-zero if the RTC is in binary (non-BCD) mode. */
int rtc_is_binary(void);

/** @return Non-zero if the RTC is in BCD mode. */
int rtc_is_bcd(void);

/**
 * @brief Reads all six time fields from the RTC into the internal buffer.
 * @return 0 on success, 1 on error.
 */
int rtc_update_time(void);

/**
 * @brief Converts a packed BCD byte to its binary equivalent.
 * @param bcd_number  Packed BCD value (e.g. 0x26 → 26).
 */
uint8_t to_binary(uint8_t bcd_number);

/**
 * @brief Returns a read-only pointer to the last time snapshot.
 * Call rtc_update_time() first to refresh the snapshot.
 */
const real_time_info *rtc_get_time(void);

/**
 * Reads the current date from the RTC and fills the provided `rtc_date`
 * structure. Returns 0 on success, non-zero on failure.
 *
 * This function takes care of ensuring data consistency by checking the Update
 * In Progress (UIP) flag before each read. It also should check the RTC
 * configuration and perform data conversions if necessary (e.g. BCD to binary).
 */
int rtc_read_date(rtc_date *date);

#endif /* _LCOM_RTC_H_ */
