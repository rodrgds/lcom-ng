# name of the program (Minix service)
PROG=proj

.PATH: ${.CURDIR}
.PATH: ${.CURDIR}/controllers/keyboard
.PATH: ${.CURDIR}/controllers/mouse
.PATH: ${.CURDIR}/controllers/timer
.PATH: ${.CURDIR}/controllers/rtc
.PATH: ${.CURDIR}/controllers/video_graphics
.PATH: ${.CURDIR}/controllers/sprites
.PATH: ${.CURDIR}/controllers/serialport
.PATH: ${.CURDIR}/model
.PATH: ${.CURDIR}/view
.PATH: ${.CURDIR}/handler
.PATH: ${.CURDIR}/menus

# source code files to be compiled
SRCS =  main.c graphics.c sprite.c kbd.c mouse.c timer.c rtc.c mainController.c boats.c game.c assetDraw.c gameLocalController.c gameLocalDraw.c helpController.c helpDraw.c menuController.c menuDraw.c placingController.c placingDraw.c serialport.c gameRemoteDraw.c gameRemoteController.c

# additional compilation flags
# "-Wall -Wextra -Werror -I . -std=c11 -Wno-unused-parameter" are already set
CPPFLAGS += -pedantic -DLAB3

# list of library dependencies (for Lab 2, only LCF library)
DPADD += ${LIBLCF}
LDADD += -llcf

# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk>
