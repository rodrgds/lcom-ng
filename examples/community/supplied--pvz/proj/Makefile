PROG=proj

SRCS = main.c loop.c constants.c state.c scene.c plant.c zombie.c bullet.c sun.c wave.c lawnmower.c world.c combat.c plant_system.c projectile_system.c plant_select.c render.c ui_render.c sprite_loader.c map_sprites.c sunflower_sprites.c peashooter_sprites.c repeater_sprites.c cabbage_sprites.c wall_nut_sprites.c potato_mine_sprites.c spikeweed_sprites.c reg_zombie_sprite.c reg_zombie_eating_sprite.c cone_zombie_sprite.c pole_zombie_sprite.c cursor_sprite.c bullet_sprite.c cabbage_projectile_sprite.c sun_sprite.c hud_sprite.c shovel_sprite.c lawnmower_sprite.c seed_packet_sprite.c seed_bank.c system.c rtc.c input.c board.c

CFLAGS += -pedantic -D __LCOM_OPTIMIZED__ -I./include -I../lab2 -I../lab3 -I../lab4 -I../lab5

DPADD += ../lab2/libtimer.a ../lab3/libkeyboard.a ../lab4/libmouse.a ../lab5/libvideo.a ${LIBLCF}
LDADD += -L../lab2 -ltimer -L../lab3 -lkeyboard -L../lab4 -lmouse -L../lab5 -lvideo -llcf

.PATH: src
.PATH: src/game
.PATH: src/entities
.PATH: src/render
.PATH: src/ui
.PATH: src/utils

.include <minix.lcom.mk>
