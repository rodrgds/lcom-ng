PROG=proj 

.PATH: ${.CURDIR}/game_state/
.PATH: ${.CURDIR}/load/
.PATH: ${.CURDIR}/macros/
.PATH: ${.CURDIR}/devices/
.PATH: ${.CURDIR}/Images/
.PATH: ${.CURDIR}/Entities/
.PATH: ${.CURDIR}/Items/


SRCS = proj.c game.c graphics.c keyboard.c menu.c view.c timer_proj.c level.c mouse.c pause.c gameover.c spawner.c rtc.c player.c enemies_lvl.c workers.c wolf.c package.c inventory.c pizza.c ammo.c pistol.c boots.c backpack.c bone.c grenade.c watch.c bullet.c teleport.c explosion.c


CFLAGS += -pedantic -D_LCOM_OPTIMIZED_ 


DPADD += ${LIBLCF}
LDADD += -llcf -lxpm -llm

.include <minix.lcom.mk>
