PROG=proj

SRCS = proj.c
SRCS += game.c
SRCS += graphics.c
SRCS += keyboard.c
SRCS += mouse.c
SRCS += timer.c
SRCS += rtc.c
SRCS += state.c
SRCS += sprite.c
SRCS += font.c
SRCS += ui.c
SRCS += ship.c
SRCS += asteroid.c
SRCS += bullet.c
SRCS += explosion.c
SRCS += collision.c
SRCS += utils.c
SRCS += input.c
SRCS += powerup.c
SRCS += boss.c
SRCS += background.c

.PATH: ${.CURDIR}/src
.PATH: ${.CURDIR}/src/devices
.PATH: ${.CURDIR}/src/game
.PATH: ${.CURDIR}/src/graphics
.PATH: ${.CURDIR}/src/input
.PATH: ${.CURDIR}/src/utils
.PATH: ${.CURDIR}/src/assets

CPPFLAGS += -I${.CURDIR}/src
CPPFLAGS += -I${.CURDIR}/src/devices
CPPFLAGS += -I${.CURDIR}/src/game
CPPFLAGS += -I${.CURDIR}/src/graphics
CPPFLAGS += -I${.CURDIR}/src/input
CPPFLAGS += -I${.CURDIR}/src/utils
CPPFLAGS += -I${.CURDIR}/src/assets

CFLAGS += -pedantic

DPADD += ${LIBLCF}
LDADD += -llcf

.include <minix.lcom.mk>