PROG=proj

SRCS = proj.c  grafics.c keyboard.c timer.c mouse.c utils.c jogo.c numbers_letters.c jogo_funcs.c coins.c collisions.c enemies.c rtc.c files.c

# __LCOM_OPTIMIZED__ disables most of the LCF's overhead (and functionality!)
CPPFLAGS += -pedantic  -D __LCOM_OPTIMIZED__

DPADD += ${LIBLCF}
LDADD += -llcf

.include <minix.lcom.mk>
