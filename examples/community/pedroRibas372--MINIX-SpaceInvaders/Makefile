.PATH: ${.CURDIR}/classes/invader/
.PATH: ${.CURDIR}/classes/shot/
.PATH: ${.CURDIR}/controllers/keyboard/
.PATH: ${.CURDIR}/controllers/timer/
.PATH: ${.CURDIR}/controllers/graphicsCard/
.PATH: ${.CURDIR}/sprites/
.PATH: ${.CURDIR}/game/
.PATH: ${.CURDIR}/menu/
.PATH: ${.CURDIR}/menulevel/
.PATH: ${.CURDIR}/view/header
.PATH: ${.CURDIR}/view/game
.PATH: ${.CURDIR}/view/menu


# Name of the program
PROG=proj

# Source code files with proper paths
SRCS = main.c keyboard.c utils.c graphics.c sprites.c game.c timer.c menu.c menulevel.c gameView.c menuView.c


# Extra compiler flags
CFLAGS+=-pedantic

# LCF linking
DPADD+=${LIBLCF}
LDADD+=-llcf

# Minix makefile
.include <minix.lcom.mk>
