COMPILER_TYPE= gnu

CC=gcc

PROG=	proj
SRCS=	jogo.c proj.c kb.c rtc.c mouse.c timer.c vbe.c video_gr.c Bitmap.c#add all files
CCFLAGS= -Wall

DPADD+=	${LIBDRIVER} ${LIBSYS}
LDADD+=	-ldriver -lsys -llm

LDFLAGS+= -L .

MAN=

.include <bsd.prog.mk>
.include <bsd.gcc.mk>

clean:
	rm -rf *o *d ${PROG}