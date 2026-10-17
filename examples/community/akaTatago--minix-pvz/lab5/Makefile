# name of the program (Minix service)
PROG = lab5

# Labs
LAB1_DIR = ../lab1
LAB2_DIR = ../lab2
LAB3_DIR = ../lab3
LAB4_DIR = ../lab4

# source code files to be compiled
SRCS = lab5.c v_card.c

.BEGIN:
	${MAKE} -C ${LAB4_DIR}

# additional compilation flags
CFLAGS += -pedantic
CFLAGS += -DLAB5

LIB_NAME = libv_card.a
LIB_SRCS = v_card.c
LIB_OBJS = ${LIB_SRCS:T:.c=.o}

# Dependencies and Linker flags
DPADD += ${LIBLCF} ${LAB2_DIR}/libtimer.a ${LAB3_DIR}/libkeyboard.a ${LAB4_DIR}/libmouse.a
LDADD += -llcf -L. -lv_card -L${LAB2_DIR} -ltimer -L${LAB3_DIR} -lkeyboard -L${LAB4_DIR} -lmouse

# Files to remove on 'make clean'
CLEANFILES += ${LIB_NAME} ${LIB_OBJS}

lib: ${LAB1_DIR}/librtc.a ${LAB2_DIR}/libtimer.a ${LAB3_DIR}/libkeyboard.a ${LAB4_DIR}/libmouse.a ${LIB_OBJS}
	ar rcs libv_card.a v_card.o ${LAB2_DIR}/utils.o ${LAB2_DIR}/timer.o ${LAB3_DIR}/keyboard.o ${LAB4_DIR}/mouse.o

# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk>