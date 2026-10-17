# name of the program (Minix service)
PROG=proj

.PATH: ${.CURDIR}/assets/xpm
.PATH: ${.CURDIR}/controller
.PATH: ${.CURDIR}/model
.PATH: ${.CURDIR}/model/level
.PATH: ${.CURDIR}/model/menu
.PATH: ${.CURDIR}/model/utils

.PATH: ${.CURDIR}/drivers
.PATH: ${.CURDIR}/drivers/keyboard
.PATH: ${.CURDIR}/drivers/mouse
.PATH: ${.CURDIR}/drivers/timer
.PATH: ${.CURDIR}/drivers/video

# source code files to be compiled
SRCS = main.c utils.c timer.c keyboard.c mouse.c gpu.c sprite.c animated_sprite.c kbc.c game.c cursor.c level.c player.c mob.c maze.c button.c main_menu.c game_over.c victory.c bullet.c

# additional compilation flags
# "-Wall -Wextra -Werror -I . -std=c11 -Wno-unused-parameter" are already set
CFLAGS += -pedantic
CFLAGS += -ferror-limit=9999

# list of library dependencies (for Lab 2, only LCF library)
DPADD += ${LIBLCF}
LDADD += -llcf

# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk>