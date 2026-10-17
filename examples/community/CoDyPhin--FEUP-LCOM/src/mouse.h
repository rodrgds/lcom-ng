#include <lcom/lcf.h>
#include <lcom/lab3.h>

#include <stdbool.h>
#include <stdint.h>

/** @defgroup mouse Mouse
 * @{
 * Mouse functions
 */

typedef enum {INIT, FIRSTL, VERTEX, SECONDL, FINAL} state_t;
typedef enum {LPRESSED, LRELEASE, RPRESSED, RRELEASE, ERRO} ev_type_t;

/**
 * @brief Subscribes the mouse interrupts
 * @param bit_no Bit number where the IRQ is initialized
 * @return 1 if error, 0 if OK
 */
int (mouse_subscribe_int)(uint8_t *bit_no);

/**
* @brief Unsubscribes the keyboard interrupts
* @return 1 if error, 0 if OK
*/
int (mouse_unsubscribe_int)();

/**
 * @brief Mouse Interrupt Handler
 */
void (mouse_ih)();

/**
 * @brief Parses all the mouse packets recieved
 * @param pp Pointer to all the parsed packets
 * @param packetBytes Array of bytes given by a mouse interrupt
 */
void (packet_parser)(struct packet *pp, uint8_t *packetBytes);

/**
 * @brief Writes a command to the mouse control register
 * @param cmd Specified command
 * @return 1 if error, 0 if OK
*/
int write_mouse_cmd(uint32_t cmd);

/** @} end of Mouse */
