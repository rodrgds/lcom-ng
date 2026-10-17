# name of the program (Minix service)
PROG=proj

.PATH: ${.CURDIR}
.PATH: ${.CURDIR}/devices/graphics
.PATH: ${.CURDIR}/devices/kbc
.PATH: ${.CURDIR}/devices/timer
.PATH: ${.CURDIR}/devices/rtc
.PATH: ${.CURDIR}/devices/uart
.PATH: ${.CURDIR}/devices/uart/queue
.PATH: ${.CURDIR}/game

# source code files to be compiled
SRCS = proj.c keyboard.c mouse.c graphics.c utils.c event_handler.c game.c sprite.c target.c position.h timer.c menu.c rtc.c dynamite.c gameover.c player.c queue.c uart.c wait_menu.c

# additional compilation flags
# "-Wall -Wextra -Werror -I . -std=c11 -Wno-unused-parameter" are already set
CFLAGS += -std=c11 -pedantic
CPPFLAGS += -std=c11 -pedantic  -D __LCOM_OPTIMIZED__

# list of library dependencies (for Lab 2, only LCF library)
DPADD += ${LIBLCF}
LDADD += -llcf

# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk>
