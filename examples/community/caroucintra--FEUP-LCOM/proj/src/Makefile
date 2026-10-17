# name of the program (Minix service)
PROG=proj

.PATH: ${.CURDIR}
.PATH: ${.CURDIR}/utils

# source code files to be compiled
SRCS = proj.c video.c keyboard.c timer.c utils.c helpers.c game.c violetchi.c sprite.c rtc.c mouse.c

# additional compilation flags
# "-Wall -Wextra -Werror -I . -std=c11 -Wno-unused-parameter" are already set
CFLAGS += -pedantic -D__LCOM_OPTIMIZED__

# list of library dependencies (for Lab 2, only LCF library)
DPADD += ${LIBLCF}
LDADD += -llcf
CPPFLAGS += -D PROJ -Wall

# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk>

clean: rm -rf *.o *.txt
