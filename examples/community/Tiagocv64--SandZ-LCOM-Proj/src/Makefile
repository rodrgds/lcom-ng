PROG=proj

SRCS = proj.c Bitmap.c blood.c bullet.c button.c edit_text.c game.c leaderboard_menu.c life.c main_menu.c mouse_proj.c obstacle.c over_menu.c pause_menu.c pick_name_menu.c player.c program.c text.c UI_game.c wait_player_menu.c wait_winner_menu.c weapon.c zombie.c
.PATH: ${.CURDIR}/libs
SRCS += keyboard.c mouse.c rtc.c serial_port.c timer.c video.c

CPPFLAGS += -pedantic #-D __LCOM_OPTIMIZED__

DPADD += ${LIBLCF}
LDADD += -llcf

.include <minix.lcom.mk>
