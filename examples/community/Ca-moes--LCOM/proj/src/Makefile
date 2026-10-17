PROG=proj

SRCS = proj.c vc_macros.h vc.c vc.h i8042.h keyboard.h keyboard.c utils.h utils.c timer.c i8254.h game.h game.c xpm.h sentences.h sentences.c mouse.c mouse.h rtc.h rtc.c rtc_macros.h highscores.h highscores.c

# __LCOM_OPTIMIZED__ disables most of the LCF's overhead (and functionality!)
CPPFLAGS += -pedantic #-D __LCOM_OPTIMIZED__

DPADD += ${LIBLCF}
LDADD += -llcf

.include <minix.lcom.mk>
