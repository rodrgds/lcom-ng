# Makefile for proj

COMPILER_TYPE= gnu

CC= gcc

PROG= proj
SRCS= graphics.c timer.c mouse.c keyboard.c handler.c read_xpm.c rtc.c

CCFLAGS= -Wall

DPADD+= ${LIBDRIVER} ${LIBSYS}
LDADD+= -llm -ldriver -lsys

LDFLAGS+= -L .

MAN=

.include <bsd.prog.mk>
.include <bsd.gcc.mk>
 