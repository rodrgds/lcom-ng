# Makefile for the lab1 driver (based on that of hello).

COMPILER_TYPE= gnu

CC=gcc

PROG=	proj
SRCS=	proj.c graphics.c vbe.c video_gr.c Utilities.c  kbd.c pixmap.c buffer.c timer.c data_pixmap.c EndScreen.c
SRCS+=  GameController.c sprite.c keyStates.c mouse.c scope.c boat.c collision.c vector2.c shark.c rtc.c
SRCS+=  multiplayerSync.c test7.c uart.c
CCFLAGS= -Wall

DPADD+=	${LIBDRIVER} ${LIBSYS}
LDADD+= -llm -ldriver -lsys

LDFLAGS+= -L .

MAN=

.include <bsd.prog.mk>
.include <bsd.gcc.mk>
