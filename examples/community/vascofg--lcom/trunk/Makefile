# Makefile for lab2 (based on that of hello).
PROG=	batalha_naval
SRCS=	vbe.c video_gr.c main.c xpm.c kbc.c keyboard.c interface.c battleship.c

DPADD+=	${LIBDRIVER} ${LIBSYS} liblm.a
LDADD+=	-llm -ldriver -lsys 
LDFLAGS+= -L. 

MAN=

BINDIR?= /usr/sbin

.include <bsd.prog.mk>

clean:
	rm -rf *.o batalha_naval.out
	rm -rf *.d batalha_naval.out
