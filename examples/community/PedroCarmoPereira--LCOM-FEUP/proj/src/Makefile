PROG=proj

SRCS = proj.c mouse.c timer.c video.c kbd.c Entity.c Player.c Laser.c Enemy.c GameState.c Game.c Score.c rtc.c

CPPFLAGS += -pedantic #-D __LCOM_OPTIMIZED__

DPADD += ${LIBLCF}
LDADD += -llcf

.include <minix.lcom.mk>
