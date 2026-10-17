PROG=proj

SRCS = proj.c video.c sprite.c keyboard.c mainmenu.c mouse.c timer.c game.c rtc.c utils.c initiate.c highscores.c

# __LCOM_OPTIMIZED__ disables most of the LCF's overhead (and functionality!)
CPPFLAGS += -pedantic -D __LCOM_OPTIMIZED__

DPADD += ${LIBLCF}
LDADD += -llcf

.include <minix.lcom.mk>
