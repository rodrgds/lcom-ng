# name of the program (Minix service)
PROG = proj

# define include paths for source directories
.PATH: ${.CURDIR}
.PATH: ${.CURDIR}/drivers
.PATH: ${.CURDIR}/game/controllers
.PATH: ${.CURDIR}/game/viewers
.PATH: ${.CURDIR}/game
.PATH: ${.CURDIR}/utils

# source files to be compiled (no paths needed due to .PATH)
SRCS = \
    proj.c \
    utils.c \
    KBC.c \
    i8042.h \
    keyboardLogic.c \
    gameLogic.c \
    gameviewer.c \
    menuviewer.c \
    stateviewer.c \
    MC.c \
    videogr.c \
    timer.c\
    frames.c\
    structs.c

# additional compilation flags
CFLAGS += -g -pedantic -D_LCOM_OPTIMIZED_ #não sei o que é isto <<<<<<<<<<<<<<<<<<<<

# list of library dependencies
DPADD += ${LIBLCF}
LDADD += -llcf

.include <minix.lcom.mk>