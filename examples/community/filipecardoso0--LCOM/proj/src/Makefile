# name of the program (Minix service)
PROG=proj

.PATH: ${.CURDIR}/drivers/graphic/src
.PATH: ${.CURDIR}/drivers/kbd/src
.PATH: ${.CURDIR}/drivers/mouse/src
.PATH: ${.CURDIR}/drivers/timer/src
.PATH: ${.CURDIR}/lib/model/src
.PATH: ${.CURDIR}/lib/controller/src
.PATH: ${.CURDIR}/lib/view/src
.PATH: ${.CURDIR}/lib/state/src
.PATH: ${.CURDIR}/lib/event/src
.PATH: ${.CURDIR}/utils/src

# source code files to be compiled
SRCS = proj.c graphic.c kbd.c mouse.c timer.c snake_model.c position.c sprite.c xpm.c utils.c menu_model.c board_model.c wall_model.c fruit_model.c menu_view.c board_view.c snake_view.c wall_view.c fruit_view.c fruit_controller.c snake_controller.c menu_controller.c board_controller.c state.c menu_state.c game_state.c event.c event_handler.c
IPATHS = -I./drivers/graphic/include -I./drivers/kbd/include -I./drivers/mouse/include -I./drivers/timer/include -I./lib/model/src -I./drivers/utils/src -I./lib/view/src -I.lib/controller/src -I.lib/state/src -I.lib/event/src -I./images

# additional compilation flags
# "-Wall -Wextra -Werror -I . -std=c11 -Wno-unused-parameter" are already set
CFLAGS += -pedantic ${IPATHS}

# list of library dependencies (for Lab 2, only LCF library)
DPADD += ${LIBLCF}
LDADD += -llcf

# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk>
