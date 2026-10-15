PROG=proj

.PATH: ../..
.PATH: ../../../src/data_structures/
.PATH: ../../../src/monster
.PATH: ../../../src/

SRCS= \
	main.c \
	mp_test.c \
	inputHandler.c \
	monster.c \
	vector.c \
	vec2d.c \
	path.c 


CPPFLAGS += -Iinclude -DLAB3
CFLAGS += -Wall -Wextra -Werror -pedantic -std=c11

DPADD += ${LIBLCF}
LDADD += -llcf -L.
LDADD += -L../../../../lab2 -ltimer
LDADD += -L../../../../lab3 -lkbc
LDADD += -L../../../../lab4 -lmouse
LDADD += -L../../../../lab5 -lterminal 
LDADD += -L../../../../lab5 -lspriteXPM
 

.include <minix.lcom.mk>
