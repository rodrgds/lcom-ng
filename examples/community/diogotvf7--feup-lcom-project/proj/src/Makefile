# name of the program (Minix service)
PROG=proj

# folders of the source code files
.PATH: ${.CURDIR}/controller/
.PATH: ${.CURDIR}/controller/keyboard
.PATH: ${.CURDIR}/controller/mouse
.PATH: ${.CURDIR}/controller/timer
.PATH: ${.CURDIR}/controller/graphics_card
.PATH: ${.CURDIR}/controller/uart
.PATH: ${.CURDIR}/controller/rtc
.PATH: ${.CURDIR}/model/
.PATH: ${.CURDIR}/view/

# source code files to be compiled
SRCS = proj.c utils.c keyboard.c kbc.c graphics_card.c timer.c mouse.c model.c view.c sprite.c base_frame.c rtc.c queue.c ser_port.c

# additional compilation flags
# "-Wall -Wextra -Werror -I . -std=c11 -Wno-unused-parameter" are already set
CFLAGS += -pedantic

# list of library dependencies (for Lab 4, only LCF library)
DPADD += ${LIBLCF}
LDADD += -llcf

# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk>
