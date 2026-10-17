# name of the program (Minix service)
PROG=proj


.PATH: ${.CURDIR}/devices/
.PATH: ${.CURDIR}/devices/keyboard
.PATH: ${.CURDIR}/devices/mouse
.PATH: ${.CURDIR}/devices/timer
.PATH: ${.CURDIR}/devices/graphics
.PATH: ${.CURDIR}/devices/rtc
.PATH: ${.CURDIR}/devices/serialport
.PATH: ${.CURDIR}/visuals/
.PATH: ${.CURDIR}/draws/
# source code files to be compiled
SRCS = proj.c utils.c rtc.c serialport.c queue.c timer.c vbe.c keyboard.c mouse.c video.c sprite.c draw.c model.c

# additional compilation flags
# "-Wall -Wextra -Werror -I . -std=c11 -Wno-unused-parameter" are already set
CFLAGS += -pedantic

# list of library dependencies (for Lab 2, only LCF library)
DPADD += ${LIBLCF}
LDADD += -llcf

# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk>