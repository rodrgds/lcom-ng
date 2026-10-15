# name of the program (Minix service)
PROG=lab3 

# source code files to be compiled
SRCS = lab3.c kbc.c

# additional compilation flags
# "-Wall -Wextra -Werror -I . -std=c11 -Wno-unused-parameter" are already set
CFLAGS += -pedantic
CFLAGS += -DLAB3 	# define the macro LAB3 to be used in the code to conditionally compile code specific for Lab 3

# list of library dependencies (for Lab 3, only LCF library)
DPADD += ${LIBLCF} ${LIB_NAME}
LDADD += -llcf
LDADD += -L. -L../lab2  # Tell the compiler to look in the lab2 and lab3 directory for libraries
LDADD += -ltimer      # Link the library named libtimers.a and libutils.a
LDADD += -lkbd            # Link the library named libkbd.a

LIB_NAME = libkbd.a 												# name of the libs filename
LIB_SRCS = kbc.c									# source files to include
LIB_OBJS = ${LIB_SRCS:.c=.o}									#The rule used to convert your .c source files in .o object files
CLEANFILES += ${LIB_NAME} ${LIB_OBJS} 				# add the library

all: lib ${PROG}
lib: ${LIB_OBJS}
	ar rcs ${LIB_NAME} ${LIB_OBJS}		# create the library file	from the object files


# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk>
