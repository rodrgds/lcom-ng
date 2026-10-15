# name of the program (Minix service)
PROG=lab5

# source code files to be compiled
SRCS = lab5.c utils.c kbd.o graphics.c timer.o

# additional compilation flags
# "-Wall -Wextra -Werror -I . -std=c11 -Wno-unused-parameter" are already set
CFLAGS += -pedantic -I../lab3 -I../lab2

# list of library dependencies (for Lab 2, only LCF library)
DPADD += ${LIBLCF}
LDADD += -llcf

# search for source files in ../lab3
VPATH = ../lab3 ../lab2

# rule to compile ../lab3/kbd.c into kbd.o in the current directory
kbd.o: ../lab3/kbd.c
	${CC} ${CFLAGS} ${CPPFLAGS} -c $< -o $@

# rule to compile ../lab2/timer.c into timer.o in the current directory
timer.o: ../lab2/timer.c
	${CC} ${CFLAGS} ${CPPFLAGS} -c $< -o $@	
	
# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk>
