# Makefile for project

COMPILER_TYPE= gnu

CC= gcc

PROG= mosu
SRCS= mosu.c i8042.c vbe.c video_gr.c timer.c mouse.c keyboard.c rtc.c read_xpm.c sprite.c game.c circle.c cursor.c queue.c game_core_st.c pixmap.c

CPPFLAGS+= -D MOSU
CFLAGS= -Wall

DPADD+= ${LIBDRIVER} ${LIBSYS}
LDADD+= -llm -ldriver -lsys

LDFLAGS+= -L .

MAN=

BINDIR?= /usr/sbin

.include <bsd.prog.mk>
.include <bsd.gcc.mk>