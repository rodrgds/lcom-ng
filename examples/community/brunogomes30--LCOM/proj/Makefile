PROG=proj

SRCS = sprite.c proj.c graphics.c kbd.c mouse.c timer.c player.c linkedEntities.c game.c enemy.c gun.c rtc.c letters.cpp

# __LCOM_OPTIMIZED__ disables most of the LCF's overhead (and functionality!)
CPPFLAGS += -pedantic  -D __LCOM_OPTIMIZED__

DPADD += ${LIBLCF}
LDADD += -llcf

.include <minix.lcom.mk>
