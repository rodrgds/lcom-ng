# Name of the program
PROG=proj

.PATH: ${.CURDIR}/devices/
.PATH: ${.CURDIR}/views/
.PATH: ${.CURDIR}/controllers/
.PATH: ${.CURDIR}/utils/
.PATH: ${.CURDIR}/texts/
.PATH: ${.CURDIR}/model/

# Source code files
SRCS = main.c \
       video.c \
       timer.c \
       keyboard.c \
       mouse.c \
       views.c \
       interrupts.c \
       utils.c \
       allChars.c \
       virtual_keyboard_view.c \
       virtual_keyboard_controller.c \
       cursor.c \
       model.c \
       time_selector.c \
       phrases.c \
       typing_test_view.c \
       typing_test_controller.c \
       countdown_timer_view.c

# Additional compilation flags
CFLAGS += -pedantic

# Library dependencies
DPADD += ${LIBLCF}
LDADD += -llcf

# include LCOM's makefile
.include <minix.lcom.mk>