# name of the program (Minix service)
PROG=proj

.PATH: ${.CURDIR}/controllers/
.PATH: ${.CURDIR}/controllers/keyboard
.PATH: ${.CURDIR}/controllers/mouse
.PATH: ${.CURDIR}/controllers/timer
.PATH: ${.CURDIR}/controllers/rtc
.PATH: ${.CURDIR}/controllers/video
.PATH: ${.CURDIR}/controllers/utils
.PATH: ${.CURDIR}/game/
.PATH: ${.CURDIR}/sprites/

# source code files to be compiled
SRCS = game.c proj.c timer.c keyboard.c kbc.c utils.c mouse.c gpu.c video_gr.c sprites.c list.c hitbox.c rtc.c

# additional compilation flags
# "-Wall -Wextra -Werror -I . -std=c11 -Wno-unused-parameter" are already set
CFLAGS += -pedantic -D __LCOM_OPTIMIZED__

# list of library dependencies (for Lab 2, only LCF library)
DPADD += ${LIBLCF}
LDADD += -llcf

# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk>
