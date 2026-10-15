PROG = lab3
SRCS = lab3.c

LIB_NAME = libkeyboard.a
LIB_SRCS = keyboard.c
LIB_OBJS = ${LIB_SRCS:.c=.o}

DPADD += ${LIBLCF} ${LIB_NAME}
LDADD += -llcf -L../lab2 -ltimer -L.
LDADD += -lkeyboard

CLEANFILES += ${LIB_NAME} ${LIB_OBJS}

all: lib
lib: ${LIB_OBJS}
	ar rcs ${LIB_NAME} ${LIB_OBJS}

.include <minix.lcom.mk>