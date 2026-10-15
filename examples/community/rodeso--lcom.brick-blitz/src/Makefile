# name of the program (Minix service)
PROG=proj

.PATH: ${.CURDIR}/dvcs/
.PATH: ${.CURDIR}/dvcs/graphics
.PATH: ${.CURDIR}/dvcs/keyboard
.PATH: ${.CURDIR}/dvcs/mouse
.PATH: ${.CURDIR}/dvcs/timer
.PATH: ${.CURDIR}/game/
.PATH: ${.CURDIR}/game/sprite/

# source code files to be compiled
SRCS = proj.c game.c graphics.c keyboard.c mouse.c timer.c objects.c utils.c sprite.c draw.c

# additional compilation flags
# "-Wall -Wextra -Werror -I . -std=c11 -Wno-unused-parameter" are already set
CFLAGS += -pedantic -D_LCOM_OPTIMIZED_ -O2

# list of library dependencies (for Lab 2, only LCF library)
DPADD += ${LIBLCF}
LDADD += -llcf

# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk>