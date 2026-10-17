#ifndef _INTERUPTS_H_
#define _INTERUPTS_H_

///IRQ lines used to susbscribe all possivel events
#define IRQ_TIMER0 0 //Timer0 to control game_time (cronometro)
#define IRQ_KBD 1 //keyboard
#define IRQ_COM2 3 //serial port line 2
#define IRQ_COM4 4 //serial port line 1
#define IRQ_RTC 8 // RTC (to know current date and time)
#define IRQ_MOUSE 12 // mouse

///keyboard-related variables
///int TWO_BYTE_KEY = 0;
///unsigned char scancode;

//mouse-related variables
///unsigned char packet[3]={0,0,0}; //To read the packets from the PS/2 mouse
///int count=0; //For the while cycle in test_packet

///interupts.c functions

///susbscribes the devices used in game
void subscribe_interrupts();

///unsusbscribes the devices used in game
void unsubscribe_interrupts();

///Main interuption cycle to limit player turn either by time or keyboard event
void interupt_handler(int time);

#endif //_INTERUPTS_H_
