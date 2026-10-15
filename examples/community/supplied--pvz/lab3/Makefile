PROG=lab3
SRCS=lab3.c

CFLAGS += -pedantic -DLAB3 -I../lab2

DPADD += libkeyboard.a ${LIBLCF}
LDADD += -L. -lkeyboard -L../lab2 -ltimer -llcf

# Create library
LIB_NAME = libkeyboard.a
LIB_SRCS = keyboard.c utils.c
LIB_OBJS = ${LIB_SRCS:.c=.o}
CLEANFILES += ${LIB_NAME} ${LIB_OBJS}

all: lib
lib: ${LIB_OBJS}
	ar rcs ${LIB_NAME} ${LIB_OBJS}

.include <minix.lcom.mk>