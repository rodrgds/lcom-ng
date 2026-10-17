# Makefile for the proj driver (based on that of lab1, lab2, lab3, lab4 and lab5).

COMPILER_TYPE= gnu

CC=gcc

PROG=	proj
SRCS=	proj.c game.c vbe.c video_gr.c keyboard.c timer.c rtc.c dispatcher.c read_xpm.c mouse.c logic.c images.c highscore.c menu.c
CCFLAGS= -Wall

DPADD+=	${LIBDRIVER} ${LIBSYS}
LDADD+= -llm -ldriver -lsys

LDFLAGS+= -L .

MAN=
.include <bsd.prog.mk>
.include <bsd.gcc.mk>

trash:
	rm -rf *.o *.d

clean:
	rm -rf *.o *.d ${PROG}
