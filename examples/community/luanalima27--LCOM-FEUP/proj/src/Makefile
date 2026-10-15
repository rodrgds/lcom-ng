# name of the program (Minix service)
PROG=proj

# source code files to be compiled
SRCS = proj.c video.c keyboards.c utils.c timer.c mouse.c player.c pixmapproj.h buttons.c cursor.c alien.c sprites.c projectiles.c letters.c score.c rtc.c bg.c

# additional compilation flags
# "-Wall -Wextra -Werror -I . -std=c11 -Wno-unused-parameter" are already set
CFLAGS += -pedantic -Wno-error

# list of library dependencies (for Lab 2, only LCF library)
DPADD += ${LIBLCF}
LDADD += -llcf

# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk>
