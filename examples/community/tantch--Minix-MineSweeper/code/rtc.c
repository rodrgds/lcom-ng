unsigned long regSec;
unsigned long regMin;
unsigned long regHour;
unsigned long regSeca;
unsigned long regMina;
unsigned long regHoura;
unsigned long regWeek;
unsigned long regDay;
unsigned long regMonth;
unsigned long regYear;
unsigned long regA;
unsigned long regB;
unsigned long regC;
unsigned long regD;

#include "rtc.h"
int upd = 0;
int hook_id_rtc = 8;

int subscribe_rtc() {
	unsigned long bitmask = BIT(hook_id_rtc);
	if (sys_irqsetpolicy(IRQFRTC, IRQ_REENABLE, &hook_id_rtc) != OK) {
		printf("IRQ_SETPOLICY falhou /n");
		return -1;
	}
	if (sys_irqenable(&hook_id_rtc) != OK) {
		printf("IRQENABLE falhou /n");
		return -1;
	}
	return bitmask;

}
int unsubscribe_rtc() {
	if (sys_irqdisable(&hook_id_rtc) != OK) {
		printf("IRQDISABLE falhou /n");
		return -1;
	}
	if (sys_irqrmpolicy(&hook_id_rtc) != OK) {
		printf("IRQRMPOLICY falhou /n");
		return -1;
	}
	return 0;
}
void set_rtc() {
	//enable periodic-interrupt
	sys_outb(ADDR_REGR, REG_B);
	sys_inb(DATA_REGR, &regB);

	regB = regB | 0x40;

	sys_outb(ADDR_REGR, REG_B);
	sys_outb(DATA_REGR, regB);

	//set interrupt rate to 500ms

	sys_outb(ADDR_REGR, REG_A);
	sys_inb(DATA_REGR, &regA);

	regA = regA | 0x0F;

	sys_outb(ADDR_REGR, REG_A);
	sys_outb(DATA_REGR, regA);

}
int read_state() {
	regSec = 0;
	regMin = 0;
	regHour = 0;
	regWeek = 0;
	regDay = 0;
	regMonth = 0;
	regYear = 0;

	sys_outb(ADDR_REGR, SEC);
	sys_inb(DATA_REGR, &regSec);

	sys_outb(ADDR_REGR, MINU);
	sys_inb(DATA_REGR, &regMin);

	sys_outb(ADDR_REGR, HOUR);
	sys_inb(DATA_REGR, &regHour);

	sys_outb(ADDR_REGR, WEEK_DAY);
	sys_inb(DATA_REGR, &regWeek);

	sys_outb(ADDR_REGR, DAY);
	sys_inb(DATA_REGR, &regDay);

	sys_outb(ADDR_REGR, MONTH);
	sys_inb(DATA_REGR, &regMonth);

	sys_outb(ADDR_REGR, YEAR);
	sys_inb(DATA_REGR, &regYear);

	return 0;
}
void print_date() { //we decided to print the numbers in hexadecimal even though the numbers
	//are in BCD because the range of digits is 0-9 there is no need for conversions
	printf("drawing week: %d \n", regWeek);

	switch (regWeek) {

	case 1:
		draw_string("thu", 700, 100, 35);
		printf(" thu \n");
		break;
	case 2:
		draw_string("fri", 700, 100, 35);
		printf(" fri \n");
		break;
	case 3:
		draw_string("sat", 700, 100, 35);
		printf(" sat \n");
		break;
	case 4:
		draw_string("sun", 700, 100, 35);
		printf(" sun \n");
		break;
	case 5:
		draw_string("mon", 700, 100, 35);
		printf(" mon \n");
		break;
	case 6:
		draw_string("tue", 700, 100, 35);
		printf(" tue \n");
		break;
	case 7:
		draw_string("wed", 700, 100, 35);
		printf(" wed \n");
		break;
	default:
		break;
	}

	switch (regMonth) {

	case 1:
		draw_string("jan", 700, 150, 35);
		printf(" jan \n");
		break;
	case 2:
		draw_string("feb", 700, 150, 35);
		printf(" feb \n");
		break;
	case 3:
		draw_string("mar", 700, 150, 35);
		break;
	case 4:
		draw_string("apr", 700, 150, 35);
		break;
	case 5:
		draw_string("may", 700, 150, 35);
		break;
	case 6:
		draw_string("jun", 700, 150, 35);
		break;
	case 7:
		draw_string("fel", 700, 150, 35);
		break;
	case 8:
		draw_string("aug", 700, 150, 35);
		break;
	case 9:
		draw_string("sep", 700, 150, 35);
		break;
	case 16: //since in bcd when he rights then it is the eqeuivelent of 16
		draw_string("oct", 700, 150, 35);

		break;
	case 17:
		draw_string("nov", 700, 150, 35);

		break;
	case 18:
		draw_string("dec", 700, 150, 35);

		break;
	default:
		break;
	}

	int tp1 = regDay >> 4;
	printf("%d \n", tp1);
	int tp2 = regDay & 0x0F;
	printf("%d \n", tp2);
	int tp3 = tp1 * 10 + tp2;

	draw_cp(tp3,750,150,35);


}

void rtc_handle() {
	sys_outb(ADDR_REGR, REG_A);
	sys_inb(DATA_REGR, &regA);
	if (regA & BIT(7)) {
		read_state();
		upd = 1;
	}
	if (upd == 1)
		print_date();

}

