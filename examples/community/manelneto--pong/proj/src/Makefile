# name of the program (Minix service)
PROG=proj

.PATH: ${.CURDIR}/model
.PATH: ${.CURDIR}/view
.PATH: ${.CURDIR}/controller

# source code files to be compiled
SRCS = proj.c model.c menu.c levels.c game.c button.c cursor.c ball.c wall.c sprite.c view.c menu_view.c levels_view.c game_view.c button_view.c cursor_view.c ball_view.c wall_view.c sprite_view.c kbc.c keyboard.c mouse.c timer.c utils.c video.c

# additional compilation flags
# "-Wall -Wextra -Werror -I . -std=c11 -Wno-unused-parameter" are already set
CFLAGS += -pedantic -D __LCOM_OPTIMIZED__

# list of library dependencies
DPADD += ${LIBLCF}
LDADD += -llcf

# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk>
