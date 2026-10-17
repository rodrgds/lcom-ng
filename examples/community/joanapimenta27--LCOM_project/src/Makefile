PROG=proj

.PATH: ${.CURDIR}/controller
.PATH: ${.CURDIR}/controller/kbc
.PATH: ${.CURDIR}/controller/timer
.PATH: ${.CURDIR}/controller/video
.PATH: ${.CURDIR}/controller/rtc
.PATH: ${.CURDIR}/sprites
.PATH: ${.CURDIR}/visual
.PATH: ${.CURDIR}/states
.PATH: ${.CURDIR}/game



SRCS = main.c KBC.c rtc.c keyboard.c mouse.c timer.c utils.c graphics.c sprites.c visual.c visual_menu.c state_menu.c state_game.c state_dead.c game_play.c state_win.c  scores.c



CFLAGS += -pedantic

DPADD += ${LIBLCF}
LDADD += -llcf

.include <minix.lcom.mk>