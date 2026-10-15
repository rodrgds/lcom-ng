# name of the program (Minix service)
PROG=lab4

# source code files to be compiled
SRCS = lab4.c KBC.c mouse.c utils.c timer.o

# additional compilation flags
# "-Wall -Wextra -Werror -I . -std=c11 -Wno-unused-parameter" are already set
CFLAGS += -pedantic

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
