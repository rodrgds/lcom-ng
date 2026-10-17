PROG=proj

SRCS = proj.c kbc.c timer.c vbe.c utils.c sprite.c mouse.c bullet.c collisions.c enemy.c game.c rtc.c queue.c serial_port.c
#  __LCOM_OPTIMIZED__ disables most of the LCF's overhead (and functionality!)
CPPFLAGS += -pedantic -D __LCOM_OPTIMIZED__

DPADD += ${LIBLCF}
LDADD += -llcf

.include <minix.lcom.mk>
