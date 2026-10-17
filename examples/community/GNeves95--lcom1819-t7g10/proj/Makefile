PROG=proj

SRCS = proj.c kbd.c mouse.c ps2.c timer.c video_gr.c bmp.c rtc.c

CPPFLAGS += -pedantic #-D __LCOM_OPTIMIZED__

DPADD += ${LIBLCF}
LDADD += -llcf -llm

.include <minix.lcom.mk>
