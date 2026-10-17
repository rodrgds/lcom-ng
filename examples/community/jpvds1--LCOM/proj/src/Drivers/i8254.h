/**
@file i8254.h
@brief Constants and macros for programming the i8254 Timer. */

#ifndef LCOM_I8254_H
#define LCOM_I8254_H

/**
@defgroup i8254 i8254

@{
Constants for programming the i8254 Timer. */

/** 
@brief Clock frequency for timer in PC and AT. */ 
#define TIMER_FREQ 1193182

/**
@brief Timer 0 IRQ line. */
#define TIMER0_IRQ 0

/**
@brief Timer 0 count register. */
#define TIMER_0 0x40

/**
@brief Timer 1 count register. */
#define TIMER_1 0x41

/**
@brief Timer 2 count register. */
#define TIMER_2 0x42

/** 
@brief Control register. */
#define TIMER_CTRL 0x43

/** 
@brief Register for speaker control. */
#define SPEAKER_CTRL 0x61

/**
@brief Control Word for Timer 0.
@details Timer selection: bits 7 and 6. */

#define TIMER_SEL0 0x00

/**
@brief Control Word for Timer 1.
@details Timer selection: bits 7 and 6. */

#define TIMER_SEL1 BIT(6)

/**
@brief Control Word for Timer 2.
@details Timer selection: bits 7 and 6. */

#define TIMER_SEL2 BIT(7)

/**
@brief Read Back Command.
@details Used in conjunction with TIMER_SEL0, TIMER_SEL1, or TIMER_SEL2.
Register selection: bits 5 and 4. */

#define TIMER_RB_CMD (BIT(7) | BIT(6))

/** 
@brief Initialize Counter LSB only.

@details Register selection: bits 5 and 4.
Operating mode: bits 3, 2 and 1.
Counting mode: bit 0. */

#define TIMER_LSB BIT(4)

/** 
@brief Initialize Counter MSB only.

@details Register selection: bits 5 and 4.
Operating mode: bits 3, 2 and 1.
Counting mode: bit 0. */

#define TIMER_MSB BIT(5)

/**
@brief Initialize LSB first and MSB afterwards.
@details Register selection: bits 5 and 4.
Operating mode: bits 3, 2 and 1.
Counting mode: bit 0. */

#define TIMER_LSB_MSB (TIMER_LSB | TIMER_MSB)

/** 
@brief Mode 3: square wave generator.
@details Operating mode: bits 3, 2 and 1. */

#define TIMER_SQR_WAVE (BIT(2) | BIT(1))

/** 
@brief Mode 2: rate generator.
@details Operating mode: bits 3, 2 and 1. */

#define TIMER_RATE_GEN BIT(2)

/**
@brief Count in BCD.
@details Counting mode: bit 0. */

#define TIMER_BCD 0x01

/** 
@brief Count in binary.
@details Counting mode: bit 0. */

#define TIMER_BIN 0x00

/**

@brief Read Back Command for count.
@details Used in conjunction with TIMER_SELx.
Register selection: bits 5 and 4. */

#define TIMER_RB_COUNT_ BIT(5)

/**
@brief Read Back Command for status.
@details Used in conjunction with TIMER_SELx.
Register selection: bits 5 and 4. */

#define TIMER_RB_STATUS_ BIT(4)

/**
@brief Read Back Command with select.
@details Used in conjunction with TIMER_RB_COUNT or TIMER_RB_STATUS.
Parameter n: 0-2. Select timer to read. */

#define TIMER_RB_SEL(n) BIT((n) + 1)

/**

@} */
#endif /* _LCOM_I8254_H */
