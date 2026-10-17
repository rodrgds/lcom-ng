#include "test5.h"
/*#include <limits.h>
#include <string.h>
#include <errno.h>
#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/sysutil.h>
#include <minix/com.h>*/


static int proc_args(int argc, char *argv[]);
static unsigned long parse_ulong(char *str, int base);
static void print_usage(char *argv[]);

int main(int argc, char **argv) {

	sef_startup();

	if (argc == 1) {
		print_usage(argv);
		return 0;
	} else
		return proc_args(argc, argv);
}

static void print_usage(char *argv[]) {
	printf(
			"Usage: one of the following:\n"
			"    service run %s -args \"init <short mode, short delay>\" \n"
			"    service run %s -args \"square <short x, short y, short size, long color>\"\n"
			"    service run %s -args \"line <short xi, short yi, short xi, short xf, long color>\"\n"
			"    service run %s -args \"xpm <short x, short y, short pixmap <0-4> >\" \n"
			"    service run %s -args \"move <short xi, short yi, short pixmap <0 - 4>, short hor, short delta, short time>\" \n"
			"    service run %s -args \"controller \" \n",
			argv[0], argv[0], argv[0], argv[0],argv[0],argv[0],argv[0]);
}

static int proc_args(int argc, char *argv[]) {
	if (strncmp(argv[1], "init", strlen("init")) == 0) {
		if (argc != 4) {
			printf("Invalid number of arguments for test_init()!\n");
			return 1;
		}
		unsigned short mode;
		mode = parse_ulong(argv[2], 10);
		unsigned short delay;
		delay = parse_ulong(argv[3],10);
		printf("Init:(%lu)\n");
		return test_init(mode, delay);

	} else if (strncmp(argv[1], "square", strlen("square")) == 0) {
		if (argc != 6) {
			printf("Wrong number of args for test_square()\n");
			return 1;
		}
		unsigned short x = parse_ulong(argv[2], 10);
		printf("%d", x);
		unsigned short y = parse_ulong(argv[3], 10);
		unsigned short size = parse_ulong(argv[4], 10);
		unsigned short color = parse_ulong(argv[5], 10);
		printf("Square:\n");
		return test_square(x,y,size,color);

	} else if (strncmp(argv[1], "line", strlen("line")) == 0) {
		if (argc != 7) {
			printf("Wrong number of args for test_line()\n");
			return 1;
		}
		unsigned short xi = parse_ulong(argv[2], 10);
		unsigned short yi = parse_ulong(argv[3], 10);
		unsigned short xf = parse_ulong(argv[4], 10);
		unsigned short yf = parse_ulong(argv[5], 10);
		unsigned short color = parse_ulong(argv[6], 10);

		printf("Line:\n");
		return test_line(xi,yi,xf,yf,color);

	} else if (strncmp(argv[1], "xpm", strlen("xpm")) == 0){
		if(argc != 5){
			printf("Wrong number of args for test_xpm()\n");
			return 1;
		}
		char **xpm;
		unsigned short x = parse_ulong(argv[2], 10);
		unsigned short y = parse_ulong(argv[3], 10);
		unsigned short pixmap = parse_ulong(argv[4],10);
		if(pixmap == 0)
			xpm =pic1;
		else if(pixmap == 1)
			xpm = pic2;
		else if (pixmap == 2)
			xpm = cross;
		else if (pixmap == 3)
			xpm = pic3;
		else if (pixmap == 4)
			xpm = penguin;
		else
		{
			printf("Choice of pixmap not valid <0-4>\n");
			return 1;
		}

		printf("Xpm:\n");
		return test_xpm(x,y, xpm);


	}else if(strncmp(argv[1], "move", strlen("move")) == 0){
		if(argc != 8){
			printf("Wrong number of args for test_move()\n");
			return 1;
		}
		char **xpm;
		unsigned short x = parse_ulong(argv[2], 10);
		unsigned short y = parse_ulong(argv[3], 10);
		unsigned short pixmap = parse_ulong(argv[4],10);
		if(pixmap == 0)
			xpm =pic1;
		else if(pixmap == 1)
			xpm = pic2;
		else if (pixmap == 2)
			xpm = cross;
		else if (pixmap == 3)
			xpm = pic3;
		else if (pixmap == 4)
			xpm = penguin;
		else
		{
			printf("Choice of pixmap not valid <0-4>\n");
			return 1;
		}
		unsigned short hor = parse_ulong(argv[5], 10);
		unsigned short delta = parse_ulong(argv[6], 10);
		unsigned short time = parse_ulong(argv[7],10);

		printf("Move:\n");
		return test_move(x,y,xpm,hor,delta,time);


	}else if(strncmp(argv[1], "move", strlen("move")) == 0){
		if(argc != 2){
			printf("Wrong number of args for test_controller()\n");
			return 1;
		}

		return test_controller();

	}else {
		printf("vbe: %s - no valid function!\n", argv[1]);
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
		printf("vbe: parse_ulong: no digits were found in %s\n", str);
		return ULONG_MAX;
	}


	return val;
}
