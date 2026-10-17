# name of the program (Minix service)
PROG=proj

# No RegEx in Makefile for some reason
.PATH: ${.CURDIR}/drivers/graphics
.PATH: ${.CURDIR}/drivers/kbc
.PATH: ${.CURDIR}/drivers/timer
.PATH: ${.CURDIR}/drivers/rtc
.PATH: ${.CURDIR}/drivers/serial_port
.PATH: ${.CURDIR}/drivers
.PATH: ${.CURDIR}/sprites
.PATH: ${.CURDIR}/maps
.PATH: ${.CURDIR}/count_down
.PATH: ${.CURDIR}/characters
.PATH: ${.CURDIR}/objects
.PATH: ${.CURDIR}/communication

# source code files to be compiled
SRCS = 	utils.c kbc.c keyboard.c mouse.c timer.c rtc.c sprite.c graphics.c count_down.c character.c map.c game.c proj.c block.c ser.c queue.c communication.c

# additional compilation flags
# "-Wall -Wextra -Werror -I . -std=c11 -Wno-unused-parameter" are already set
CFLAGS += -pedantic

# list of library dependencies
DPADD += ${LIBLCF}
LDADD += -llcf -lxpm -llm

# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk>

