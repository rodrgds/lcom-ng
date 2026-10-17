# name of the program (Minix service)
PROG=proj

# source code files to be compiled

SRCS := $(shell find src -type f -name '*.c')

# additional compilation flags
# "-Wall -Wextra -Werror -I . -std=c11 -Wno-unused-parameter" are already set
CFLAGS += -pedantic

CFLAGS += -o $@
CFLAGS += -Wno-c11-extensions
CFLAGS += -Wno-overlength-strings

# list of library dependencies
DPADD += ${LIBLCF}
LDADD += -llcf

# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk>