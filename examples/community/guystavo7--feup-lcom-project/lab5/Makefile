# name of the program (Minix service)
PROG=lab5

# source code files to be compiled
.PATH: ${.CURDIR}/.
SRCS = lab5.c ./terminal.c ./terminalUtils.c ./spriteXMP.c

# additional compilation flags
# "-Wall -Wextra -Werror -I . -std=c11 -Wno-unused-parameter" are already set
CFLAGS += -pedantic -DLAB3 -I../lab3/

# list of library dependencies (for Lab 2, only LCF library)
DPADD += ${LIBLCF} ${LIB_NAME}              # Let make track the versions of the new target
LDADD += -llcf -L.                          # Tell the compiler to look in the current directory for libraries
LDADD += -L../lab3/
LDADD += -lkbc

LIB_NAME = libterminal.a                         # The name filename for your library
LIB_SRCS = terminal.c  ./terminalUtils.c               # The source files to include
LIB_OBJS = ${LIB_SRCS:.c=.o}                # The rule used to convert your .c source files in .o object files
CLEANFILES += ${LIB_NAME} ${LIB_OBJS}       # Add the library file and object files to the files to be cleaned

LIB2_NAME = libspriteXPM.a
LIB2_SRCS = ./spriteXMP.c ./terminal.c ./terminalUtils.c
LIB2_OBJS = ${LIB2_SRCS:.c=.o}
CLEANFILES += ${LIB2_NAME} ${LIB2_OBJS}

all: libterminal libsprite                                    # Instruct make run the lib compilation target before building the lab2 program
libterminal: ${LIB_OBJS}                            # Define the lib compilation target, with its dependency source files
	ar rcs ${LIB_NAME} ${LIB_OBJS}          # Package the library
libsprite: ${LIB2_OBJS}
	ar rcs ${LIB2_NAME} ${LIB2_OBJS}

# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk>
