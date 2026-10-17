PROG=proj

.PATH: ${.CURDIR}/devices/serialPort
.PATH: ${.CURDIR}/devices/timer
.PATH: ${.CURDIR}/devices/video
.PATH: ${.CURDIR}/devices/kbc
.PATH: ${.CURDIR}/devices
.PATH: ${.CURDIR}/view
.PATH: ${.CURDIR}/controller
.PATH: ${.CURDIR}/model

SRCS = utils.c graphics.c KBC.c timer.c mouse.c keyboard.c sprite.c control.c view.c main.c player.c rtc.c table.c serial_port.c helper.c view_menu.c view_game.c control_game.c control_menu.c control_base.c

# additional compilation flags
# "-Wall -Wextra -Werror -I . -std=c11 -Wno-unused-parameter" are already set
CFLAGS += -pedantic

# list of library dependencies (for Lab 2, only LCF library)
DPADD += ${LIBLCF}
LDADD += -llcf -lxpm -llm

# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk>