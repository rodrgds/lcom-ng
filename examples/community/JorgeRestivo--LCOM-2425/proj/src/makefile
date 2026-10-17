# name of the program (Minix service)
PROG=proj


.PATH: ${.CURDIR}/graphics
.PATH: ${.CURDIR}/input_devices
.PATH: ${.CURDIR}/game_core
.PATH: ${.CURDIR}/sprites
.PATH: ${.CURDIR}/leaderboard
.PATH: ${.CURDIR}/menu

# Source code files in subfolders
SRCS = main.c proj.c game.c keyboard.c mouse.c timer.c kbc.c sprites.c utils.c graphics.c leaderboard.c hud.c font8x8.c menu.c

# additional compilation flags
# "-Wall -Wextra -Werror -I . -std=c11 -Wno-unused-parameter" are already set
CFLAGS += -pedantic -Igame_core -Iinput_devices -Igraphics -Isprites -Ileaderboard -DLCOM_OPTIMIZED_

# list of library dependencies (for Lab 2, only LCF library)
DPADD += ${LIBLCF}
LDADD += -llcf -lxpm -llm

# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk>
