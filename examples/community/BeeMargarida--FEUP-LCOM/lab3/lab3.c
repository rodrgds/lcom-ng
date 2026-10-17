#include "i8042.h"
#include "test3.h"
#include <limits.h>
#include <string.h>
#include <errno.h>
#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/sysutil.h>
#include <minix/com.h>


static int proc_args(int argc, char **argv);
static unsigned long parse_ulong(char *str, int base);
static void print_usage(char **argv);

int main(int argc, char **argv){

	sef_startup();

	sys_enable_iop(SELF);

	if (argc == 1) {					/* Prints usage of the program if no arguments are passed */
		print_usage(argv);
		return 0;
	}
	else return proc_args(argc, argv);
}

static void print_usage(char **argv)
{
	printf("Usage: one of the following:\n"
			"\t service run %s -args \"scan <decimal no.- asm = 1, c = 0>\"\n"
			"\t service run %s -args \"leds <decimal no. -leds>\"\n"
			"\t service run %s -args \"timed <decimal no. -n>\"\n",
			argv[0], argv[0], argv[0]);
}

static int proc_args(int argc, char **argv)
{
	unsigned long scan, leds;
	if (strncmp(argv[1], "scan", strlen("scan")) == 0) {
		if (argc != 3) {
			printf("scan: wrong no. of arguments for kbd_test_scan()\n");
			return 1;
		}
		unsigned short ass;
		//ass = parse_ulong(argv[2], 10);						/* Parses string to unsigned long */
		if (strncmp(argv[2], "0", 1) == 0)
			ass = 0;
		else if(strncmp(argv[2], "1" ,1) == 0)
			ass = 1;
		printf("keyboard::kbd_test_scan(%lu)\n", scan);
		return kbd_test_scan(ass);
	}
	else if (strncmp(argv[1], "leds", strlen("leds")) == 0) {
		if (argc < 3) {
			printf("leds: wrong no. of arguments for kbd_test_leds()\n");
			return 1;
		}
		int n = argc - 2;
		unsigned short *l = (unsigned short *) malloc((sizeof(unsigned short))*n);

		int i=0;
		while(i < n){
			unsigned short x = parse_ulong(argv[i + 2], 10);
			if(x < 0 || x > 2){
				printf("Invalid input of leds");
				return 1;
			}
			l[i] = x;
			i++;
		}
		printf("keyboard::kbd_test_leds(%lu)\n", leds);
		return kbd_test_leds((unsigned short) n, l);
	}
	else if (strncmp(argv[1], "timed", strlen("timed")) == 0) {
		if (argc != 3) {
			printf("timed: wrong no. of arguments for kbd_test_timed_scan()\n");
			return 1;
		}
		unsigned short n;
		n = parse_ulong(argv[2], 10);
		printf("keyboard::kbd_test_timed_scan(%lu)\n", scan);
		return kbd_test_timed_scan(n);
	}
	else {
		printf("timer: %s - no valid function!\n", argv[1]);
		return 1;
	}
}

static unsigned long parse_ulong(char *str, int base)
{
	char *endptr;
	unsigned long val;

	/* Convert string to unsigned long */
	val = strtoul(str, &endptr, base);

	/* Check for conversion errors */
	if ((errno == ERANGE && val == ULONG_MAX) || (errno != 0 && val == 0)) {
		perror("strtoul");
		return ULONG_MAX;
	}

	if (endptr == str) {
		printf("timer: parse_ulong: no digits were found in %s\n", str);
		return ULONG_MAX;
	}

	/* Successful conversion */
	return val;
}

