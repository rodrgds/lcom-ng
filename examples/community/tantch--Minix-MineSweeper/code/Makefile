

COMPILER_TYPE= gnu

CC=gcc

PROG=	game
SRCS=	game.c mouse.c vbe.c timer.c keyboard.c field.c multiplayer.c porta.c font.c rtc.c

CCFLAGS= -Wall

DPADD+=	${LIBDRIVER} ${LIBSYS}
LDADD+=	-llm -ldriver -lsys

LDFLAGS+= -L .

MAN=

.include <bsd.prog.mk>
.include <bsd.gcc.mk>
