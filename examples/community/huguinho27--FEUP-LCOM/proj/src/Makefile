# Makefile for project

COMPILER_TYPE= gnu

CC= gcc

PROG= proj
SRCS= video_gr.c kbd.c sprite.c read_xpm.c timer.c main.c mouse.c vbe.c Bitmap.c Balloon.c hero.c bow_and_arrow.c arrow.c menu.c rtc.c
CCFLAGS= -Wall

DPADD+= ${LIBDRIVER} ${LIBSYS}
LDADD+= -llm -ldriver -lsys

LDFLAGS+= -L .

MAN=

.include <bsd.prog.mk>
.include <bsd.gcc.mk>