PROG=proj

.PATH: ${.CURDIR}/controllers/
.PATH: ${.CURDIR}/controllers/keyboard_mouse
.PATH: ${.CURDIR}/controllers/timer
.PATH: ${.CURDIR}/controllers/video
.PATH: ${.CURDIR}/sprites
.PATH: ${.CURDIR}/images
.PATH: ${.CURDIR}/menu
.PATH: ${.CURDIR}/images/alphabet
.PATH: ${.CURDIR}/game_logic
.PATH: ${.CURDIR}/game_logic/board
.PATH: ${.CURDIR}/game_logic/card
.PATH: ${.CURDIR}/game_logic/game
.PATH: ${.CURDIR}/game_logic/player


SRCS = main.c utils.c kbc.c keyboard.c timer.c graphic.c sprites.c Card.c Board.c Game.c Player.c menu.c

CFLAGS += -pedantic -D_LCOM_OPTIMIZED_

DPADD += ${LIBLCF}
LDADD += -llcf -lxpm -llm

.include <minix.lcom.mk>