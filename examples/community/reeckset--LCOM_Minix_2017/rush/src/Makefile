COMPILER_TYPE= gnu
CC=gcc
PROG= rush
SRCS= rush.c bitmap.c gameEngine.c vbe.c video_gr.c keyboard.c timer.c gameObjects.c ev_queue.c mainGame.c stateMachine.c mouse.c rtc.c textRender.c
CFLAGS= -Wall
DPADD+= ${LIBDRIVER} ${LIBSYS}
LDADD+= -llm -ldriver -lsys
LDFLAGS+= -L .
MAN=
BINDIR?= /usr/sbin
.include <bsd.prog.mk>
.include <bsd.gcc.mk>
