PROG=proj

.PATH: ${.CURDIR}/controller
.PATH: ${.CURDIR}/controller/timer
.PATH: ${.CURDIR}/controller/keyboard
.PATH: ${.CURDIR}/controller/mouse
.PATH: ${.CURDIR}/controller/graphics
.PATH: ${.CURDIR}/controller/rtc
.PATH: ${.CURDIR}/controller/serialport
.PATH: ${.CURDIR}/model/
.PATH: ${.CURDIR}/view/
.PATH: ${.CURDIR}/game/

SRCS  = proj.c timer.c utils.c KBC.c keyboard.c mouse.c graphics.c sprite.c model.c view.c player.c game.c rtc.c

CFLAGS += -pedantic -D_LCOM_OPTIMIZED_

DPADD += ${LIBLCF}
LDADD += -llcf -lxpm -llm
.include <minix.lcom.mk>