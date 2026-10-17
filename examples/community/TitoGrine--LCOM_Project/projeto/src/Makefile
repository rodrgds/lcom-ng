PROG=proj

SRCS = proj.c timer.c keyboard.c mouse.c graphics.c sprite.c events.c queue.c rtc.c words.c player.c

CPPFLAGS += -pedantic -D __LCOM_OPTIMIZED__

DPADD += ${LIBLCF}
LDADD += -llcf

.include <minix.lcom.mk>
