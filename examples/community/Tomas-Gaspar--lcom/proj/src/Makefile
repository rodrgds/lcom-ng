# name of the program (Minix service)
PROG=proj

.PATH: ${.CURDIR}/controllers/utils
.PATH: ${.CURDIR}/controllers/keyboard_mouse/
.PATH: ${.CURDIR}/controllers/rtc/
.PATH: ${.CURDIR}/controllers/serial_port/
.PATH: ${.CURDIR}/controllers/timer/
.PATH: ${.CURDIR}/controllers/video/
.PATH: ${.CURDIR}/graphics/
.PATH: ${.CURDIR}/game/

# source code files to be compiled
SRCS = keyboard.c utils.c proj.c timer.c video_gr.c mouse.c rtc.c paint_window.c cursor.c game.c word_picker.c drawing_state_graphics.c queue.c serial_port.c menu_state.c

# additional compilation flags
# "-Wall -Wextra -Werror -I . -std=c11 -Wno-unused-parameter" are already set
CFLAGS += -pedantic

# list of library dependencies (for Lab 2, only LCF library)
DPADD += ${LIBLCF}
LDADD += -llcf

# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk>
