# name of the program (Minix service)
PROG=lab2 

# source code files to be compiled
SRCS = lab2.c timer.c utils.c

# additional compilation flags
# "-Wall -Wextra -Werror -I . -std=c11 -Wno-unused-parameter" are already set
CFLAGS += -pedantic

# list of library dependencies (for Lab 2, only LCF library)
DPADD += ${LIBLCF} ${LIB_NAME}
LDADD += -llcf -L. 								# look for the library in the current directory
LDADD += -ltimer	 								# link the timer library

LIB_NAME = libtimer.a 												# name of the libs filename 
LIB_SRCS = timer.c utils.c										# source files to include
LIB_OBJS = ${LIB_SRCS:.c=.o}									#The rule used to convert your .c source files in .o object files
CLEANFILES += ${LIB_NAME} ${LIB_OBJS} 				# add the library file and object files to the files to be cleaned


all: lib
lib: ${LIB_OBJS}
	ar rcs ${LIB_NAME} ${LIB_OBJS}		# create the library file from the object files

# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk>
