#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

/* The IRQ which the kbc raises for keyboard interrupt. */
#define KBD_IRQ 1 /* Keyboard's IRQ line is 1 */
#define MOUSE_IRQ 12 /* Mouse's IRQ line is 12 */

/* KBC Ports */
#define OUT_BUF_PORT 0x60
#define IN_BUF_PORT 0x60
#define STATUS_REG_PORT 0x64
#define CMDS_PORT 0x64
#define ARGS_PORT 0x60

/* Status bits */
#define PARITY_STATUS_BIT (0x01 << 7)
#define TIMEOUT_STATUS_BIT (0x01 << 6)
#define AUX_STATUS_BIT (0x01 << 5)
#define INH_STATUS_BIT (0x01 << 4)
#define A2_STATUS_BIT (0x01 << 3)
#define SYS_STATUS_BIT (0x01 << 2)
#define IBF_STATUS_BIT (0x01 << 1)
#define OBF_STATUS_BIT (0x01)

/* KBC Commands */
#define READ_COMMAND_BYTE 0x20
#define WRITE_COMMAND_BYTE 0x60

/* KBC time to wait between data */
#define DELAY_US 20000

/* Status Register Bits */
#define PARITY_BIT BIT(7) // Error Checking
#define TIME_OUT_BIT BIT(6) // Error Checking
#define AUX_BIT BIT(5) // Mouse data
#define INH_BIT BIT(4)// Bit is cleared if keyboard is inhibited
#define A2_BIT BIT(3)
#define SYS_BIT BIT(2)
#define IBF_BIT BIT(1) // Bit is set if input buffer full.
#define OBF_BIT 0x01 // Bit is set if output buffer full.

/* KBC "Command Byte" bits */
#define MOUSE_INTF_DISABLED BIT(5) // Bit is set if mouse is disabled.
#define KEYBOARD_INTF_DISABLED BIT(4) // Bit is set if keyboard is disabled.
#define MOUSE_INTS_ENABLED BIT(1) // Bit is set if mouse interrups are enabled.
#define KEYBOARD_INTS_ENABLED 0x01 // Bit is set if keyboard interrupts are enabled.

/* Ports */
#define OUT_BUF 0x60 /* Output buffer port */
#define IN_BUF 0x60 /* Input buffer port */

#define STATUS_REG_PORT 0x64 /* Status register port. */

#define CMDS_REG_PORT 0x64 /* Register to send KBC commands */
#define ARGS_REG_PORT 0x60 /* Register to send KBC command arguments. */

/* Keyboard commands PC-AT | PS2 */
#define KBC_READ_COMMAND 0x20  /* KBC read command */
#define KBC_WRITE_COMMAND 0x60  /* KBC write command */
#define KBC_SELF_TEST_COMMAND 0xAA  /* KBC self test command */
#define KBC_KEYBOARD_INTERFACE_COMMAND 0xAB  /* KBC keyboard interface command */
#define KBC_DISABLE_KBD_COMMAND 0xAD  /* KBC disable keyboard command */
#define KBC_ENABLE_KBD_COMMAND 0xAE  /* KBC enable keyboard command */

/* Mouse commands */
#define MOUSE_WRITE_COMMAND 0xD4 /* Allows the DD to issue commands directly to the mouse. */
#define MOUSE_ENABLE_DATA_REPORTING 0xF4 /* Enable mouse's stream. */
#define MOUSE_DISABLE_DATA_REPORTING 0xF5 /* Disable mouse's data reporting. */

/* Mouse responses */
#define ACK 0xFA /* Everithing went right. */
#define NACK 0xFE /* Invalid byte received must start over. */
#define ERROR 0xFC /* An error occoured. */

#endif /* _LCOM_I8042_H */
