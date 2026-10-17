PROG = lab1 
SRCS = main.c rtc.c bitwise.c

CFLAGS += -pedantic

DPADD += ${LIBLCF}
LDADD += -llcf

# Library generation settings
LIB_NAME = librtc.a
LIB_SRCS = rtc.c bitwise.c
LIB_OBJS = ${LIB_SRCS:.c=.o}
CLEANFILES += ${LIB_NAME}

all: ${PROG} lib

lib: ${LIB_OBJS}
	ar rcs ${LIB_NAME} ${LIB_OBJS}

.include <minix.lcom.mk>