PROG=proj

SRCS = proj.c words.c keyboard.c timer.c video_card.c utils.c scancodes.c sprite.c mouse.c Leaderboard.c game.c RTC.c

# __LCOM_OPTIMIZED__ disables most of the LCF's overhead (and functionality!)
CPPFLAGS += -pedantic #-D __LCOM_OPTIMIZED__

DPADD += ${LIBLCF}
LDADD += -llcf

.include <minix.lcom.mk>
