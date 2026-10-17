# name of the program (Minix service)
PROG=proj

SRCS = main.c \
			 utils.c \
			 vector/vector.c \
			 state/state.c \
			 scheduler/scheduler.c \
			 queue/queue.c \
			 menu/controller/menu.c \
			 menu/model/menu.c \
			 menu/view/menu.c \
			 menu/state.c \
			 graphics/graphics.c \
			 game/controller/game.c \
			 game/controller/local_player.c \
			 game/model/game.c \
			 game/model/local_player.c \
			 game/view/game.c \
			 game/view/local_player.c \
			 game/state.c \
			 game/wordlist.c \
			 events/events.c \
			 devices/mouse.c \
			 devices/timer.c \
			 devices/kbc.c \
			 devices/keyboard.c \
			 devices/vbe.c
			#  devices/serial.c
			#  protocol/protocol.c \
			#  multiplayer/controller/multiplayer.c \
			#  multiplayer/controller/remote_player.c \
			#  multiplayer/state.c \
			#  multiplayer/model/multiplayer.c \
			#  multiplayer/model/remote_player.c \
			#  multiplayer/view/multiplayer.c \
			#  multiplayer/view/remote_player.c \

# additional compilation flags
# "-Wall -Wextra -Werror -I . -std=c11 -Wno-unused-parameter" are already set
CFLAGS += -pedantic
CFLAGS += -I include/

DPADD += ${LIBLCF}
LDADD += -llcf

.ifmake optimized
CFLAGS += -D__LCOM_OPTIMIZED__
CFLAGS += -O3
.endif

.ifmake lima
CFLAGS += -DLIMA
.endif


.PHONY: optimized
optimized:
	CFLAGS="${CFLAGS}" ${MAKE} clean all

.for SRC in ${SRCS}

${SRC:R}.o: ${SRC:R}.c
	echo "       compile $?"
	${CC} ${CFLAGS} -o $@ -c $?

.endfor

.PHONY: lima
lima:
	CFLAGS="${CFLAGS}" ${MAKE} clean all

# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk>
