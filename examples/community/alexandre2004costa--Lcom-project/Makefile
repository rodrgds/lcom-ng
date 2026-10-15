# name of the program (Minix service)
PROG=proj

.PATH: ${.CURDIR}/devices
.PATH: ${.CURDIR}/classes


# source code files to be compiled
SRCS = proj.c utils.c graphic.c timer.c keyboard.c kbc.c mouse.c Sprite.c controller.c Object.c model.c Bomb.c Player.c Plane.c serialPort.c queue.c rtc.c Score.c


# additional compilation flags
# "-Wall -Wextra -Werror -I . -std=c11 -Wno-unused-parameter" are already set
CFLAGS += -pedantic

# list of library dependencies (for Lab 2, only LCF library)
DPADD += ${LIBLCF}
LDADD += -llcf

# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk>
