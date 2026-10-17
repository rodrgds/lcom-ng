COMPILER_TYPE= gnu

CC= gcc

PROG= proj

SRCS= keyboard.c main.c menus.c mouse.c timer.c vbe.c game.c graphics.c rtc.c files.c

CCFLAGS= -Wall

DPADD+= ${LIBDRIVER} ${LIBSYS}

LDADD+= -ldriver -lsys -llm

LDFLAGS+= -L .

MAN=

.include <bsd.prog.mk>

.include <bsd.gcc.mk>

clean:

	rm -rf *o *d ${PROG}
