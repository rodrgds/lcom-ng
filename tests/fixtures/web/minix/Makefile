PROG = proj
SRCS = main.c
LIB_NAME = libvalue.a

# Dependency chain crosses these two archives four times, requiring a fixed point.
LIB_SRCS = value.c early2.c early3.c early4.c
LDADD += -lvalue -llate
liblate.a: late1.o late2.o late3.o late4.o
	ar rcs liblate.a late1.o late2.o late3.o late4.o
.include <minix.lcom.mk>
