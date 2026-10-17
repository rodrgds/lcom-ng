# name of the program (Minix service)
PROG = proj 

.PATH: ${.CURDIR}/devices
.PATH: ${.CURDIR}/models
.PATH: ${.CURDIR}/view
.PATH: ${.CURDIR}/game

# source code files to be compiled
SRCS = main.c menu.c keyboard.c video.c player.c canvas.c utils.c timer.c bullet.c helicopter.c ih.c platforms.c rtc.c

# additional compilation flags
# "-Wall -Wextra -Werror -I . -std=c11 -Wno-unused-parameter" are already set
CFLAGS += -pedantic -D_LCOM_OPTIMIZED_

# list of library dependencies
DPADD += ${LIBLCF}
LDADD += -llcf

# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk>
