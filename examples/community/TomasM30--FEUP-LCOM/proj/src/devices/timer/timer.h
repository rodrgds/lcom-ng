#ifndef __TIMER_H
#define __TIMER_H

#include <stdbool.h>
#include <stdint.h>

/** @defgroup timer timer
 *  @brief Functions to work with the i8254 timer
*/


/**
 * @brief Enumerated type for specifying the timer value initialization
 */
enum timer_init {
	INVAL_val,    /*!< Invalid initialization mode */
	LSB_only,     /*!< Initialization only of the LSB */
	MSB_only,     /*!< Initialization only of the MSB */
	MSB_after_LSB /*!< Initialization of LSB and MSB, in this order */
};

/**
 * @brief Enumerated type for identifying the timer status fields
 */
enum timer_status_field {
	tsf_all,     /*!< configuration/status */
	tsf_initial, /*!< timer initialization mode */
	tsf_mode,    /*!< timer counting mode */
	tsf_base     /*!< timer counting base */
};


union timer_status_field_val {
	uint8_t byte;            /*!< status */
	enum timer_init in_mode; /*!< initialization mode */
	uint8_t count_mode;      /*!< counting mode: 0, 1,.., 5 */
	bool bcd;                /*!< counting base, true if BCD */
};

/**
 * @brief Sets the frequency of a timer
 * @param timer Timer to configure
 * @param freq Frequency to set
 * @return Return 0 upon success and 1 otherwise
*/
int(timer_set_frequency)(uint8_t timer, uint32_t freq);

/**
 * @brief Subscribes and enables Timer 0 interrupts
 * @param bit_no Pointer to the variable where the bit mask will be stored
 * @return Return 0 upon success and 1 otherwise
 */
int(timer_subscribe_int)(uint8_t *bit_no);

/**
 * @brief Unsubscribes Timer 0 interrupts
 * @return Return 0 upon success and 1 otherwise
 */
int(timer_unsubscribe_int)();

/**
 * @brief Timer 0 interrupt handler
 */
void(timer_int_handler)();

/**
 * @brief Reads the input timer configuration via read-back command
 * @param timer Timer whose configuration to read
 * @param st Address of memory position to be filled with the timer config
 * @return Return 0 upon success and 1 otherwise
 */
int(timer_get_conf)(uint8_t timer, uint8_t *st);

/**
 * @brief Displays the specified field of a timer configuration
 * @param timer Timer whose configuration should be displayed
 * @param st Timer configuration
 * @param field Timer status field to display: tsf_all, tsf_initial, tsf_mode, tsf_base
 * @return Return 0 upon success and 1 otherwise
 */
int(timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field);


#endif
