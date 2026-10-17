PROG=proj

.PATH: ${.CURDIR}/devices/
.PATH: ${.CURDIR}/devices/keyboard
.PATH: ${.CURDIR}/devices/mouse
.PATH: ${.CURDIR}/devices/timer
.PATH: ${.CURDIR}/devices/video
.PATH: ${.CURDIR}/devices/rtc
.PATH: ${.CURDIR}/devices/sp
.PATH: ${.CURDIR}/sprites/
.PATH: ${.CURDIR}/game/

SRCS = 	main.c timer.c utils.c graphics.c KBC.c keyboard.c mouse.c sprite.c handle_int.c draw.c rtc.c serial_port.c elements.c sprite_management.c game.c

CFLAGS += -pedantic -D_LCOM_OPTIMIZED_ -O2

DPADD += ${LIBLCF}
LDADD += -Wall -llcf -lxpm -llm
.include <minix.lcom.mk>
