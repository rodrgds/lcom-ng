#include "rtc.h"

/** @defgroup rtc rtc
  * @{
 */

int rtc_hook_id = 5;
real_time realTime;

/**
 * @brief Receives a rtc output using a command.
 * @param command Instruction sent to the input register.
 * @param output Output received by the output register.
 * @return 0 in case of success.
 */
int rtc_output(uint8_t c, uint8_t *out) {
    if (sys_outb(REG_INPUT, c) != 0) return 1; 
    if (util_sys_inb(REG_OUTPUT, out) != 0) return 1;
    return 0;
}

/**
 * @brief Checks if the rtc is updating by getting its current output.
 * @return 0 in case of update.
 * @see rtc_output()
 */
int is_updating() {
    uint8_t result;
    if (rtc_output(REG_UPDATING, &result)) return 1;
	return result & IS_UPDATING;
}

/**
 * @brief Checks if the provided output is in binary.
 * @return 0 in case of truth.
 * @see rtc_output()
 */
int is_binary() {
    uint8_t result;
    if (rtc_output(REG_COUNTING, &result)) return 1;
	return result & IS_BINARY;
}

/**
 * @brief Converts a decimal number to binary.
 * @return Converted number.
 */
int convertTo_binary(uint8_t number) {
    return ((number >> 4) * 10) + (number & 0xF);
}

/**
 * @brief Updates the real_time variable with the current time.
 * @details The created real_time variable has 6 parameters: year, month, day, hour, month and second.
 * @return 0 in case of update.
 * @see is_updating()
 * @see rtc_output()
 * @see is_binary()
 * @see convertTo_binary()
 */
int update_time() {

    if (is_updating() != 0) return 1;

    uint8_t output;

    if (rtc_output(SECOND, &output) != 0) return 1;
    realTime.second = is_binary() ? output : convertTo_binary(output);
    
    if (rtc_output(MINUTE, &output) != 0) return 1;
    realTime.minute = is_binary() ? output : convertTo_binary(output);

    if (rtc_output(HOUR, &output) != 0) return 1;
    realTime.hour = is_binary() ? output : convertTo_binary(output);

    if (rtc_output(DAY, &output) != 0) return 1;
    realTime.day = is_binary() ? output : convertTo_binary(output);

    if (rtc_output(MONTH, &output) != 0) return 1;
    realTime.month = is_binary() ? output : convertTo_binary(output);

    if (rtc_output(YEAR, &output) != 0) return 1;
    realTime.year = is_binary() ? output : convertTo_binary(output);

    return 0;
}

/**@}*/

