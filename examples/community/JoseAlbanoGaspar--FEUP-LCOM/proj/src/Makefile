# name of the program (Minix service)
PROG=proj

# source code files to be compiled
SRCS = proj.c vbe.c keyboard.c utils.c timer.c mouse.c assist.c snake.c menu.c game.c rtc.c gameOver.c pause.c

# additional compilation flags
# "-Wall -Wextra -Werror -I . -std=c11 -Wno-unused-parameter" are already set
CPPFLAGS += -pedantic -DPROJ

# list of library dependencies (for Lab 2, only LCF library)
DPADD += ${LIBLCF}
LDADD += -llcf

# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk>
