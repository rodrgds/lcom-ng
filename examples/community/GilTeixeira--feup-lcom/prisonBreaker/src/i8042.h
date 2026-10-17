#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

/* Useful KBD Codes */
#define ESC 			0x81
#define FIRST_BYTE		0xE0

/* KBD Ports */
#define OUT_BUF 0x60
#define IN_BUF 0x60
#define STAT_REG 0x64
#define KBC_CMD_REG 0x64

/* Status Register Bits Meaning */
#define PAR_ERR BIT(7)
#define TO_ERR BIT(6)
#define AUX BIT(5)
#define OBF BIT(0)
#define IBF BIT(1)

/* KBC Bits Meaning */
#define ENABLE_KBD_INT BIT(0)
#define DISABLE_MOUSE_INT BIT(5)
#define ENABLE_MOUSE_INT BIT(1)

/* KBC Commands */
#define READ_COMM_BYTE 0x20
#define WRITE_COMM_BYTE 0x60

/* Mouse Responses */
#define ACK				0xFA
#define NACK			0xFE
#define ERROR 			0xFC

/* Mouse Commands */
#define SET_STREAM_MODE 0XEA
#define ENABLE_DR 0XF4
#define DISABLE_DR 0XF5
#define SET_REMOTE_MODE 0XF0
#define READ_DATA 0XEB

/* Mouse Related Commands */
#define WRITE_BYTE_MOUSE 0XD4

/* IRQ line */
#define KBD_IRQ	        1    /**< @brief KBD IRQ line */
#define MOUSE_IRQ	        12    /**< @brief KBD IRQ line */

/* Mouse packets Bits Meaning */
#define MOUSE_LB BIT(0)
#define MOUSE_RB BIT(1)
#define MOUSE_MB BIT(2)
#define MOUSE_XSIGN BIT(4)
#define MOUSE_YSIGN BIT(5)
#define MOUSE_XOVFL BIT(6)
#define MOUSE_YOVFL BIT(7)
#define MOUSE_BIT3 BIT(3)

/* Time To Delay */
#define DELAY_US    20000

#endif
