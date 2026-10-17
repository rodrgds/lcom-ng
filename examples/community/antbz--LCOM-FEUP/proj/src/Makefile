PROG=proj

.PATH: ${.CURDIR}/src 

SRCS += videocard.c keyboard.c utils.c timer.c fruit.c mouse.c gestures.c line.c rtc.c serialport.c screen.c game.c images.c

SRCS += proj.c

# __LCOM_OPTIMIZED__ disables most of the LCF's overhead (and functionality!)
CPPFLAGS += -pedantic -D __LCOM_OPTIMIZED__

DPADD += ${LIBLCF}
LDADD += -llcf

.include <minix.lcom.mk>
