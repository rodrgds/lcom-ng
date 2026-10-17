# proj
PROG=proj

.PATH: ${.CURDIR}/devices/
.PATH: ${.CURDIR}/devices/timer
.PATH: ${.CURDIR}/devices/mouse
.PATH: ${.CURDIR}/devices/keyboard
.PATH: ${.CURDIR}/devices/video
.PATH: ${.CURDIR}/model/
.PATH: ${.CURDIR}/view/

# All .c project files. Have to be inside of the folders included above 
SRCS = main.c mouse.c keyboard.c timer.c video.c sprites.c model.c view.c utils.c

# Compilation flags
CFLAGS += -pedantic -D __LCOM_OPTIMIZED__

# Dependencies
DPADD += ${LIBLCF}
LDADD += -Wall -llcf -lxpm -llm
.include <minix.lcom.mk>
