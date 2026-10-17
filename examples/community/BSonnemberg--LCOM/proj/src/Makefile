# name of the program (Minix service)
PROG=proj

.PATH: ${.CURDIR}/io/
.PATH: ${.CURDIR}/io/timer
.PATH: ${.CURDIR}/io/keyboard
.PATH: ${.CURDIR}/io/mouse
.PATH: ${.CURDIR}/io/video
.PATH: ${.CURDIR}/io/rtc

.PATH: ${.CURDIR}/view

# source code files to be compiled
SRCS= proj.c controller.c view.c pool.c sprite_manager.c physics.c vector2.c vg.c vbe.c sprite.c keyboard.c mouse.c timer.c kbc.c utils.c

# additional compilation flags
# "-Wall -Wextra -Werror -I . -std=c11 -Wno-unused-parameter" are already set
CFLAGS += -pedantic

# list of library dependencies
DPADD += ${LIBLCF}
LDADD += -llcf

# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk>
