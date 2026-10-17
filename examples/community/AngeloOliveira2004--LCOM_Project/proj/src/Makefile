# name of the program (Minix service)
PROG=proj

.PATH: ${.CURDIR}/mvc/controller/graphics

.PATH: ${.CURDIR}/mvc/controller/keyboard

.PATH: ${.CURDIR}/mvc/controller/mouse

.PATH: ${.CURDIR}/mvc/controller/kbc

.PATH: ${.CURDIR}/mvc/controller/rtc

.PATH: ${.CURDIR}/mvc/controller

.PATH: ${.CURDIR}/mvc/controller/serial_port

.PATH: ${.CURDIR}/mvc/controller/timer

.PATH: ${.CURDIR}/mvc/model
.PATH: ${.CURDIR}/mvc/view

.PATH: ${.CURDIR}/graphics
.PATH: ${.CURDIR}/keyboard
.PATH: ${.CURDIR}/mouse
.PATH: ${.CURDIR}/timer
.PATH: ${.CURDIR}/utils 
 

# source code files to be compiled
SRCS = main.c utils.c keyboard.c kbc.c mouse.c rtc.c graphic.c view.c timer.c game.c controller.c
# additional compilation flags
# "-Wall -Wextra -Werror -I . -std=c11 -Wno-unused-parameter" are already set
CFLAGS += -pedantic

# list of library dependencies (for Lab 2, only LCF library)
DPADD += ${LIBLCF}
LDADD += -llcf

CPPFLAGS += -pedantic -DLAB3

# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk>