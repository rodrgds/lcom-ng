#ifndef SER_H
#define SER_H

/**
 * @brief Initializes the send and receive queues.
 */
void init_queues();

/**
 * @brief Clears the send and receive queues.
 */
void clear_queues();

/**
 * @brief Destroys the send and receive queues.
 */
void destroy_queues();

/**
 * @brief Removes and returns the front element from the receive queue.
 *
 * @return The value of the front element in the receive queue.
 */
uint8_t receive_queue_pop();

/**
 * @brief Adds a byte to the send queue.
 *
 * @param byte The byte to be added to the send queue.
 */
void send_queue_push(uint8_t byte);

/**
 * @brief Checks if the receive queue is empty.
 *
 * @return True if the receive queue is empty, false otherwise.
 */
bool receive_queue_is_empty();

/**
 * @brief Checks if the send queue is empty.
 *
 * @return True if the send queue is empty, false otherwise.
 */
bool send_queue_is_empty();

/**
 * @brief Retrieves the value of the front element in the receive queue without removing it.
 *
 * @return The value of the front element in the receive queue.
 */
uint8_t receive_queue_front();

/**
 * @brief Retrieves the number of elements in the receive queue.
 *
 * @return The number of elements in the receive queue.
 */
uint8_t receive_queue_size();

/**
 * @brief Adds a byte to the receive queue.
 *
 * @param data The byte to be added to the receive queue.
 */
void receive_queue_push(uint8_t data);

/**
 * @brief Reads a byte from the UART receive buffer.
 *
 * @param data Pointer to store the read byte.
 * @return 0 upon success, 1 otherwise.
 */
int read_byte(uint8_t *data);

/**
 * @brief Sends a byte through the UART transmit buffer.
 *
 * @param data The byte to be sent.
 * @return 0 upon success, 1 otherwise.
 */
int send_byte(uint8_t data);

/**
 * @brief Sends bytes from the send queue to the UART transmit buffer.
 *
 * @return 0 upon success, 1 otherwise.
 */
int send_bytes();

/**
 * @brief Reads the Line Status Register (LSR) value.
 *
 * @param status Pointer to store the read LSR value.
 * @return 0 upon success, 1 otherwise.
 */
int read_lsr(uint8_t *status);

/**
 * @brief Writes a value to the Line Control Register (LCR).
 *
 * @param conf The value to write to the LCR.
 * @return 0 upon success, 1 otherwise.
 */
int write_lcr(uint8_t conf);

/**
 * @brief Reads the Line Control Register (LCR) value.
 *
 * @param conf Pointer to store the read LCR value.
 * @return 0 upon success, 1 otherwise.
 */
int read_lcr(uint8_t *conf);

/**
 * @brief Configures the serial port parameters.
 *
 * @param bits Number of bits per character (5-8).
 * @param stop Number of stop bits (1-2).
 * @param par_mode Parity mode (1-4).
 * @return 0 upon success, 1 otherwise.
 */
int ser_conf(uint8_t bits, uint8_t stop, uint8_t par_mode);

/**
 * @brief Configures the bit rate for the serial port.
 *
 * @param bit_rate The desired bit rate.
 * @return 0 upon success, 1 otherwise.
 */
int bit_rate_conf(uint32_t bit_rate);

/**
 * @brief Initializes the serial port by setting up its parameters, bit rate, interrupts, and FIFOs.
 *
 * @return 0 upon success, 1 otherwise.
 */
int ser_init();

/**
 * @brief Subscribes to interrupts from the serial port.
 *
 * @param bit_no Pointer to the bit mask of the subscribed interrupt.
 * @return 0 upon success, 1 otherwise.
 */
int ser_subscribe_int(uint8_t* bit_no);

/**
 * @brief Unsubscribes from interrupts from the serial port.
 *
 * @return 0 upon success, 1 otherwise.
 */
int ser_unsubscribe_int();

/**
 * @brief Enables interrupts for the serial port.
 *
 * @return 0 upon success, 1 otherwise.
 */
int ser_enable_interrupts();

/**
 * @brief Reads the interrupt identification register (IIR) status.
 *
 * @param irr_status Pointer to store the read IIR status.
 * @return 0 upon success, 1 otherwise.
 */
int iir_status_read(uint8_t *irr_status);

/**
 * @brief Retrieves the data errors from the Line Status Register (LSR).
 *
 * @param errors Array to store the data errors.
 * @return 0 upon success, 1 otherwise.
 */
int get_data_errors(int errors[]);

/**
 * @brief Handles errors encountered during communication.
 *
 * @param errors Array of data errors.
 * @return 0 upon success, 1 otherwise.
 */
int handle_errors(int errors[]);

/**
 * @brief Interrupt handler for the serial port.
 */
void ser_ih();

/**
 * @brief Checks if the transmitter is ready to send data.
 *
 * @return True if the transmitter is ready, false otherwise.
 */
bool transmitter_ready();

/**
 * @brief Initializes the FIFOs of the serial port.
 *
 * @return 0 upon success, 1 otherwise.
 */
int init_fifos();

/**
 * @brief Clears the FIFOs of the serial port.
 *
 * @return 0 upon success, 1 otherwise.
 */
int clear_fifos();

int check_ih_err();

int check_transmitter();

#endif // SER_H
