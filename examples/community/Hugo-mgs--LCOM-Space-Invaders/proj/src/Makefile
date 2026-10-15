# name of the program (Minix service)
PROG=proj

.PATH: ${.CURDIR}/controllers
.PATH: ${.CURDIR}/controllers/kbc
.PATH: ${.CURDIR}/controllers/rtc
.PATH: ${.CURDIR}/controllers/timer
.PATH: ${.CURDIR}/controllers/video
.PATH: ${.CURDIR}/sprites
.PATH: ${.CURDIR}/model/cursor
.PATH: ${.CURDIR}/model
.PATH: ${.CURDIR}/model/player
.PATH: ${.CURDIR}/model/alien
.PATH: ${.CURDIR}/model/barrier
.PATH: ${.CURDIR}/model/projectiles
.PATH: ${.CURDIR}/model/entry
.PATH: ${.CURDIR}/model/hud

# source code files to be compiled
SRCS = main.c timer.c kbd.c mouse.c utils.c KBC.c video.c sprite.c cursor.c game.c player.c alien.c barrier.c projectile.c entry.c hud.c

# additional compilation flags
# "-Wall -Wextra -Werror -I . -std=c11 -Wno-unused-parameter" are already set
CPPFLAGS += -pedantic -D __LCOM_OPTIMIZED__

# list of library dependencies (for Lab 2, only LCF library)
DPADD += ${LIBLCF}
LDADD += -llcf

# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk>