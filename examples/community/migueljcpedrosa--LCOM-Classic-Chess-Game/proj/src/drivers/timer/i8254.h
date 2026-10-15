/**
 * @file i8254
 * @brief This file has all defines related to the timer.
 */

#ifndef _LCOM_I8254_H_
#define _LCOM_I8254_H_

#include <lcom/lcf.h>

/** @brief Frequency of the timer */
#define TIMER_FREQ 1193182 
/** @brief IRQ line for timer 0 */
#define TIMER0_IRQ 0 

/** @brief I/O port for timer 0 */
#define TIMER_0    0x40 
/** @brief I/O port for timer 1 */
#define TIMER_1    0x41 
/** @brief I/O port for timer 2 */
#define TIMER_2    0x42 
/** @brief I/O port for the timer control */
#define TIMER_CTRL 0x43 

/** @brief I/O port for the speaker control */
#define SPEAKER_CTRL 0x61 

/** @brief Select timer 0 */
#define TIMER_SEL0   0x00         
/** @brief Select timer 1 */
#define TIMER_SEL1   BIT(6)            
/** @brief Select timer 2 */
#define TIMER_SEL2   BIT(7)          
/** @brief Timer read-back command */
#define TIMER_RB_CMD (BIT(7) | BIT(6)) 

/** @brief Timer LSB */
#define TIMER_LSB     BIT(4)              
/** @brief Timer MSB */
#define TIMER_MSB     BIT(5)            
/** @brief Timer LSB followed by MSB */
#define TIMER_LSB_MSB (TIMER_LSB | TIMER_MSB) 

/** @brief Timer square wave mode */
#define TIMER_SQR_WAVE (BIT(2) | BIT(1)) 
/** @brief Timer rate generator */
#define TIMER_RATE_GEN BIT(2)          

/** @brief Timer BCD mode */
#define TIMER_BCD 0x01 
/** @brief Timer binary mode */
#define TIMER_BIN 0x00 

/** @brief Timer read-back count */
#define TIMER_RB_COUNT_  BIT(5)
/** @brief Timer read-back status */
#define TIMER_RB_STATUS_ BIT(4)
/** @brief Timer read-back select */
#define TIMER_RB_SEL(n)  BIT((n) + 1)


#endif 
