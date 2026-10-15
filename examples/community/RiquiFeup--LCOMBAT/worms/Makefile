PROG=proj

.PATH: ${.CURDIR}/src
.PATH: ${.CURDIR}/../lab1
.PATH: ${.CURDIR}/../lab2
.PATH: ${.CURDIR}/../lab3
.PATH: ${.CURDIR}/../lab4
.PATH: ${.CURDIR}/../lab5
.PATH: ${.CURDIR}/src/game
.PATH: ${.CURDIR}/src/game/cursor
.PATH: ${.CURDIR}/src/game/physics
.PATH: ${.CURDIR}/src/game/worm
.PATH: ${.CURDIR}/src/game/projectile
.PATH: ${.CURDIR}/src/game/state
.PATH: ${.CURDIR}/src/game/terrain
.PATH: ${.CURDIR}/src/game/explosion
.PATH: ${.CURDIR}/src/game/water
.PATH: ${.CURDIR}/src/game/weapons/bazooka
.PATH: ${.CURDIR}/src/game/weapons/grenade
.PATH: ${.CURDIR}/src/game/weapons/utils
.PATH: ${.CURDIR}/src/game/weapons
.PATH: ${.CURDIR}/src/game/hud
.PATH: ${.CURDIR}/src/engine/event
.PATH: ${.CURDIR}/src/engine/graphics
.PATH: ${.CURDIR}/src/engine/input
.PATH: ${.CURDIR}/src/engine/physics
.PATH: ${.CURDIR}/src/assets
.PATH: ${.CURDIR}/src/assets/xpm
.PATH: ${.CURDIR}/src/menu
.PATH: ${.CURDIR}/src/engine/ui

SRCS = main.c \
       game.c \
       game_viewer.c \
       game_controller.c \
       physics_controller.c \
       state_machine.c \
       event_queue.c \
       hitbox.c \
       projectile.c \
       cursor.c \
       cursor_controller.c \
       cursor_viewer.c \
       bazooka.c \
       bazooka_viewer.c \
       bazooka_controller.c \
       grenade.c \
       grenade_viewer.c \
       grenade_controller.c \
       weapon_viewer_utils.c \
       weapon_physics_utils.c \
       aim_viewer.c \
       worm.c \
       worm_viewer.c \
       worm_controller.c \
       keyboard.c kbc.c \
       mouse.c \
       timer.c \
       rtc.c \
       graphics.c \
       renderer.c \
       sprite.c \
       animated_sprite.c \
       font.c \
       assets.c \
       utils.c \
       terrain.c \
       terrain_viewer.c \
       terrain_controller.c \
       explosion.c \
       explosion_viewer.c \
       explosion_controller.c \
       water.c \
       water_viewer.c \
       water_controller.c \
       input.c \
       menu.c \
       pause_menu.c \
       controls.c \
       game_over.c \
       splash_menu.c \
       ui_panel.c \
       ui_bar.c \
       ui_menu.c \
       hud.c \
       hud_controller.c

CFLAGS += -pedantic -D_LCOM_OPTIMIZED_ -DHAVE_LOGO_XPM -O2 -Wall -Wextra \
          -I${.CURDIR}/src \
          -I${.CURDIR}/../lab1 \
          -I${.CURDIR}/../lab2 \
          -I${.CURDIR}/../lab3 \
          -I${.CURDIR}/../lab4 \
          -I${.CURDIR}/../lab5 \
          -I${.CURDIR}/src/game \
          -I${.CURDIR}/src/game/cursor \
          -I${.CURDIR}/src/game/physics \
          -I${.CURDIR}/src/game/worm \
          -I${.CURDIR}/src/game/projectile \
          -I${.CURDIR}/src/game/state \
          -I${.CURDIR}/src/game/terrain \
          -I${.CURDIR}/src/game/explosion \
          -I${.CURDIR}/src/game/water \
          -I${.CURDIR}/src/game/weapons \
          -I${.CURDIR}/src/game/weapons/bazooka \
          -I${.CURDIR}/src/game/weapons/grenade \
          -I${.CURDIR}/src/game/weapons/utils \
          -I${.CURDIR}/src/engine/event \
          -I${.CURDIR}/src/engine/graphics \
          -I${.CURDIR}/src/engine/input \
          -I${.CURDIR}/src/engine/physics \
          -I${.CURDIR}/src/menu \
          -I${.CURDIR}/src/engine/ui \
          -I${.CURDIR}/src/game/hud \
          -I${.CURDIR}/src/assets \
          -I${.CURDIR}/src/assets/xpm

DPADD += ${LIBLCF}
LDADD += -llcf -lm -L.

.include <minix.lcom.mk>
