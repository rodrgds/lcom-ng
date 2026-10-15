# name of the program (Minix service)
PROG=lab5 

# source code files to be compiled
SRCS = lab5.c video_gr.c

# additional compilation flags
# "-Wall -Wextra -Werror -I . -std=c11 -Wno-unused-parameter" are already set
CFLAGS += -pedantic
CFLAGS += -DLAB5 	# define the macro LAB5 to be used in the code to conditionally compile code specific for Lab 5
CFLAGS += -I../lab3

# list of library dependencies
DPADD += ${LIBLCF} ${LIB_NAME}
LDADD += -llcf -L../lab3  # Tell the compiler to look in the lab3 directory for libraries
LDADD += -lkbd -ltimer -L. -L../lab2
LDADD += -lgraphics


LIB_NAME = libgraphics.a 												# name of the libs filename
LIB_SRCS = video_gr.c														# source files to include
LIB_OBJS = ${LIB_SRCS:.c=.o}									#The rule used to convert your .c source files in .o object files
CLEANFILES += ${LIB_NAME} ${LIB_OBJS} 				# add the library

all: lib ${PROG}
lib: ${LIB_OBJS}
	ar rcs ${LIB_NAME} ${LIB_OBJS}		# create the library file	from the object files


# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk>
