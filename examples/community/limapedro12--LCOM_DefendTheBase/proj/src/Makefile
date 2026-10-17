# name of the program (Minix service)
PROG=proj

.PATH: ${.CURDIR}
.PATH: ${.CURDIR}/our_framework
.PATH: ${.CURDIR}/our_framework/device_handlers
.PATH: ${.CURDIR}/draw

# source code files to be compiled
SRCS = proj.c game.c framework_essencials.c utils.c timer.c kbd.c gpu.c mouse.c rtc.c draw.c drag.c bullet.c night_mode.c

# additional compilation flags
# "-Wall -Wextra -Werror -I . -std=c11 -Wno-unused-parameter" are already set
CFLAGS += -pedantic

# list of library dependencies (for Lab 2, only LCF library)
DPADD += ${LIBLCF}
LDADD += -llcf

# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk>
