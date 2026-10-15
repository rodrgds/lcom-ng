# name of the program (Minix service)
PROG=proj

.PATH: ${.CURDIR}/controllers/
.PATH: ${.CURDIR}/controllers/KBC
.PATH: ${.CURDIR}/controllers/video
.PATH: ${.CURDIR}/controllers/timer
.PATH: ${.CURDIR}/model/
.PATH: ${.CURDIR}/view/
.PATH: ${.CURDIR}/utils/

# source code files to be compiled
SRCS = 	main.c timer.c random.c utils.c video.c KBC.c keyboard.c mouse.c state.c modes.c view.c font8x8_basic.c player_menu.c gamemode_menu.c react_mode.c guess_mode.c game_over_menu.c instructions.c game_over_view.c react_mode_view.c guess_mode_view.c player_menu_view.c instructions_view.c gamemode_menu_view.c

# additional compilation flags
# "-Wall -Wextra -Werror -I . -std=c11 -Wno-unused-parameter" are already set
CFLAGS += -pedantic -D_LCOM_OPTIMIZED_ -O2

# list of library dependencies (for Lab 2, only LCF library)
DPADD += ${LIBLCF}
LDADD += -Wall -llcf -lxpm -llm

# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk>