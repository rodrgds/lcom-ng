PROG=lab5
SRCS=lab5.c

CFLAGS += -pedantic -I../lab3

DPADD += libvideo.a ../lab3/libkeyboard.a ${LIBLCF}
LDADD += -L. -lvideo -L../lab3 -lkeyboard -llcf

# Create library
LIB_NAME = libvideo.a
LIB_SRCS = video.c
LIB_OBJS = ${LIB_SRCS:.c=.o}
CLEANFILES += ${LIB_NAME} ${LIB_OBJS}

all: lib
lib: ${LIB_OBJS}
	ar rcs ${LIB_NAME} ${LIB_OBJS}

.include <minix.lcom.mk>