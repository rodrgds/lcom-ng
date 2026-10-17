# name of the program (Minix service)
PROG=lab2 

LAB1_DIR = ../lab1

# source code files to be compiled
SRCS = lab2.c timer.c utils.c

.BEGIN:
	${MAKE} -C ${LAB1_DIR}

# additional compilation flags
CFLAGS += -pedantic

DPADD += ${LIBLCF}
LDADD += -llcf

LIB_NAME = libtimer.a

CLEANFILES += ${LIB_NAME}

all: lib
lib: timer.o utils.o
	ar rcs ${LIB_NAME} timer.o utils.o

# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk>