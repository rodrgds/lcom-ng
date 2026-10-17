# name of the program (Minix service)
PROG=proj 

# Folders to compile
.PATH:${.CURDIR}/src 
# .PATH: ${.CURDIR}/serial

# source code files to be compiled
SRCS = main.c timer.c video.c utils.c keyboard.c mouse.c game.c

# additional compilation flags ("-Wall -Wextra -Werror -I . -std=c11 -Wno-unused-parameter" are already set)
CFLAGS += -pedantic
# CFLAGS += -D_LCOM_OPTIMIZED_ -O2       # Optimization flags

# list of library dependencies
DPADD += ${LIBLCF}
LDADD += -llcf -lxpm -llm

# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk>
