# name of the program (Minix service)
PROG=proj

# source code files to be compiled
SRCS = main.c utils.c graphics.c keyboard.c KBC.c game.c font8x8_basic.c cube_sprite.c progress_bar.c mouse.c

# additional compilation flags
# "-Wall -Wextra -Werror -I . -std=c11 -Wno-unused-parameter" are already set
CFLAGS += -pedantic -DLAB5

# list of library dependencies (for Lab 2, only LCF library)
DPADD += ${LIBLCF}
LDADD += -llcf -lxpm -llm

# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk>