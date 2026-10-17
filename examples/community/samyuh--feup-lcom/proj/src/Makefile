PROG=proj

SRCS = proj.c graphics.c keyboard.c utils.c timer.c gamehandler.c mouse.c images.c game.c rtc.c track.c

# __LCOM_OPTIMIZED__ disables most of the LCF's overhead (and functionality!)
CPPFLAGS += -pedantic -D __LCOM_OPTIMIZED__

DPADD += ${LIBLCF}
LDADD += -llcf

.include <minix.lcom.mk>
