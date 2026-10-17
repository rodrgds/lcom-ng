#ifndef TIMER_H_
#define TIMER_H_


#define TIMER_FREQ  1193182


#define TIMER0_IRQ	        0

/* I/O port addresses */

#define TIMER_0			0x40
#define TIMER_1			0x41
#define TIMER_2			0x42
#define TIMER_CTRL		0x43
#define SPEAKER_CTRL		0x61
/* Timer control */
#define SEL_COUNTER1  6
#define SEL_COUNTER2  7
#define ACESS1 4
#define ACESS2 5
#define OPMODE1 1
#define OPMODE2 2
#define OPMODE3 3
#define CNTMODE 0
//when read-back
#define READVL 5
#define STATUS 4
#define Counter2 3
#define Counter1 2
#define Counter0 1


//acess, 0 = LSB, 1= MSB 2* LSB->MSB
int configure_timer(unsigned timer, unsigned acess,unsigned mode,unsigned bcd);
int unsubscribe_timer();
int subscribe_timer();
void set_frq();
void IncSeg();
int GetSeg();


#endif /* _LCOM_I8254_H */
