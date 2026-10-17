PROG=proj

SRCS = proj.c timer.c video_gr.c Bitmap.c keyboard.c tetris.c menus.c mouse.c

# __LCOM_OPTIMIZED__ disables most of the LCF's overhead (and functionality!)
CPPFLAGS += -pedantic # -D __LCOM_OPTIMIZED__

DPADD += ${LIBLCF}
LDADD += -llcf

.include <minix.lcom.mk>
