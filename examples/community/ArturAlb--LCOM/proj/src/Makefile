# name of the program (Minix service)
PROG = proj 

.PATH: ${.CURDIR}/devices/KBC
.PATH: ${.CURDIR}/devices/RTC
.PATH: ${.CURDIR}/devices/timer
.PATH: ${.CURDIR}/devices/video_card
.PATH: ${.CURDIR}/game
.PATH: ${.CURDIR}/game/models
.PATH: ${.CURDIR}/game/states
.PATH: ${.CURDIR}/game/views

# source code files to be compiled
SRCS = main.c timer.c keyboard.c KBC.c video.c object.c canvas.c utils.c mouse.c image.c game.c gamestate.c bullet.c list.c rtc.c

# additional compilation flags
# "-Wall -Wextra -Werror -I . -std=c11 -Wno-unused-parameter" are already set
CFLAGS += -pedantic -D_LCOM_OPTIMIZED_

# list of library dependencies
DPADD += ${LIBLCF}
LDADD += -llcf

# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk>
