# name of the program (Minix service)
PROG=proj

# source code files to be compiled
.PATH: Game Drivers/Keyboard Drivers/Utils Drivers/Mouse Drivers/Timer Drivers/VBE Sprites

# Source files (no directories!)
SRCS= main.c game.c menu.c game_logic.c pause_menu.c game_state.c score.c keyboard.c utils.c mouse.c timer.c vbe.c


# additional compilation flags
# "-Wall -Wextra -Werror -I . -std=c11 -Wno-unused-parameter" are already set
CFLAGS += -pedantic -ISprites -D_LCOM_OPTIMIZED_

# list of library dependencies (for Lab 2, only LCF library)
DPADD += ${LIBLCF}
LDADD += -llcf

# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk>
