# name of the program (Minix service)
PROG=lab3

# source code files to be compiled
SRCS = lab3.c kbd.c utils.c timer.o

# additional compilation flags
# "-Wall -Wextra -Werror -I . -std=c11 -Wno-unused-parameter" are already set
CFLAGS += -pedantic 
CPPFLAGS += -D LAB3

# list of library dependencies (for Lab 2, only LCF library)
DPADD += ${LIBLCF}
LDADD += -llcf

# search for source files in ../lab2
VPATH = ../lab2

# rule to compile ../lab2/timer.c into timer.o in the current directory
timer.o: ../lab2/timer.c
	${CC} ${CFLAGS} ${CPPFLAGS} -c $< -o $@  

# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk>