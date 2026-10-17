#include "RTC.h"

#define RTC_ADDR_REG 0x70
#define RTC_DATA_REG 0x71
#define BIT_POS(N) ( 1U << (N) )
#define REGISTER_A 10
#define UIP (1<<7)


void wait_valid_rtc()
{
	int enabled;
	unsigned long regA = 0;
	do
	{
		sys_outb(RTC_ADDR_REG, REGISTER_A);
		sys_inb(RTC_DATA_REG, &regA);
	}while( regA & UIP );
}


int checkValue(unsigned long registo, unsigned long bit)
{
	if (registo  & bit) return 1;
	else return 0;

}

/*
int test_conf() {

	unsigned long registers[4];
	load_config(registers);

	printf("Register A: %X \n",registers[0]);
	printf("UIP: %d \n\n", checkValue(registers[0],(1<<7)));

	printf("Register B: %X \n",registers[1]);
	printf("UIE: %d \n\n", checkValue(registers[1],(1<<4)));
	printf("24/12: %d \n\n", checkValue(registers[1],(1<<2)));

	printf("Register C: %X \n",registers[2]);
	printf("Register D: %X \n",registers[3]);
	


	return 0;
}*/
	
void get_data(long int* dia, long int* mes, long int* ano, long int* hora, long int* minuto)
{
	unsigned long dia_temp, mes_temp, ano_temp, hora_temp, minuto_temp;
	endpoint_t ep;
	char name[256];
	int priv_f;
	//printf("ENtrou get data\n");

	sys_whoami(&ep, name, 256, &priv_f);
	sys_enable_iop(ep);

	//printf("Tirou Registos\n");


	wait_valid_rtc();

	sys_outb(RTC_ADDR_REG, 2);
	sys_inb(RTC_DATA_REG, &minuto_temp);

	sys_outb(RTC_ADDR_REG, 4);
	sys_inb(RTC_DATA_REG, &hora_temp);

	sys_outb(RTC_ADDR_REG, 7);
	sys_inb(RTC_DATA_REG, &dia_temp);

	sys_outb(RTC_ADDR_REG, 8);
	sys_inb(RTC_DATA_REG, &mes_temp);

	sys_outb(RTC_ADDR_REG, 9);
	sys_inb(RTC_DATA_REG, &ano_temp);

	printf("hora_temp: %d\n", dia_temp);

	*dia = dia_temp;
	*mes = mes_temp;
	*ano = ano_temp;
	*hora = hora_temp;
	*minuto = minuto_temp;

}

