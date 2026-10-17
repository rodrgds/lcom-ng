COMPILER_TYPE= gnu

CC=gcc

PROG=	proj
SRCS=	proj.c vbe.c timer.c interface.c keyboard.c mouse.c rtc.c
SRCS+=	Bitmap.c Game.c MainMenuState.c Obstacles.c  Player.c GameState.c Collisions.c score.c Button.c Pokeball.c FinalState.c fonts.c

CCFLAGS= -Wall

DPADD+=	${LIBDRIVER} ${LIBSYS}
LDADD+=	-llm -ldriver -lsys

LDFLAGS+= -L .

MAN=

.include <bsd.prog.mk>
.include <bsd.gcc.mk>