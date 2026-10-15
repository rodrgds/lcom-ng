# name of the program (Minix service)
PROG=proj

# source code files to be compiled
SRCS = proj.c graphics.c menu.c game.c snake.c timer.c keyboard.c utils.c rtc.c header.c gameOver.c mouse.c instructions.c apple.c objects.c

# additional compilation flags
# "-Wall -Wextra -Werror -I . -std=c11 -Wno-unused-parameter" are already set
CFLAGS += -pedantic -D__LCOM_OPTIMIZED__
# list of library dependencies (for Lab 5, only LCF library)
DPADD += ${LIBLCF}
LDADD += -llcf

# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk>
