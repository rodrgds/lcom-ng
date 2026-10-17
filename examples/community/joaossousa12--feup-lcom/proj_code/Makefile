PROG=proj

.PATH: ${.CURDIR}/devices/timer/
.PATH: ${.CURDIR}/devices/utils/
.PATH: ${.CURDIR}/devices/keyboard/
.PATH: ${.CURDIR}/devices/mouse/
.PATH: ${.CURDIR}/devices/video/
.PATH: ${.CURDIR}/game/
.PATH: ${.CURDIR}/sprites/



SRCS = 	main.c timer.c utils.c keyboard.c KBC.c mouse.c KBC_mouse.c video.c game.c instructions.c menu.c sprites.c

CFLAGS += -pedantic

DPADD += ${LIBLCF}
LDADD += -Wall -llcf -lxpm -llm
.include <minix.lcom.mk>