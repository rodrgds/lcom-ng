# name of the program (Minix service)
PROG = proj

.PATH: ${.CURDIR}/devices
.PATH: ${.CURDIR}/devices/graphics
.PATH: ${.CURDIR}/devices/kbd_mouse
.PATH: ${.CURDIR}/devices/rtc
.PATH: ${.CURDIR}/devices/timer
.PATH: ${.CURDIR}/xpm
.PATH: ${.CURDIR}/sprites
.PATH: ${.CURDIR}/utils

# source code files to be compiled
SRCS = proj.c graphics.c kbc.c keyboard.c mouse.c timer.c utils.c sprites.c font.c rtc.c

# additional compilation flags
# "-Wall -Wextra -Werror -I . -std=c11 -Wno-unused-parameter" are already set
CFLAGS += -pedantic

# list of library dependencies (for Lab 2, only LCF library)
DPADD += ${LIBLCF}
LDADD += -llcf

# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk>
