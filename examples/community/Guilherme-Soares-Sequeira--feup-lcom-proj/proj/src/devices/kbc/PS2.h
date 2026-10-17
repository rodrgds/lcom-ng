#ifndef PS2_H
#define PS2_H

#include <lcom/lcf.h>

/** @defgroup PS2 PS2
 *  @{
 *
 * Constants used for the programming of the mouse
 */

/* mouse first packet byte structure */

#define MOUSE_LB BIT(0)    /**< @brief if set the left button is pressed */
#define MOUSE_RB BIT(1)    /**< @brief if set the right button is pressed */
#define MOUSE_MB BIT(2)    /**< @brief if set the middle button is pressed */
#define MOUSE_FBYTE BIT(3) /**< @brief check if the received byte is the first packet byte */
#define MOUSE_XSIGN BIT(4) /**< @brief if set delta x is negative */
#define MOUSE_YSIGN BIT(5) /**< @brief if set delta y is negative */
#define MOUSE_XOVFL BIT(6) /**< @brief if set overflow on x */
#define MOUSE_YOVFL BIT(7) /**< @brief if set overflow on y */

/* mouse controller acknowledgment bytes */

#define MOUSE_ACK 0xFA   /**< @brief everything OK */
#define MOUSE_NACK 0xFE  /**< @brief invalid byte, should retry sending the command */
#define MOUSE_ERROR 0xFC /**< @brief second consecutive invalid byte */

/* ps/2 mouse related commands */

#define MOUSE_RESET 0xFF                  /**< @brief resets the mouse */
#define MOUSE_RESEND 0xFE                 /**< @brief used in case of a communication error */
#define MOUSE_SET_DEFAULTS 0xF6           /**< @brief set defaults */
#define MOUSE_DISABLE_DATA_REPORTING 0xF5 /**< @brief disables data reporting */
#define MOUSE_ENABLE_DATA_REPORTING 0xF4  /**< @brief enables data reporting */
#define MOUSE_SET_SAMPLE_RATE 0xF3        /**< @brief sets the sample rate */
#define MOUSE_SET_REMOTE_MODE 0xF0        /**< @brief sets the remote mode */
#define MOUSE_READ_DATA 0xEB              /**< @brief read data */
#define MOUSE_SET_STREAM_MODE 0xEA        /**< @brief set the stream mode */
#define MOUSE_STATUS_REQUEST 0xE8         /**< @brief get mouse configuration */
#define MOUSE_SET_RESOLUTION 0xE8         /**< @brief sets the mouse resolution */
#define MOUSE_SET_SCALING2 0xE7           /**< @brief sets the scaling 2:1 */
#define MOUSE_SET_SCALING1 0xE6           /**< @brief sets the scaling 1:1 */

/* mouse hook id */

#define MOUSE_HOOK_ID 16 /**< @brief mouse hook id used on interrupts */

/* shift amount */

#define MOUSE_XSIGN_SHIFT 4 /**< @brief shift amount */

/* negative number mask */

#define NEG BIT(7) /**<@ if set the number is negative */

/**@}*/

#endif
