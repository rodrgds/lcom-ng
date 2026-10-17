# Makefile for project

COMPILER_TYPE= gnu

CC= gcc

PROG= proj
SRCS= main.c bitmap.c utilities.c obstacleCourse.c graphics.c
SRCS+= video_gr.c vbe.c keyboard.c timer.c mouse.c rtc.c
SRCS+= startMenuState.c gameState.c
SRCS+= rectangle.c

CFLAGS= -Wall

DPADD+= ${LIBDRIVER} ${LIBSYS}
LDADD+= -llm -ldriver -lsys

LDFLAGS+= -L .

MAN=
BINDIR?= /usr/sbin
.include <bsd.prog.mk>
.include <bsd.gcc.mk>

clean:
	rm -f *.o *.d  .depend