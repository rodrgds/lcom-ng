PROG=lab4
SRCS=lab4.c

CFLAGS += -pedantic -I../lab2

DPADD += libmouse.a ${LIBLCF}
LDADD += -L. -lmouse -L../lab2 -ltimer -llcf

# Create library
LIB_NAME = libmouse.a
LIB_SRCS = mouse.c utils.c
LIB_OBJS = ${LIB_SRCS:.c=.o}
CLEANFILES += ${LIB_NAME} ${LIB_OBJS}

all: lib
lib: ${LIB_OBJS}
	ar rcs ${LIB_NAME} ${LIB_OBJS}

.include <minix.lcom.mk>