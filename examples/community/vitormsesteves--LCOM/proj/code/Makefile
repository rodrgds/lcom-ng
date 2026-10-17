COMPILER_TYPE= gnu

CC=gcc

PROG=	mouse-maze
SRCS+=	main.c mouse.c vbe.c video_gr.c timer.c 
SRCS+=	keyboard.c kbc.c mouse-maze.c rtc.c bitmap.c  
SRCS+= 	rectangle.c labyrinth.c MainMenuState.c GameState.c 
SRCS+=	ModeState.c WinState.c LoseState.c 
CCFLAGS= -Wall

DPADD+=	${LIBDRIVER} ${LIBSYS}
LDADD+= -ldriver -lsys -llm

LDFLAGS+= -L .

MAN=

.include <bsd.prog.mk>
.include <bsd.gcc.mk>
