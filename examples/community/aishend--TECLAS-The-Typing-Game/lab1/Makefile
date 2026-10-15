PROG = lab1
SRCS = main.c

LIB_NAME = librtc.a
LIB_SRCS = rtc.c bitwise.c
LIB_OBJS = ${LIB_SRCS:.c=.o}

DPADD += ${LIB_NAME}
LDADD += -L.
LDADD += -lrtc

CLEANFILES += ${LIB_NAME} ${LIB_OBJS}

all: lib
lib: ${LIB_OBJS}
	ar rcs ${LIB_NAME} ${LIB_OBJS}

.include <minix.lcom.mk>