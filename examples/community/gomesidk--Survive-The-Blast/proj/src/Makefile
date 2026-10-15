PROG=proj

# Place the executable in the bin directory
BINDIR=${.CURDIR}/bin

SRC_DIRS = \
  controllers \
  controllers/keyboard_mouse \
  controllers/timer \
  controllers/graphics \
  controllers/serial_port \
  images \
  model/animSprite \
  model/sprite \
  model/bomb \
  model/player \
  model/wall \
  model/singleMode \
  model/singleMode/died_state \
  model/singleMode/game_state \
  model/multiMode \
  model/multiMode/connection_state \
  model/multiMode/game_state \
  model/multiMode/winner_state \
  model/spriteLoader \
  model/cursor \
  model/explosion \
  model/menu \
  model/bomb_options \
  model/state \
  model/key_pressed \
  model/score \
  model/button

# Add all source directories to .PATH and -I
.for dir in ${SRC_DIRS}
.PATH: ${.CURDIR}/${dir}
CFLAGS += -I${.CURDIR}/${dir}
.endfor

# List of source files (explicit for Minix make)
SRCS = main.c kbc.c kbd.c mouse.c timer.c utils.c graphics.c animatedSprite.c sprite.c bomb.c player.c wall.c singleMode.c spriteLoader.c cursor.c explosion.c menu.c bomb_options.c state.c key_pressed.c serial_port.c queue.c sm_died.c sm_game.c mm_connection.c mm_game.c mm_winner.c multiMode.c score.c button.c

CFLAGS += -pedantic -D_LCOM_OPTIMIZED_

DPADD += ${LIBLCF}
LDADD += -llcf -lxpm -llm

.include <minix.lcom.mk>