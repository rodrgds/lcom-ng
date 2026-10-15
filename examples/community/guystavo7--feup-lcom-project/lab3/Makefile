# name of the program (Minix service)
PROG=lab3 

# source code files to be compiled
SRCS = lab3.c kbc.c utils.c
CFLAGS += -I../lab2                # Specify directories to the compiler to search for header files, libraries

LDADD += -llcf -L../lab2           # Tell the compiler to look in the lab2 directory for libraries
LDADD += -ltimer                   # Link the library named libtimers.a

# additional compilation flags
# "-Wall -Wextra -Werror -I . -std=c11 -Wno-unused-parameter" are already set
CFLAGS += -pedantic
CFLAGS += -DLAB3

DPADD += ${LIBLCF} 

LDADD += -llcf -L.                          # Tell the compiler to look in the current directory for libraries
 
LIB_NAME = libkbc.a                         # The name filename for your library
LIB_SRCS = kbc.c ./utils.c                # The source files to include
LIB_OBJS = ${LIB_SRCS:.c=.o}                # The rule used to convert your .c source files in .o object files
CLEANFILES += ${LIB_NAME} ${LIB_OBJS}       # Add the library file and object files to the files to be cleaned

all: lib                                    # Instruct make run the lib compilation target before building the lab2 program
lib: ${LIB_OBJS}                            # Define the lib compilation target, with its dependency source files
	ar rcs ${LIB_NAME} ${LIB_OBJS}          # Package the library

.include <minix.lcom.mk>
