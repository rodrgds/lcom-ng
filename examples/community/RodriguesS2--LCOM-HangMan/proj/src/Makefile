# name of the program (Minix service)
PROG=proj

.PATH: ${.CURDIR}/peripherals/
.PATH: ${.CURDIR}/peripherals/graphics
.PATH: ${.CURDIR}/peripherals/keyboard
.PATH: ${.CURDIR}/peripherals/mouse
.PATH: ${.CURDIR}/peripherals/timer
.PATH: ${.CURDIR}/game
.PATH: ${.CURDIR}/assets

SRCS = 	main.c timer.c utils.c graphics.c keyboard.c mouse.c hangman.c words.c k_controller.c menu.c font.c game.c instructions.c game_state.c categories.c


# additional compilation flags
# "-Wall -Wextra -Werror -I . -std=c11 -Wno-unused-parameter" are already set
CFLAGS += -pedantic -D_LCOM_OPTIMIZED_

# list of library dependencies
DPADD += ${LIBLCF}
LDADD += -llcf -lxpm -llm

# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk>