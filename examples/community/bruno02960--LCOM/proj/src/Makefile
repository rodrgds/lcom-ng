# Makefile for Space-Invaders

COMPILER_TYPE= gnu

CC= gcc

PROG= Space-Invaders
SRCS = spaceinvaders.c
SRCS += video_gr.c print.c devices.c timer.c keyboard.c xpm.c mouse.c game.c rtc.c utilities.c file_handler.c

CCFLAGS= -Wall

DPADD+= ${LIBDRIVER} ${LIBSYS}
LDADD+= -llm -ldriver -lsys

LDFLAGS+= -L .

MAN=

BINDIR?= /usr/sbin

.include <bsd.prog.mk>
.include <bsd.gcc.mk>