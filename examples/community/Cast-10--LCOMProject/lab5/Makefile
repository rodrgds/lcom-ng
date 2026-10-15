PROG=lab5

SRCS = lab5.c video.c

CFLAGS += -pedantic
CFLAGS += -I../lab3
CFLAGS += -I../lab2

DPADD += ${LIBLCF}
LDADD += -llcf -L../lab3 -lkeyboard -L../lab2 -ltimer

LIB_NAME = libvideo.a
LIB_SRCS = video.c
LIB_OBJS = ${LIB_SRCS:.c=.o}

CLEANFILES += ${LIB_NAME} ${LIB_OBJS}

all: lib

lib: ${LIB_NAME}

${LIB_NAME}: ${LIB_OBJS}
	ar rcs ${LIB_NAME} ${LIB_OBJS}

.include <minix.lcom.mk>