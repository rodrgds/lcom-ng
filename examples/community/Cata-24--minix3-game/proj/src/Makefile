PROG=proj

.PATH: ${.CURDIR}/helper/
.PATH: ${.CURDIR}/drivers/
.PATH: ${.CURDIR}/xpm/
.PATH: ${.CURDIR}/states/

# source code files to be compiled  serial_pPOrt.Port.c
SRCS = 	main.c timer.c utils.c video.c keyboard.c mouse.c menu.c game.c

# ./controllers/keyboardMouse/KBC.c

# additional compilation flags
# "-Wall -Wextra -Werror -I . -std=c11 -Wno-unused-parameter" are already set
CFLAGS += -pedantic -D_LCOM_OPTIMIZED_

# list of library dependencies
DPADD += ${LIBLCF}
LDADD += -llcf -lxpm -llm

# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk>