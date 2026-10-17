# name of the program (Minix service)
PROG=proj

.PATH: ${.CURDIR}/src
.PATH: ${.CURDIR}/src/Devices
.PATH: ${.CURDIR}/src/Devices/graphics
.PATH: ${.CURDIR}/src/Devices/keyboard
.PATH: ${.CURDIR}/src/Devices/mouse
.PATH: ${.CURDIR}/src/Devices/timer
.PATH: ${.CURDIR}/src/Devices/Utilities


# source code files to be compiled
SRCS = main.c snake.c VBE.c KBC.c timer.c utils.c

# additional compilation flags
# "-Wall -Wextra -Werror -I . -std=c11 -Wno-unused-parameter" are already set
CFLAGS += -pedantic

# list of library dependencies (for Lab 2, only LCF library)
DPADD += ${LIBLCF}
LDADD += -llcf

# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk>
