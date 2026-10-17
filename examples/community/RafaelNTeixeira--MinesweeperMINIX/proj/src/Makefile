# name of the program (Minix service)
PROG=proj

.PATH: ${.CURDIR}
.PATH: ${.CURDIR}/devices
.PATH: ${.CURDIR}/devices/graphic
.PATH: ${.CURDIR}/devices/keyboard
.PATH: ${.CURDIR}/devices/mouse
.PATH: ${.CURDIR}/devices/timer
.PATH: ${.CURDIR}/devices/rtc
.PATH: ${.CURDIR}/game
.PATH: ${.CURDIR}/event_handler

# source code files to be compiled
SRCS = utils.c video.c kbc.c mouse.c statemachine.c timer.c rtc.c main.c int_manager.c game_state.c mouse_event_handler.c timer_event_handler.c display.c kbc_event_handler.c game_logic.c

# __LCOM_OPTIMIZED__ disables most of the LCF's overhead (and functionality!)
CFLAGS += -pedantic -D __LCOM_OPTIMIZED__
DPADD += ${LIBLCF}
LDADD += -llcf

.include <minix.lcom.mk>>
