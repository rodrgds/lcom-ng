# Name of the executable
PROG = proj

.PATH: ${.CURDIR}/controllers

# All source files (now in the same directory)
SRCS = main.c game.c deck.c\
       timer.c keyboard.c mouse.c graphics.c\
    	utils.c

# Compiler flags (enable warnings, include current dir for headers)
CFLAGS += -Icontrollers. -pedantic

# Link LCOM library (required for Minix)
DPADD += ${LIBLCF}
LDADD += -llcf

# Minix build system
.include <minix.lcom.mk>
