# name of the program (Minix service)
PROG=proj

.PATH: ${.CURDIR}/controllers/gpu
.PATH: ${.CURDIR}/controllers/gpu/sprites
.PATH: ${.CURDIR}/controllers/KBC
.PATH: ${.CURDIR}/controllers/KBC/keyboard
.PATH: ${.CURDIR}/controllers/KBC/mouse
.PATH: ${.CURDIR}/controllers/timer
.PATH: ${.CURDIR}/controllers/sp
.PATH: ${.CURDIR}/controllers
.PATH: ${.CURDIR}/game/elements
.PATH: ${.CURDIR}/game
.PATH: ${.CURDIR}/menu
.PATH: ${.CURDIR}/input

# source code files to be compiled
SRCS = proj.c gpu.c KBC.c timer.c utils.c keyboard.c sprite.c mouse.c menu.c game.c input.c snake.c freesnake.c choose_input_method.c apple.c score.c dynamicSprite.c text.c sp.c gamestate.c select_mode.c local_or_remote.c

# "-Wall -Wextra -Werror -I . -std=c11 -Wno-unused-parameter" are already set
CFLAGS += -pedantic -DLAB4

# list of library dependencies (for Lab 2, only LCF library)
DPADD += ${LIBLCF}
LDADD += -llcf

# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk>

all: ${PROG}
	${MAKE} clean-objects

clean-objects:
	rm -f *.o
