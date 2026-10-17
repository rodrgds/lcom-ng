# name of the program (Minix service)
PROG=lab3 

# source code files to be compiled
SRCS = lab3.c keyboard.c

# additional compilation flags
CFLAGS += -pedantic
CFLAGS += -DLAB3

LAB2_DIR = ../lab2
.BEGIN:
	${MAKE} -C ${LAB2_DIR}

LIB_NAME = libkeyboard.a

# Dependencies and Linker flags
DPADD += ${LIBLCF} ${LIB_NAME}
LDADD += -llcf -L. -lkeyboard -L${LAB2_DIR} -ltimer 

# Files to remove on 'make clean'
CLEANFILES += ${LIB_NAME}

# Tell make to build the local library before the program
all: lib
lib: ${LAB2_DIR}/libtimer.a keyboard.o
	ar rcs ${LIB_NAME} keyboard.o ${LAB2_DIR}/utils.o
    

# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk>