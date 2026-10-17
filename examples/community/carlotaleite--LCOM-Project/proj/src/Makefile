# name of the program (Minix service)
PROG=proj

.PATH: ${.CURDIR}/controller/
.PATH: ${.CURDIR}/model/
.PATH: ${.CURDIR}/view/

# source code files to be compiled
SRCS = main.c utils.c kbc.c gpu.c keyboard.c timer.c mouse.c game.c snake.c world.c sprite.c

# additional compilation flags
# "-Wall -Wextra -Werror -I . -std=c11 -Wno-unused-parameter" are already set
CFLAGS += -pedantic -DLAB5 -D_LCOM_OPTIMIZED_ -O2

# list of library dependencies (for Lab 2, only LCF library)
DPADD += ${LIBLCF}
LDADD += -llcf 

# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk>
