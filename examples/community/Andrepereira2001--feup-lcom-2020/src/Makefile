# name of the program (Minix service)
PROG=proj

# source code files to be compiled
SRCS = proj.c video_gr.c sprite.c game.c keyboard.c utils.c state_machine.c timer.c mouse.c rtc.c uart.c

# additional compilation flags
# "-Wall -Wextra -Werror -I . -std=c11 -Wno-unused-parameter" are already set
# __LCOM_OPTIMIZED__ disables most of the LCF's overhead (and functionality!)
CFLAGS += -pedantic #-D __LCOM_OPTIMIZED__

# list of library dependencies 
DPADD += ${LIBLCF}
LDADD += -llcf

# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk>
