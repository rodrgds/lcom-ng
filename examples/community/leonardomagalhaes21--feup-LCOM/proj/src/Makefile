PROG=proj

.PATH: ${.CURDIR}/devices/timer
.PATH: ${.CURDIR}/devices
.PATH: ${.CURDIR}/devices/graphics
.PATH: ${.CURDIR}/devices/keyboard
.PATH: ${.CURDIR}/devices/mouse
.PATH: ${.CURDIR}/devices/rtc

.PATH: ${.CURDIR}/controllers/serialPort
.PATH: ${.CURDIR}/game
.PATH: ${.CURDIR}/game/menu
.PATH: ${.CURDIR}/game/sprite
.PATH: ${.CURDIR}/game/classes
.PATH: ${.CURDIR}/game/logic
.PATH: ${.CURDIR}/game/classes
.PATH: ${.CURDIR}/game/classes




SRCS = 	main.c timer.c utils.c graphics.c keyboard.c mouse.c rtc.c sprite.c player.c mouse_cursor.c menu.c game_logic.c enemy.c bullet.c bullet_node.c save_score.c enemy_bullet.c

# ./controllers/keyboardMouse/KBC.c

# additional compilation flags
# "-Wall -Wextra -Werror -I . -std=c11 -Wno-unused-parameter" are already set
CFLAGS += -pedantic -D_LCOM_OPTIMIZED_

DPADD += ${LIBLCF}
LDADD += -llcf -lxpm -llm

.include <minix.lcom.mk>


