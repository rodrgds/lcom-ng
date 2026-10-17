#ifndef G6_I8254_TIMER_H
#define G6_I8254_TIMER_H

#include <lcom/lcf.h>

// Base Timer Frequency
#define TIMER_FREQ 60

// IRQ Macros
#define TIMER0_IRQ 0
#define TIMER_BIT_NO 0

// Control Word Macros
#define CW_TIMER0 0
#define CW_TIMER1 BIT(6)
#define CW_TIMER2 BIT(7)

#define CW_LSB BIT(4)
#define CW_MSB BIT(5)
#define CW_LSB_MSB BIT(4) | BIT(5)

#define CW_MODE_3 BIT(1) | BIT(2) | BIT(3)

#define CW_BINARY 0
#define CW_BCD BIT(0)

// Read-Back Macros
#define READ_BACK BIT(7) | BIT(6)
#define RB_NO_COUNT BIT(5)
#define RB_NO_STATUS BIT(4)
#define RB_TIMER(n) BIT(n + 1)

// Address Macros
#define TIMER_CTRL 0x43
#define TIMER_0 0x40

// Game Loop Macros
#define FRAME_RATE 30

#endif //G6_I8254_TIMER_H
