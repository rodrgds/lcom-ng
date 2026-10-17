# name of the program (Minix service)
PROG=proj 

.PATH: ${.CURDIR}/src

# source code files to be compiled
SRCS = timer.c graphics.c main.c utils.c controller.c kbc.c keyboard.c mouse.c rtc.c sprite.c draw.c setup_game.c
IPATHS=-I./inc/sprite.h -I./inc/graphic.h -I./inc/i8254.h -I./inc/pixmap.h -I./inc/video.h -I./inc/controller.h -I./inc/i8042.h -I./inc/kbc.h -I./inc/keyboard.h -I./inc/data_structures.h -I./inc/mouse.h -I./inc/rtc.h -I./inc/draw.h -I./inc/setup_game.h

# additional compilation flags
# "-Wall -Wextra -Werror -I . -std=c11 -Wno-unused-parameter" are already set
CPPFLAGS += -pedantic ${IPATHS} -D LCOM_MACRO -D __LCOM_OPTIMIZED__ -Wall -Wextra -Wshadow -Wunreachable-code #-Weverything -Wno-padded -Wno-unused-macros

# list of library dependencies (for Lab 2, only LCF library)
DPADD += ${LIBLCF}
LDADD += -llcf

# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk>