# Makefile for the Battle Tetris Project

COMPILER_TYPE= gnu

CC=gcc

PROG =	proj
SRCS =	proj.c handler.c
SRCS += vbe.c video_gr.c timer.c kbd.c mouse.c rtc.c serial.c
SRCS += logic.c menu.c piece.c game.c bitmap.c character.c


CCFLAGS= -Wall

DPADD+=	${LIBDRIVER} ${LIBSYS}
LDADD+= -llm -ldriver -lsys

LDFLAGS+= -L .

MAN=

.include <bsd.prog.mk>
.include <bsd.gcc.mk>
