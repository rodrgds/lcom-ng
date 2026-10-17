# Makefile for the lab4 driver (based on that of hello).

COMPILER_TYPE= gnu

CC=gcc

PROG=	projeto
SRCS=	main.c jogo.c sprite.c jogador.c plataforma.c kbc.c vbe.c video_gr.c read_xpm.c log.c image.c

CCFLAGS= -Wall

DPADD+=	${LIBDRIVER} ${LIBSYS}
LDADD+=	-llm -ldriver -lsys

LDFLAGS+= -L .

MAN=

.include <bsd.prog.mk>
.include <bsd.gcc.mk>
