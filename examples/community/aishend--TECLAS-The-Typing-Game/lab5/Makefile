PROG = lab5
SRCS = lab5.c

# 1. NOME DA TUA NOVA BIBLIOTECA E FICHEIRO FONTE
LIB_NAME = libvideo.a
LIB_SRCS = video_gr.c
LIB_OBJS = ${LIB_SRCS:.c=.o}

DPADD += ${LIBLCF} ${LIB_NAME}

# 2. LINKAGEM DAS LIBS (A nova libvideo local, mais as do teclado e timer dos labs anteriores)
LDADD += -llcf -L. -lvideo -L../lab3 -lkeyboard -L../lab2 -ltimer

CLEANFILES += ${LIB_NAME} ${LIB_OBJS}

all: lib
lib: ${LIB_OBJS}
	ar rcs ${LIB_NAME} ${LIB_OBJS}

.include <minix.lcom.mk>