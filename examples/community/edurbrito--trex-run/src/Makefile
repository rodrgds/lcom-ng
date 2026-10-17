PROG=proj

SRCS = proj.c utils.c timer.c kbd.c mouse.c vdh.c bitmap.c rtc.c main_menu.c single_player.c score.c

# __LCOM_OPTIMIZED__ disables most of the LCF's overhead (and functionality!)
CPPFLAGS += -pedantic #-D __LCOM_OPTIMIZED__

DPADD += ${LIBLCF}
LDADD += -llcf

.include <minix.lcom.mk>
