#ifndef _LCOM_MACROS_H_
#define _LCOM_MACROS_H_

#define BIT(n) (0x01<<(n))

#define TIMER0_IRQ	        	0    /**< @brief Timer 0 IRQ line */
#define TIMER0_FREQ           60

#define VBE_CALL_CMD 0x4F00
#define VBE_SET_GRAPHIC_MODE 0x0002

#define GRAPHIC_MODE 0x105
#define LINEAR_FRAMEBUFFER BIT(14)

#define VBE_INTERFACE 0x10

#define VBE_SET_TEXT_MODE 0x00
#define TEXT_MODE 0x03

#define	KEY_ESC 0x81
#define KEY_SPACE_DOWN 0x39
#define KEY_SPACE_UP 0xB9

#define GET_VBE_CONTROLLER_INFO 0x00

#define ALIGN_LEFT 0
#define ALIGN_RIGHT 1
#define ALIGN_CENTER 2

#define MAGENTA 0xf81f

//GAME
#define Z_INDEX_MAX 5
#define N_STATES 5

#endif
