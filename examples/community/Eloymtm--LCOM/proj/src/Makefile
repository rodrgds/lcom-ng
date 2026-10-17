# name of the program (Minix service)
PROG=proj

.PATH: ${.CURDIR}/viewer
.PATH: ${.CURDIR}/labs
.PATH: ${.CURDIR}/model
.PATH: ${.CURDIR}/state
.PATH: ${.CURDIR}/controller

# source code files to be compiled
SRCS = proj.c keyboard.c video.c aux.c utils.c boardViewer.c game.c ship.c coordinate.c board.c state.h mouse.c timer.c menuViewer.c devicesController.c structGame.h instrViewer.c waitingViewer.c winViewer.c

# additional compilation flags
# "-Wall -Wextra -Werror -I . -std=c11 -Wno-unused-parameter" are already set
CFLAGS += -pedantic -D_LCOM_OPTIMIZED_ 

# list of library dependencies (for Lab 2, only LCF library)
DPADD += ${LIBLCF}
LDADD += -llcf

# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk>
