# Makefile for projecto (based on that of hello).
PROG=	projectojogo
SRCS=	TUI.c Base.c Jogo.c main.c video_txt.c kbc.c prospeaker.c protimer.c


DPADD+=	${LIBDRIVER} ${LIBSYS} libvt.a
LDADD+=	-lvt -ldriver -lsys 
LDFLAGS+= -L. 

MAN=

BINDIR?= /usr/sbin

.include <bsd.prog.mk>
