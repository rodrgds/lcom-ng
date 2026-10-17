PROG=proj

SRCS = proj.c keyboard.c video_gr.c mouse.c game.c timer.c xpmHandler.c RTC.c highscores.c

# __LCOM_OPTIMIZED__ disables most of the LCF's overhead (and functionality!)
CPPFLAGS += -pedantic -D __LCOM_OPTIMIZED__

DPADD += ${LIBLCF}
LDADD += -llcf

.include <minix.lcom.mk>
