# name of the program (Minix service)
PROG=proj

.PATH: ${.CURDIR}/drivers
.PATH: ${.CURDIR}/drivers/video
.PATH: ${.CURDIR}/drivers/kbc
.PATH: ${.CURDIR}/drivers/timer
.PATH: ${.CURDIR}/states

SRCS = main.c game.c grid.c graphics.c keyboard.c mouse.c kbc_utils.c timer.c utils.c ui_assets.c levels.c start_menu.c instructions.c show_original.c user_paint.c results.c util_states.c sprite.c

# additional compilation flags
# "-Wall -Wextra -Werror -I . -std=c11 -Wno-unused-parameter" are already set
CFLAGS += -pedantic -DLAB5 -O2

# list of library dependencies
DPADD += ${LIBLCF}
LDADD += -llcf -lxpm -llm

# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk>
