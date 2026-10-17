PROG=proj

.PATH: ${.CURDIR}/game/src
.PATH: ${.CURDIR}/libs/vbe/src
.PATH: ${.CURDIR}/libs/kbc/src
.PATH: ${.CURDIR}/libs/timer/src
.PATH: ${.CURDIR}/libs/utils/src

SRCS = proj.c grid.c kbc.c mouse.c sudoku.c timer.c utils.c vbe_functions.c event_handler.c

# __LCOM_OPTIMIZED__ disables most of the LCF's overhead (and functionality!)
CPPFLAGS += -pedantic -D __LCOM_OPTIMIZED__

DPADD += ${LIBLCF}
LDADD += -llcf

.include <minix.lcom.mk>
