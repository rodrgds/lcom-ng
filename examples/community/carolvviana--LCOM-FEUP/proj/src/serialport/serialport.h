#include <lcom/lcf.h>
#include <stdbool.h>
#include <stdint.h>

#define SERIALP_ADDRESS 0x3F8 /*!< @brief COM1 Address */

#define SERIALP_RECEIVER BIT(0) /*!< @brief Enable the Received Data Available Interrupt */

#define SERIALP_STARTGAME 0x11 /*!< @brief Message to connect */
#define SERIALP_PLAY 0x12 /*!< @brief Message to start game */

#define SERIALP_RECEIVING_DATA  BIT(0) /*!< @brief Set in the status when there is data for receiving */

#define SERIALP_IER_OFFSET 1 /*!< @brief Interrupt Enable Register Offset regarding the COM1 address */
#define SERIALP_LSR_OFFSET  5  /*!< @brief Line Status Register Offset regarding the COM1 address */

#define SERIALP_OVERRUN_ERR (1<<1)  /*!< @brief Overrun Error */
#define SERIALP_PARITY_ERR (1<<2) /*!< @brief Parity Error */
#define SERIALP_FRAME_ERR (1<<3) /*!< @brief Frame Error */

#define SERIALP_TRANSMITTERS BIT(5) | BIT(6) /*!< @brief THR and TSR are both empty */

#define SERIALP_IRQLINE 4 /*!< @brief IRQLine of SerialPort */


/**
 * @brief Initializes the serial port: Enables serialport interrupts and sets the serialport to the correct configuration (interrupt when there is data avilable). Reads all data that might be unread and signals it is ready to connect
 * 
 * @return int 0 if success, 1 otherwise
*/
int (init_serialp)();

/**
 * @brief SerialPort interrupt handler
 * 
*/
void(serialp_ih)();

/**
 * @brief Reads data from the serial port: Checks status, sees if there is any data received, reads it and stores it in the variable data
 * 
 * @return int 0 if success, 1 otherwise
*/
int (read_data_serialp)();

/**
 * @brief Reads the status of the serial port from Line Status Register and checks if there are no errors
 * 
 * @param status pointer to the variable where the status will be stored
 * @return 0 if success, 1 if error
*/
int (read_status_serialp)(uint8_t* status);

/**
 * @brief Sends data to the serial port: First checks status and sees if THR and TSR are empty, then sends the data
 * 
 * @param data to be sent
 * @return 0 if success, 1 if error
*/
int (send_data_serialp)(uint8_t data);

/**
 * @brief Estabilishes a first connection (and connects) between the two serial ports. Determines which player is which and sends the ready signal
 * 
 * @param pointer "ready" to send ready-signal between serialports
 * @return 0 if success, 1 if error
*/
int (serialp_connection)(int *ready);

/**
 * @brief Subscribes the serial port interrupts
 * 
 * @param bit_no pointer to the hook_id_serialp
 * @return 0 if success, 1 if error
*/
int (serialp_subscribe_int)(uint8_t *bit_no);

/**
 * @brief Unsubscribes the serial port interrupts
 * 
 * @return 0 if success, 1 if error
 */
int (serialp_unsubscribe_int)();

/**
 * @brief Reads the status of the serial port from the LSR
 * 
 * @param status pointer to the variable where the status will be stored
 * @return 0 if success, 1 if error
*/
int (serialp_get_status)(uint8_t* status);

/**
 * @brief Checks if the parameter status does not have Parity, overrun or framing errors
 * 
 * @param status pointer to the variable where the status will be stored
 * @return 0 if success, 1 if error
*/
int (serialp_check_error)(uint8_t *status);

/**
 * @brief Writes the parameter data to a certain address (address of COM1 + offset)
 * 
 * @param offset of the address relative to the COM1 address
 * @param data to be written
 * @return 0 if success, 1 if error
*/
int (write_address_with_offset)(int offset, uint8_t data);
