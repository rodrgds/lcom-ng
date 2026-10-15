# name of the program (Minix service)
PROG=lab4 

# source code files to be compiled
SRCS = lab4.c mouse.c

# additional compilation flags
# "-Wall -Wextra -Werror -I . -std=c11 -Wno-unused-parameter" are already set
CFLAGS += -pedantic
CFLAGS += -DLAB4 	# define the macro LAB4 to be used in the code to conditionally compile code specific for Lab 4
CFLAGS += -I../lab2 -I../lab3

# list of library dependencies
DPADD += ${LIBLCF} ${LIB_NAME}
LDADD += -llcf -L../lab2 -L../lab3  # Tell the compiler to look in the lab2, Lab3 and Lab4 directory for libraries
LDADD += -ltimer -lkbd
LDADD += -lmouse

LIB_NAME = libmouse.a 												# name of the libs filename
LIB_SRCS = mouse.c														# source files to include
LIB_OBJS = ${LIB_SRCS:.c=.o}									#The rule used to convert your .c source files in .o object files
CLEANFILES += ${LIB_NAME} ${LIB_OBJS} 				# add the library

all: lib ${PROG}
lib: ${LIB_OBJS}
	ar rcs ${LIB_NAME} ${LIB_OBJS}		# create the library file	from the object files


# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk>
