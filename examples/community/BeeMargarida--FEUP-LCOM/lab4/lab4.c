
#include "mouse.h"
#include "test4.h"
#include <limits.h>
#include <string.h>
#include <errno.h>
#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/sysutil.h>
#include <minix/com.h>

//#include "kbd.h"
/*
static void print_usage(char *argv[]);
static int proc_args(int argc, char *argv[]);
static unsigned long parse_ulong(char *str, int base);

int main(int argc, char *argv[]) {

	sef_startup();

	if (argc == 1) {
		print_usage(argv);
		return 0;
	} else
		proc_args(argc, argv);

	return 0;
}




static void print_usage(char *argv[]) {
	printf(
			"\n\nUsage: one of the following:\n"
			"    service run %s -args \"packet <int packet>\" \n"
			"    service run %s -args \"async <int time-idle>\"\n"
			"    service run %s -args \"config\"\n"
			"    service run %s -args \"gesture <short length, unsigned short tolerance>\" \n",
			argv[0], argv[0], argv[0], argv[0]);
}

static int proc_args(int argc, char *argv[]) {

	if (strncmp(argv[1], "packet", strlen("packet")) == 0) {
		if (argc != 3) {
			printf("Wrong number of args for test_packet()\n");
			return 1;
		}

		unsigned short packets = parse_ulong(argv[2], 10);

		printf("Packets:\n\n", packets);

		test_packet(packets);

		return 0;
	}

	else if (strncmp(argv[1], "async", strlen("async")) == 0) {
		if (argc != 3) {
			printf("Wrong number of args for test_async()\n");
			return 1;
		}

		unsigned short time = parse_ulong(argv[2], 10);

		test_async(time);

		return 0;
	}

	else if (strncmp(argv[1], "test_config", strlen("test_config")) == 0) {
		if (argc != 2) {
			printf("Wrong number of args for test_config()\n");
			return 1;
		}

		printf("Mouse config:\n\n");

		test_config();

		return 0;
	}
}

//given in lab2
static unsigned long parse_ulong(char *str, int base) {
	char *endptr;
	unsigned long val;

	val = strtoul(str, &endptr, base);

	if ((errno == ERANGE && val == ULONG_MAX )
			|| (errno != 0 && val == 0)) {
		perror("strtol");
		return ULONG_MAX;
	}

	if (endptr == str) {
		printf("video_txt: parse_ulong: no digits were found in %s \n", str);
		return ULONG_MAX;
	}

	return val;
}

 */


static int proc_args(int argc, char *argv[]);
static unsigned long parse_ulong(char *str, int base);
static void print_usage(char *argv[]);

int main(int argc, char **argv) {

	sef_startup();

	//sys_enable_iop(SELF);

	if (argc == 1) {
		print_usage(argv);
		return 0;
	} else
		return proc_args(argc, argv);
}

static void print_usage(char *argv[]) {
	printf(
			"Usage: one of the following:\n"
			"    service run %s -args \"packet <int packet>\" \n"
			"    service run %s -args \"async <int time-idle>\"\n"
			"    service run %s -args \"config\"\n"
			"    service run %s -args \"gesture <short length>\" \n",
			argv[0], argv[0], argv[0], argv[0]);
}

static int proc_args(int argc, char *argv[]) {
	if (strncmp(argv[1], "packet", strlen("packet")) == 0) {
		if (argc != 3) {
			printf("Invalid number of arguments for test_packet()!\n");
			return 1;
		}
		unsigned short packets;
		packets = parse_ulong(argv[2], 10);
		printf("Packets:(%lu)\n", packets);
		return test_packet(packets);

	} else if (strncmp(argv[1], "async", strlen("async")) == 0) {
		if (argc != 3) {
			printf("Wrong number of args for test_async()\n");
			return 1;
		}

		unsigned short time = parse_ulong(argv[2], 10);

		return test_async(time);

	} else if (strncmp(argv[1], "config", strlen("config")) == 0) {
		if (argc != 2) {
			printf("Wrong number of args for test_config()\n");
			return 1;
		}

		printf("Mouse config:\n\n");
		return test_config();

	} else if (strncmp(argv[1], "gesture", strlen("gesture")) == 0){
		if(argc != 3){
			printf("Wrong number of args for test_gesture()\n");
			return 1;
		}

		short length = parse_ulong(argv[2],10);

		printf("Mouse gesture:\n");
		return test_gesture(length);


	}else {
		printf("timer: %s - no valid function!\n", argv[1]);
		return 1;
	}
}

static unsigned long parse_ulong(char *str, int base) {
	char *endptr;
	unsigned long val;


	val = strtoul(str, &endptr, base);


	if ((errno == ERANGE && val == ULONG_MAX) || (errno != 0 && val == 0)) {
		perror("strtoul");
		return ULONG_MAX;
	}

	if (endptr == str) {
		printf("timer: parse_ulong: no digits were found in %s\n", str);
		return ULONG_MAX;
	}


	return val;
}

