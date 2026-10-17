# Makefile for HUEtris

COMPILER_TYPE= gnu

CC=gcc

PROG=	huetris
SRCS=	board.c main.c scene_sp.c scene.c tetriminos.c video_gr.c vbe.c timer.c interrupt.c kbc.c read_xpm.c scene_mp.c mouse.c rtc.c scene_mm.c scene_eg.c

CCFLAGS= -Wall

DPADD+=	${LIBDRIVER} ${LIBSYS}
LDADD+=	-lvt -llm -ldriver -lsys

LDFLAGS+= -L .

MAN=

.include <bsd.prog.mk>
.include <bsd.gcc.mk>
