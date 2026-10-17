# name of the program (Minix service)
PROG=proj

.PATH: ${.CURDIR}
.PATH: ${.CURDIR}/drivers
.PATH: ${.CURDIR}/game
.PATH: ${.CURDIR}/images

# source code files to be compiled
SRCS = main.c timer.c utils.c video.c graphics.c map.c player.c rtc.c game_state.c kbc.c mouse.c driver_handler.c cursor.c clock.c serial_port.c intercom.c

# additional compilation flags
# "-Wall -Wextra -Werror -I . -std=c11 -Wno-unused-parameter" are already set
CPPFLAGS += -pedantic

# list of library dependencies (for Lab 2, only LCF library)
DPADD += ${LIBLCF}
LDADD += -llcf -lm

# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk>
