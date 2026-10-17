#Makefile for Project duelnix

COMPILER_TYPE= gnu

CC=gcc

PROG= duelnix

SRCS = main.c
SRCS += bitmap.c graphics.c keyboard.c mouse.c timer.c vbe.c
SRCS += duelnix.c cowboy.c rectangle.c target.c shield.c bullet.c table.c rtc.c
SRCS += PracticeState.c MainMenuState.c GameState.c Multiplayer.c
SRCS += kbd_asm.S

CCFLAGS= -Wall

DPADD+= ${LIBDRIVER} ${LIBSYS}

LDADD+= -llm -ldriver -lsys

LDFLAGS+= -L .

MAN=

BINDIR?= /usr/sbin

.include <bsd.prog.mk>
.include <bsd.gcc.mk>