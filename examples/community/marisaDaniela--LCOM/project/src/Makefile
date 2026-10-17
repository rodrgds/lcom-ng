COMPILER_TYPE = gnu

CC = gcc

PROG = project
SRCS = bitmap.c fruit.c game.c graphics.c kbd.c main.c mouse.c snake.c timer.c utils.c vbe.c point.c

CCFLAGS= -Wall -O3

DPADD += ${LIBDRIVER} ${LIBSYS}
LDADD += -llm -ldriver -lsys

LDFLAGS += -L.

MAN=
BINDIR? = /usr/sbin

.include <bsd.prog.mk>
.include <bsd.gcc.mk>