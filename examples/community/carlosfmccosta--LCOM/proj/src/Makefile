# name of the program (Minix service)
PROG=proj

# files to compile
.PATH: ${.CURDIR}/controller/
.PATH: ${.CURDIR}/controller/KBC/
.PATH: ${.CURDIR}/controller/KBC/keyboard
.PATH: ${.CURDIR}/controller/KBC/mouse
.PATH: ${.CURDIR}/controller/timer
.PATH: ${.CURDIR}/controller/video_gr
.PATH: ${.CURDIR}/model/
.PATH: ${.CURDIR}/view/

# source code files to be compiled
SRCS = mouse.c utils.c video_gr.c keyboard.c timer.c sprite.c main.c player.c queue.c event.c draw_timer.c bullet.c enemy.c health_bar.c wall.c text.c menu.c game.c leaderboard.c write_name.c leaderboard_page.c

# additional compilation flags
# "-Wall -Wextra -Werror -I . -std=c11 -Wno-unused-parameter" are already set
CFLAGS += -pedantic

# list of library dependencies (for Lab 2, only LCF library)
DPADD += ${LIBLCF}
LDADD += -llcf

# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk>
