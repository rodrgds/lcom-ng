#ifndef _LCOM_I8402_H_
#define _LCOM_I8402_H_



#include <lcom/lcf.h>

/** @defgroup i8402 i8402
 * @{
 *
 * Constants for programming the i8402 Keyboard. Needs to be completed.
 *
*/

#define DELAY_US 20000 //wait 20 ms


/*i8042 ports*/

#define STATUS_REGISTER 0x64 //read the KBC state

#define INPUT_BUFFER_COMMANDS 0x64 //write commands to KBC access

#define INPUT_BUFFER_ARGUMENTS 0x60 //write arguments to KBC access

#define OUTPUT_BUFFER 0x60 //read sandcodes both make and break and return values from KBC commands

/* STATUS REGISTER */

#define OBF   BIT(0)              /**< @brief Output buffer full - data available for reading */

#define IBF BIT(1)         /**< @brief Input buffer full don’t write commands or arguments */

#define INH BIT(4)         /**< @brief Inhibit flag: 0 if keyboard is inhibited */

#define AUX BIT(5)         /**< @brief Mouse data */

#define TIMEOUT   BIT(6)            /**< @brief Timeout error - invalid data */

#define PARITY   BIT(7)            /**< @brief Parity error - invalid data */

/** COMMANDS */

#define READ_COM_BYTE    0x20              /**< @brief Returns Command Byte */

#define WRITE_COM_BYTE    0x60              /**< @brief Takes Arguments: Command Byte */

#define COMMAND_MOUSE 0xD4

#define STREAM_MODE 0xEA

#define ENABLE_MOUSE 0xF4

#define DISABLE_MOUSE 0xF5

#define IRQ2 2
#define IRQ1 1
#define IRQ0 0
#define IRQ12 12

#define ENTER 0x1C
#define ESC 0x81
#define WK 0x11
#define AK 0x1e
#define DK 0x20
#define SK 0x1f

#endif /* _LCOM_I8042_H */
