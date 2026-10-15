PROG=lab4

SRCS = lab4.c

CFLAGS += -pedantic
CFLAGS += -I../lab2
CFLAGS += -I../lab3

DPADD += ${LIBLCF} ${LIB_NAME}
LDADD += -llcf -L. -lmouse -L../lab3 -lkeyboard -L../lab2 -ltimer

LIB_NAME = libmouse.a
LIB_SRCS = mouse.c
LIB_OBJS = ${LIB_SRCS:.c=.o}
CLEANFILES += ${LIB_NAME} ${LIB_OBJS}

all: lib

lib: ${LIB_OBJS}
	ar rcs ${LIB_NAME} ${LIB_OBJS}

.include <minix.lcom.mk>