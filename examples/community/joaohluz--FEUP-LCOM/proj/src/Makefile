PROG=proj

SRCS = proj.c game.c graphics.c mouse.c keyboard.c timer.c menu.c file.c RTC.c

CPPFLAGS += -pedantic #-D __LCOM_OPTIMIZED__

DPADD += ${LIBLCF}
LDADD += -llcf

.include <minix.lcom.mk>
