# Makefile for the lab1 driver (based on that of hello).

COMPILER_TYPE= gnu

CC=gcc

PROG=	projeto
SRCS=	main.c KBC.c KBC_mouse.c RTC.c timer.c vbe.c video_gr.c graphics.c sprites.c entities.c Player.c input.c game.c projectiles.c collisions.c enemies.c PowerUp.c scores.c Serial.c

CCFLAGS= -Wall

DPADD+=	${LIBDRIVER} ${LIBSYS}
LDADD+=	-llm -ldriver -lsys

LDFLAGS+= -L .

MAN=

clean:
	rm *.d

.include <bsd.prog.mk>
.include <bsd.gcc.mk>


