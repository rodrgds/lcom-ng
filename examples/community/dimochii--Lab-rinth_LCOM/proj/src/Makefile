PROG=proj

.PATH: ${.CURDIR}
.PATH: ${.CURDIR}/game
.PATH: ${.CURDIR}/devices
.PATH: ${.CURDIR}/devices/tools

SRCS = proj.c timer.c keyboard.c mouse.c rtc.c utils.c KBC.c graphics.c game.c sprite.c logic.c queue.c serialPort.c

# __LCOM_OPTIMIZED__ disables most of the LCF's overhead (and functionality!)
CPPFLAGS += -pedantic -D __LCOM_OPTIMIZED__

DPADD += ${LIBLCF}
LDADD += -llcf

.include <minix.lcom.mk>