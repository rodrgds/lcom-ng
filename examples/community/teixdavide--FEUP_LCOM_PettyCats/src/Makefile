# name of the program (Minix service)
PROG=proj

.PATH: ${.CURDIR}/drivers
.PATH: ${.CURDIR}/models
.PATH: ${.CURDIR}/viewer
.PATH: ${.CURDIR}/controller
.PATH: ${.CURDIR}/utils
.PATH: ${.CURDIR}/utils/aux_structs



# source code files to be compiled
SRCS = timer.c utils_drivers.c gpu.c rtclock.c keyboard.c mouse.c Game.c Sprite.c gameView.c controllerKeyboard.c main.c CatInfo.c Entity.c Room.c utils.c controllerMouse.c Position.c HurtBox.c PlayerInfo.c


# additional compilation flags
# "-Wall -Wextra -Werror -I . -std=c11 -Wno-unused-parameter" are already set
CFLAGS += -pedantic -D_LCOM_OPTIMIZED_ -O2

# list of library dependencies (for Lab 2, only LCF library)
DPADD += ${LIBLCF}
LDADD += -llcf

# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk>
