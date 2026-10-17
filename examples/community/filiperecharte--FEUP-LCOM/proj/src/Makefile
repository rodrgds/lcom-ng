PROG=proj

SRCS = proj.c videocard.c mouse.c timer.c keyboard.c utils.c snake.c game.c fruit.c rtc.c leaderboard.c

# __LCOM_OPTIMIZED__ disables most of the LCF's overhead (and functionality!)
CPPFLAGS += -pedantic -D __LCOM_OPTIMIZED__


DPADD += ${LIBLCF}
LDADD += -llcf

.include <minix.lcom.mk>

