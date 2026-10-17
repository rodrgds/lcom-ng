/**
 * @file macros.h
 * @author Bruno Pinto and Vitor Magalhaes
 * @brief Contains devices macros
 */

#ifndef _MACROS_H_
#define _MACROS_H_

/** @defgroup macros macros
 * @{
 *
 * Devices macros
 */

/* General */

#define BIT(n)		(0x01<<(n))	/**< @brief Returns an hexadecimal number with the nth bit set */

#define DELAY_US	20000		/**< @brief Delay time */

/* Timer */

#define TIMER0_IRQ	0			/**< @brief Timer IRQ */

/* Mouse and Keyboard */

#define OUT_BUF		0x64		/**< @brief Output port address */

#define IN_BUF		0x60		/**< @brief Input port address */

/* Keyboard */

#define ESC_COD		0x81		/**< @brief Escape released */

#define SPC_COD		0xB9		/**< @brief Space released */

#define ENTER_COD	0x1C		/**< @brief Enter pressed */

#define W_PRESS		0x11		/**< @brief W pressed */

#define A_PRESS     0x1E		/**< @brief A pressed */

#define S_PRESS		0x1F		/**< @brief S pressed */

#define D_PRESS     0x20		/**< @brief D pressed */

#define RESET_KEY	0x00		/**< @brief Key reset */

#define SPECIAL		0xE0		/**< @brief Special keys most significant byte */

/* Mouse */

#define MOUSE_IRQ	12			/**< @brief Mouse IRQ */

#define DR_DIS		0xF5		/**< @brief Data Report Disable */

#define SET_STREAM	0xEA		/**< @brief Set stream mode */

#define DR_ENA		0xF4		/**< @brief Data Report Enable */

#define GET_STATUS	0xE9		/**< @brief Get status */

#define CTRL_REG	0x64		/**< @brief Control register */

#define ACK			0xFA		/**< @brief Acknowledgement response */

/* RTC */

#define RTC_ADDR_REG   0x70		/**< @brief Address port */

#define RTC_DATA_REG   0x71		/**< @brief Data port */

#define IRQ_RTC		   8		/**< @brief RTC IRQ */

#define PC_SEC		   0x00		/**< @brief Seconds time address */

#define PC_MIN         0x02		/**< @brief Minutes time address */

#define PC_HOUR        0x04		/**< @brief Hours time address */

#define PC_DAY		   0x07		/**< @brief Days date address */

#define PC_MONTH       0x08		/**< @brief Months date address */

#define PC_YEAR        0x09		/**< @brief Months date address */

/** @} end of macros */

#endif
