#include <lcom/lcf.h>

#ifndef PS2_H
#define PS2_H

/** @defgroup PS2 PS2
 * @{
 * Functions to control the mouse input 
 */

/** @brief Extern variable that contains the packet with the mouse data */
extern struct packet pp;
/** @brief Extern variable that contains the bytes read from the mouse */
extern uint8_t mouseBytes[3];

/**
 * @brief Function to subscribe to the mouse interrupts
 * @param bit_no uint8_t* for the variable bit_no to the changed 
 * @return Returns 0 if successfull
 */
int mouse_subscribe_int(uint16_t *bit_no);
/**
 * @brief Function to unsubscribe from the mouse interrupts
 * @return Returns 0 if successfull
 */
int mouse_unsubscribe_int();

/**
 * @brief evaluates the mouse bytes read from the KBC
 * @return Returns 1 if third mouse byte
 */
int mouse_evaluate_bytes();
/**
 * @brief Function to convert a byte to their two's complement
 * @param value uint8_t of the byte value to be converted 
 * @return Returns the two's complement from value
 */
int16_t util_twos_complement(uint8_t value);
/**
 * @brief Function to read a bit value from a byte
 * @param val uint8_t of the byte value 
 * @param bit_no uint8_t bit to be read from the byte
 * @return Returns the bit value
 */
bool util_get_bit(uint8_t val, uint8_t bit_no);
/**
 * @brief writes the mouse default configuration to the KBC
 * @return Returns 0 if successfull
 */
int mouse_write_dflt_kbc_cmd_byte();
/**
 * @brief Enables the report from the mouse data
 * @return Returns 0 if successfull
 */
int mouse_enable_data_report();
/**
 * @brief Function to write a byte to a port in the KBC
 * @param port uint8_t for the address to be written 
 * @param byte uint8_t* for the byte value to be written 
 * @return Returns 0 if successfull
 */
int kbc_write_byte(uint8_t port, uint8_t byte);

/**
 * @brief Function to read a byte from a port in the KBC
 * @param port uint8_t for the address to be read 
 * @param byte uint8_t* for the byte value to be saved 
 * @return Returns 0 if successfull
 */
int kbc_read_byte(uint8_t port, uint8_t *byte);
/**
 * @brief Function to reset the mouse to its original settings
 * @return Returns 0 if successfull
 */
int mouse_reset();
/**
 * @brief Function to initialize the packet pp with the mouse data
 * @param pp packet* for the packet to be altered 
 * @param bytes[] uint8_t for the mouse data 
 */
void initialize_packet(struct packet *pp, uint8_t bytes[]);
/**
 * @}
 */
#endif /*PS2_H*/
