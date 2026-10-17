#ifndef KBC_H_
#define KBC_H_

#define DELAY_US        20000

#define KBC_MAX_TRIES   20

#define KBC_ERROR_RET   0xD06EF00D

/*
 *  Some of the constants in here were taken from the following header:
 *  http://web.fe.up.pt/~pfs/aulas/lcom2010/labs/lab5/kbc.h
 */

#define KBC_REG_STATUS  0x64
#define KBC_REG_DATA    0x60

#define KBC_ERR_ERROR   0xFE
#define KBC_ERR_REPEAT  0xFC

#define KBC_BIT_OBF     (1 << 0)
#define KBC_BIT_IBF     (1 << 1)
#define KBC_BIT_AUX     (1 << 5)
#define KBC_BIT_PAR_ERR (1 << 7)
#define KBC_BIT_TO_ERR  (1 << 6)

#define KB_IRQ          0x01
#define MOUSE_IRQ       0x0C

/** @defgroup kbc kbc
 * @{
 * Functions for controlling the KBC
 */

/**
 * @brief Reads from the KBC.
 *
 * @return The read result.
 */

int keyboard_read(void);

/**
 * @brief Writes to the KBC.
 *
 * @param port port to write to (normally, mouse or keyboard)
 * @param input bytes to write
 * @return 0 on success, undefined on failure.
 */

int keyboard_write(unsigned long port, unsigned char input);

/**@}*/

#endif /* KBC_H_ */
