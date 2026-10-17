# name of the program (Minix service)
PROG=proj

# source code files to be compiled
.PATH : ${.CURDIR}/Modules ${.CURDIR}/controller ${.CURDIR}/view
SRCS = main.c vg.c timer.c utils.c  keyboard.c mouse.c states.c draw_functions.c menu_controller.c game_controller.c draw_game.c
# additional compilation flags
# "-Wall -Wextra -Werror -I . -std=c11 -Wno-unused-parameter" are already set
CFLAGS += -pedantic -O2

# list of library dependencies (for Lab 2, only LCF library)
DPADD += ${LIBLCF}
LDADD += -llcf

# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk>
