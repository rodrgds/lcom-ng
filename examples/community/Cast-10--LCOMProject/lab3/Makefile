PROG=lab3

SRCS = lab3.c

CFLAGS += -pedantic
CFLAGS += -I../lab2

DPADD += ${LIBLCF} ${LIB_NAME}
LDADD += -llcf -L. -lkeyboard -L../lab2 -ltimer

LIB_NAME = libkeyboard.a
LIB_SRCS = keyboard.c
LIB_OBJS = ${LIB_SRCS:.c=.o}
CLEANFILES += ${LIB_NAME} ${LIB_OBJS}

all: lib
lib: ${LIB_OBJS}
	ar rcs ${LIB_NAME} ${LIB_OBJS}

.include <minix.lcom.mk>