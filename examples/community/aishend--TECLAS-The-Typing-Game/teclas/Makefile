PROG = proj


SRCS = main.c state.c timer.c utils.c keyboard.c mouse.c video_gr.c video.c cursor.c sprite.c font.c scancode.c dictionary.c words.c ui.c fast_teclas_state.c fast_teclas_end_state.c results.c config.c config_state.c config_state_survival.c start_state.c main_menu_state.c game_over_state.c survival.c dino.c leaderboard_state.c

.PATH: ${.CURDIR}/src ${.CURDIR}/src/states ../lab2 ../lab3 ../lab4 ../lab5

CFLAGS += -I./src -I./src/states -I../lab1 -I../lab2 -I../lab3 -I../lab4 -I../lab5

DPADD += ${LIBLCF}

LDADD += -llcf -L../lab1 -lrtc

.include <minix.lcom.mk>
