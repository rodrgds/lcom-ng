# name of the program (Minix service)
PROG=proj


.PATH: ${.CURDIR}/controller/
.PATH: ${.CURDIR}/controller/mouse
.PATH: ${.CURDIR}/controller/keyboard
.PATH: ${.CURDIR}/controller/timer
.PATH: ${.CURDIR}/controller/video
.PATH: ${.CURDIR}/controller/utils
.PATH: ${.CURDIR}/controller/realTimeClock
.PATH: ${.CURDIR}/game/
.PATH: ${.CURDIR}/game/snake
.PATH: ${.CURDIR}/game/apple
.PATH: ${.CURDIR}/game/tongue
.PATH: ${.CURDIR}/game/game
.PATH: ${.CURDIR}/game/score


# source code files to be compiled
SRCS = proj.c graphics.c keyboard.c timer.c utils.c kbc.c mouse.c snake.c apple.c tongue.c pixmap.c game.c rtc.c score.c

# additional compilation flags
# "-Wall -Wextra -Werror -I . -std=c11 -Wno-unused-parameter" are already set
CFLAGS += -pedantic 

# list of library dependencies (for Lab 2, only LCF library)
DPADD += ${LIBLCF}
LDADD += -llcf

# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk>
