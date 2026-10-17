COMPILER_TYPE= gnu

CC=gcc

PROG= proj
SRCS=  vbe.c video_gr.c timer.c kbd.c Mario.c mouse.c Bitmap.c Background.c Pipe.c Button.c StateMachineMenu.c main.c MarioEvent.c rtc.c Colision.c IRQ.c

CCFLAGS= -Wall

DPADD+= ${LIBDRIVER} ${LIBSYS}
LDADD+= -llm -ldriver -lsys
LDFLAGS+= -L .
MAN=

BINDIR?= /usr/sbin

.include <bsd.prog.mk>
.include <bsd.gcc.mk>