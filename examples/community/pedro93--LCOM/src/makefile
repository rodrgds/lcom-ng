# Makefile for commodore

COMPILER_TYPE= gnu

CC=gcc

PROG=	commodore

SRCS=	vbe.c video_gr.c commodore.c read_xpm.c sprite.c RTC.c speaker.c timer.c main.c interupts.c kbd.c

CCFLAGS= -Wall -DDEBUG

DPADD+=	${LIBDRIVER} ${LIBSYS} liblm.a

LDADD+=	-llm -ldriver -lsys -lm

LDFLAGS+= -L.

MAN=

BINDIR?= /usr/sbin

.include <bsd.prog.mk>

.include <bsd.gcc.mk>