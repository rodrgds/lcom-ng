# name of the program (Minix service)
PROG=proj

.PATH: ${.CURDIR}/game
.PATH: ${.CURDIR}/game/main_game
.PATH: ${.CURDIR}/game/menu
.PATH: ${.CURDIR}/game/card
.PATH: ${.CURDIR}/game/record
.PATH: ${.CURDIR}/game/game_utils
.PATH: ${.CURDIR}/devices/
.PATH: ${.CURDIR}/devices/timer
.PATH: ${.CURDIR}/devices/keyboard
.PATH: ${.CURDIR}/devices/mouse
.PATH: ${.CURDIR}/devices/video_card
.PATH: ${.CURDIR}/devices/rtc
.PATH: ${.CURDIR}/images/sprite

# source code files to be compiled
SRCS = main.c timer.c kbc.c mouse.c graphics.c rtc.c utils.c game.c menu.c main_game.c sprite.c card.c record.c game_utils.c

# additional compilation flags
# "-Wall -Wextra -Werror -I . -std=c11 -Wno-unused-parameter" are already set
CFLAGS += -pedantic -D __LCOM_OPTIMIZED__

# list of library dependencies
DPADD += ${LIBLCF}
LDADD += -llcf

# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk>
