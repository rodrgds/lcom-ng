# Makefile for the lab1 driver (based on that of hello).

COMPILER_TYPE= gnu

CC=gcc

PROG=	proj
SRCS=	main.c video_gr.c  vbe.c game.c keyboard.c timer.c

CCFLAGS= -Wall


DPADD+=	${LIBDRIVER} ${LIBSYS}
LDADD+= -ldriver -lsys

DPADD+=	${LIBDRIVER} ${LIBSYS} liblm.a
LDADD+=	-llm -ldriver -lsys


LDFLAGS+= -L .

MAN=

.include <bsd.prog.mk>
.include <bsd.gcc.mk>