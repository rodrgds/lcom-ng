PROG = proj

.PATH: ${.CURDIR}/src
.PATH: ${.CURDIR}/src/utils
.PATH: ${.CURDIR}/src/controller/keyboard-mouse
.PATH: ${.CURDIR}/src/controller/rtc
.PATH: ${.CURDIR}/src/controller/timer
.PATH: ${.CURDIR}/src/controller/video
.PATH: ${.CURDIR}/src/model
.PATH: ${.CURDIR}/src/view
.PATH: ${.CURDIR}/src/game

SRCS = main.c bitwise.c utils.c kbc.c mouse.c rtc.c timer.c graphics.c sprite.c note.c renderer.c input.c menu.c font.c game.c assets_loader.c scoreboard.c

CFLAGS += -pedantic
CFLAGS += -Isrc
CFLAGS += -Isrc/controller/keyboard-mouse
CFLAGS += -Isrc/controller/rtc
CFLAGS += -Isrc/controller/timer
CFLAGS += -Isrc/controller/video

DPADD += ${LIBLCF}
LDADD += -llcf -ltimers

.include <minix.lcom.mk>
