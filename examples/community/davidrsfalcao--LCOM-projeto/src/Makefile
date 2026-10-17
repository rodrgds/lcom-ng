COMPILER_TYPE= gnu

CC=gcc

PROG=	projeto
SRCS= galatic_war.c
SRCS+=	keyboard.c timer.c vbe.c video_gr.c mouse.c RTC.c
SRCS+= game.c space_ship.c asteroid.c bullets.c menu.c cursor.c collisions.c game_over.c
CCFLAGS= -Wall

DPADD+=	${LIBDRIVER} ${LIBSYS}
LDADD+=	-llm -ldriver -lsys 

LDFLAGS+= -L .

MAN=

.include <bsd.prog.mk>
.include <bsd.gcc.mk>

clean:
	rm -rf *o *d ${PROG}