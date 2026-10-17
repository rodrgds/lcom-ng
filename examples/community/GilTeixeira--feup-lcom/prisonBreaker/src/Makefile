COMPILER_TYPE = gnu

CC = gcc

PROG = prisonBreaker
SRCS = main.c video_gr.c vbe.c timer.c kbd.c prisonBreaker.c bitmap.c game.c square.c level.c gameState.c mouse.c

CCFLAGS= -Wall -O3

DPADD += ${LIBDRIVER} ${LIBSYS}
LDADD += -llm -ldriver -lsys

LDFLAGS += -L.

MAN=
BINDIR? = /usr/sbin

.include <bsd.prog.mk>
.include <bsd.gcc.mk>