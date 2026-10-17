PROG=proj

#SRCS = proj.c utils.c videocard.c timer.c keyboard.c rtc.c mouse.c bricks.c ball.c background.c animate_sprite.c sprite.c game.c 

# __LCOM_OPTIMIZED__ disables most of the LCF's overhead (and functionality!)
CPPFLAGS += -pedantic -D __LCOM_OPTIMIZED__ 

DPADD += ${LIBLCF}
LDADD += -llcf

.include <minix.lcom.mk>
