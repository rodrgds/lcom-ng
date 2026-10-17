# Makefile for the lab1 driver (based on that of hello).

COMPILER_TYPE= gnu

CC=gcc

PROG=	scoreboard
SRCS=	main.c Serial.c video_gr.c vbe.c

CCFLAGS= -Wall

DPADD+=	${LIBDRIVER} ${LIBSYS}
LDADD+=	-llm -ldriver -lsys

LDFLAGS+= -L .

MAN=

clean:
	rm *.d

.include <bsd.prog.mk>
.include <bsd.gcc.mk>


