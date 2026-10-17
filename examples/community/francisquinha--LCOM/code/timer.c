/** @file timer.c
 * @brief file containing the function definitions to handle the timer
 */
#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/com.h>

#include "timer.h"

#define DEBUG_MALLOC 0

Timer *create_timer() {
	//allocate space for the "object"
	Timer *timer = (Timer *)malloc(sizeof(Timer));
	if (DEBUG_MALLOC) printf("Allocated %lu bytes for timer\n", sizeof(Timer));
	if (timer == NULL)
		return NULL;
	reset_timer(timer);
	return timer;
}

void timer_int_handler(Timer *timer) {
	timer->counter++;
	if (timer->counter == 60) {
		timer->seconds++;
		timer->counter = 0;
	}
	if (timer->seconds == 60) {
		timer->minutes++;
		timer->seconds = 0;
	}
	if (timer->minutes == 60) {
		timer->hours++;
		timer->minutes = 0;
	}
}

void reset_timer(Timer *timer) {
	timer->hours = 0;
	timer->minutes = 0;
	timer->seconds = 0;
	timer->counter = 0;
}

void penalize(Timer *timer, unsigned short minutes) {
	timer->minutes += minutes;
	if (timer->minutes >= 60) {
		timer->hours++;
		timer->minutes -= 60;
	}
}
