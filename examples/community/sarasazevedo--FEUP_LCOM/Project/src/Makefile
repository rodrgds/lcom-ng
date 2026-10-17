# name of the program (Minix service)
PROG=proj

# folders
.PATH: ${.CURDIR}/devices
.PATH: ${.CURDIR}/gameElements
.PATH: ${.CURDIR}/sprites

# source code files to be compiled
SRCS = proj.c KBC.c keyboard.c mouse.c timer.c videocard.c game.c playerShip.c  fire.c menu.c projPixmap.h sprite.c enemy.c spaceship_explosion.c cursor.c RTC.c enemyFire.c enemyExplosion.c

# additional compilation flags
# "-Wall -Wextra -Werror -I . -std=c11 -Wno-unused-parameter" are already set
CFLAGS += -pedantic -D_LCOM_OPTIMIZED_ -O2 

# list of library dependencies
DPADD += ${LIBLCF}
LDADD += -llcf -lxpm -llm

# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk>
