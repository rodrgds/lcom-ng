PROG=proj

.PATH: ${.CURDIR}/drivers/keyboard
.PATH: ${.CURDIR}/drivers/mouse
.PATH: ${.CURDIR}/drivers/timer
.PATH: ${.CURDIR}/drivers/video_card/
.PATH: ${.CURDIR}/drivers/rtc/
.PATH: ${.CURDIR}/drivers/serial_port/
.PATH: ${.CURDIR}/drivers/
.PATH: ${.CURDIR}/model/game/
.PATH: ${.CURDIR}/model/xpm/
.PATH: ${.CURDIR}/model/cursor/
.PATH: ${.CURDIR}/model/menu/
.PATH: ${.CURDIR}/model/name/
.PATH: ${.CURDIR}/model/game_over/
.PATH: ${.CURDIR}/view/
.PATH: ${.CURDIR}/controller/
.PATH: ${.CURDIR}/state/

SRCS = 	main.c timer.c mouse.c keyboard.c menu.c gpu.c game.c pieces.c board.c player.c viewer.c utils.c sprite.c cursor.c cursor_input.c game_controller.c menu_viewer.c name_viewer.c menu_controller.c name_controller.c state.c name_input.c game_viewer.c rtc.c sp.c sp_data_queue.c game_over.c game_over_viewer.c game_over_controller.c move.c

CFLAGS += -pedantic -D__LCOM_OPTIMIZED__

DPADD += ${LIBLCF}
LDADD += -Wall -llcf -lxpm -llm

.include <minix.lcom.mk>
