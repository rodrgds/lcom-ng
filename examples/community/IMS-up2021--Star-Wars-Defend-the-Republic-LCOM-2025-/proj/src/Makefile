# name of the program (Minix service)
PROG=proj

.PATH: ${.CURDIR}/controllers/
.PATH: ${.CURDIR}/controllers/video/
.PATH: ${.CURDIR}/controllers/keyboardMouse/
.PATH: ${.CURDIR}/controllers/timer/
.PATH: ${.CURDIR}/handlers/
.PATH: ${.CURDIR}/game/

# additional compilation flags
# "-Wall -Wextra -Werror -I . -std=c11 -Wno-unused-parameter" are already set
CFLAGS += -pedantic -DPROJ -O2

# source code files to be compiled
SRCS = main.c graphics.c keyboard.c KBC.c timer.c mouse.c manager.c utils.c entity.c gameState.c menu.c mouse_handler.c timer_handler.c kbd_handler.c enemy.c characters.c game.c health.c


# additional dependencies and libraries
DPADD += ${LIBLCF}
LDADD += -llcf -lxpm -llm

# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk>
