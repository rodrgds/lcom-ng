# name of the program (Minix service)
PROG=proj

# source code files to be compiled
.PATH: ${.CURDIR}/Lists/
.PATH: ${.CURDIR}/Sprites/
.PATH: ${.CURDIR}/Drivers/
SRCS = proj.c GraphicCard.c keyboard.c timer.c mouse.c RTC.c utils.c LaserList.c EnemyList.c AnimationList.c Levels.c xpms.c Handlers.c Menu.c Missile.c

# additional compilation flags
# "-Wall -Wextra -Werror -I . -std=c11 -Wno-unused-parameter" are already set
CPPFLAGS += -pedantic -DLAB5 -D __LCOM_OPTIMIZED__

# list of library dependencies (for Lab 2, only LCF library)
DPADD += ${LIBLCF}
LDADD += -llcf -lxpm -llm

# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk>
