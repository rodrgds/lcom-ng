# name of the program (Minix service)
PROG=proj


.PATH: ${.CURDIR}/labs/
.PATH: ${.CURDIR}/model/ball/
.PATH: ${.CURDIR}/viewer/
.PATH: ${.CURDIR}/States/
.PATH: ${.CURDIR}/model/
.PATH: ${.CURDIR}/controllers/
.PATH: ${.CURDIR}/physics/
.PATH: ${.CURDIR}/physics/resolve/



# source code files to be compiled

SRCS = main.c graphics.c timer.c utils.c keyboard.c mouse.c kbc.c cueViewer.c lineViewer.c ball.c Game.c resources.c table.c mouseModel.c playingController.c utilities.c cue.c evolve.c events.c resolver.c simulate.c cushion.c pocket.c event.c menu.c menuController.c button.c menuViewer.c multiplayerController.c rtc.c serialPort.c queue.c player.c quartic.c


# additional compilation flags
# "-Wall -Wextra -Werror -I . -std=c11 -Wno-unused-parameter" are already set
# CFLAGS += -pedantic -Wshadow -Winit-self -Wstrict-prototypes -Wold-style-definition -Wmissing-declarations -Wdeclaration-after-statement

# __LCOM_OPTIMIZED__ disables most of the LCF's overhead (and functionality!)
CPPFLAGS += -pedantic -D__LCOM_OPTIMIZED__

# list of library dependencies (for Lab 2, only LCF library)
DPADD += ${LIBLCF}
LDADD += -llcf

# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk>