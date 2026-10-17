# name of the program (Minix service)
PROG=proj

# source code files to be compiled
SRCS = proj.c utils/utils.c devices/timer/timer.c devices/kbc/keyboard/keyboard.c devices/kbc/mouse/mouse.c devices/graphics/graphics.c model/pacman_model/pacman.c model/ghosts_model/ghost.c model/coins_model/coin.c controller/game_controller.c controller/timer_controller/timer_controller.c controller/keyboard_controller/keyboard_controller.c controller/mouse_controller/mouse_controller.c controller/pacman_movement_controller/pacman_movement_controller.c controller/collisions_controller/collisions_controller.c controller/ghosts_movement_controller/ghosts_movement_controller.c view/initialize_pixmaps.c view/maze_view/maze_view.c view/pacman_view/pacman_view.c view/ghosts_view/ghosts_view.c view/coins_view/coins_view.c view/timer_view/timer_view.c view/cursor_view/cursor_view.c view/menu_view/menu_view.c view/score_view/score_view.c
# additional compilation flags
# "-Wall -Wextra -Werror -I . -std=c11 -Wno-unused-parameter" are already set
CFLAGS += -pedantic

# list of library dependencies (for Lab 2, only LCF library)
DPADD += ${LIBLCF}
LDADD += -llcf

devices/timer/timer.o: devices/timer/timer.c devices/timer/timer.h
	cc $(CFLAGS) -o devices/timer/timer.o -c devices/timer/timer.c
devices/kbc/keyboard/keyboard.o: devices/kbc/keyboard/keyboard.c devices/kbc/keyboard/keyboard.h
	cc $(CFLAGS) -o devices/kbc/keyboard/keyboard.o -c devices/kbc/keyboard/keyboard.c
devices/kbc/mouse/mouse.o: devices/kbc/mouse/mouse.c devices/kbc/mouse/mouse.h
	cc $(CFLAGS) -o devices/kbc/mouse/mouse.o -c devices/kbc/mouse/mouse.c	
devices/graphics/graphics.o: devices/graphics/graphics.c devices/graphics/graphics.h
	cc $(CFLAGS) -o devices/graphics/graphics.o -c devices/graphics/graphics.c	

utils/utils.o: utils/utils.c utils/utils.h
	cc $(CFLAGS) -o utils/utils.o -c utils/utils.c

model/pacman_model/pacman.o: model/pacman_model/pacman.c model/pacman_model/pacman.h
	cc $(CFLAGS) -o model/pacman_model/pacman.o -c model/pacman_model/pacman.c
model/ghosts_model/ghost.o: model/ghosts_model/ghost.c model/ghosts_model/ghost.h
	cc $(CFLAGS) -o model/ghosts_model/ghost.o -c model/ghosts_model/ghost.c
model/coins_model/coin.o: model/coins_model/coin.c model/coins_model/coin.h
	cc $(CFLAGS) -o model/coins_model/coin.o -c model/coins_model/coin.c

controller/timer_controller/timer_controller.o: controller/timer_controller/timer_controller.c controller/timer_controller/timer_controller.h
	cc $(CFLAGS) -o controller/timer_controller/timer_controller.o -c controller/timer_controller/timer_controller.c
controller/keyboard_controller/keyboard_controller.o: controller/keyboard_controller/keyboard_controller.c controller/keyboard_controller/keyboard_controller.h
	cc $(CFLAGS) -o controller/keyboard_controller/keyboard_controller.o -c controller/keyboard_controller/keyboard_controller.c
controller/mouse_controller/mouse_controller.o: controller/mouse_controller/mouse_controller.c controller/mouse_controller/mouse_controller.h
	cc $(CFLAGS) -o controller/mouse_controller/mouse_controller.o -c controller/mouse_controller/mouse_controller.c
controller/game_controller.o: controller/game_controller.c controller/game_controller.h
	cc $(CFLAGS) -o controller/game_controller.o -c controller/game_controller.c
controller/pacman_movement_controller/pacman_movement_controller.o: controller/pacman_movement_controller/pacman_movement_controller.c controller/pacman_movement_controller/pacman_movement_controller.h
	cc $(CFLAGS) -o controller/pacman_movement_controller/pacman_movement_controller.o -c controller/pacman_movement_controller/pacman_movement_controller.c
controller/collisions_controller/collisions_controller.o: controller/collisions_controller/collisions_controller.c controller/collisions_controller/collisions-controller.h
	cc $(CFLAGS) -o controller/collisions_controller/collisions_controller.o -c controller/collisions_controller/collisions_controller.c
controller/ghosts_movement_controller/ghosts_movement_controller.o: controller/ghosts_movement_controller/ghosts_movement_controller.c controller/ghosts_movement_controller/ghosts_movement_controller.h
	cc $(CFLAGS) -o controller/ghosts_movement_controller/ghosts_movement_controller.o -c controller/ghosts_movement_controller/ghosts_movement_controller.c

view/initialize_pixmaps.o: view/initialize_pixmaps.c view/initialize_pixmaps.h
	cc $(CFLAGS) -o view/initialize_pixmaps.o -c view/initialize_pixmaps.c
view/maze_view/maze_view.o: view/maze_view/maze_view.c view/maze_view/maze_view.h
	cc $(CFLAGS) -o view/maze_view/maze_view.o -c view/maze_view/maze_view.c
view/pacman_view/pacman_view.o: view/pacman_view/pacman_view.c view/pacman_view/pacman_view.h
	cc $(CFLAGS) -o view/pacman_view/pacman_view.o -c view/pacman_view/pacman_view.c
view/ghosts_view/ghosts_view.o: view/ghosts_view/ghosts_view.c view/ghosts_view/ghosts_view.h
	cc $(CFLAGS) -o view/ghosts_view/ghosts_view.o -c view/ghosts_view/ghosts_view.c
view/coins_view/coins_view.o: view/coins_view/coins_view.c view/coins_view/coins_view.h
	cc $(CFLAGS) -o view/coins_view/coins_view.o -c view/coins_view/coins_view.c

view/timer_view/timer_view.o: view/timer_view/timer_view.c view/timer_view/timer_view.h
	cc $(CFLAGS) -o view/timer_view/timer_view.o -c view/timer_view/timer_view.c
view/cursor_view/cursor_view.o: view/cursor_view/cursor_view.c view/cursor_view/cursor_view.h
	cc $(CFLAGS) -o view/cursor_view/cursor_view.o -c view/cursor_view/cursor_view.c
view/menu_view/menu_view.o: view/menu_view/menu_view.c view/menu_view/menu_view.h
	cc $(CFLAGS) -o view/menu_view/menu_view.o -c view/menu_view/menu_view.c
view/score_view/score_view.o: view/score_view/score_view.c view/score_view/score_view.h
	cc $(CFLAGS) -o view/score_view/score_view.o -c view/score_view/score_view.c

# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk>
