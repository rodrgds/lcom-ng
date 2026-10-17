# Makefile for the Final Project

COMPILER_TYPE= gnu

CC=gcc

PROG=	Projecto
SRCS=	Menu.c Jogo.c ModGrafico.c ModPortaSerie.c ModRato.c ModRTC.c ModTeclado.c ModTimer.c bitmap.c

CCFLAGS= -Wall
 
DPADD+=	${LIBDRIVER} ${LIBSYS}
LDADD+=	-llm -ldriver -lsys

LDFLAGS+= -L .

MAN=

.include <bsd.prog.mk>
.include <bsd.gcc.mk>
