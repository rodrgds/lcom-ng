# name of the program (Minix service)
PROG=proj

.PATH: ${.CURDIR}/controllers/
.PATH: ${.CURDIR}/controllers/keyboard
.PATH: ${.CURDIR}/controllers/timer
.PATH: ${.CURDIR}/controllers/video
.PATH: ${.CURDIR}/controllers/menu

# source code files to be compiled
SRCS = main.c utils.c graphics.c menu.c keyboard.c

# additional compilation flags
# "-Wall -Wextra -Werror -I . -std=c11 -Wno-unused-parameter" are already set
CFLAGS += -pedantic -DLAB5

# list of library dependencies (for Lab 2, only LCF library)
DPADD += ${LIBLCF}
LDADD += -llcf -lxpm -llm

# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk>