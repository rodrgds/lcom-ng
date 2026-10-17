# name of the program (Minix service)
PROG=proj 

# source code files to be compiled
SRCS = proj.c graphics.c keyboard.c utils.c timer.c mouse.c game.c mainmenu.c gameovermenu.c scoreboardmenu.c RTC.c

# additional compilation flags
# "-Wall -Wextra -Werror -I . -std=c11 -Wno-unused-parameter" are already set
CPPFLAGS += -pedantic -DPROJ -D __LCOM_OPTIMIZED__

# list of library dependencies (for Lab 4, only LCF library)
DPADD += ${LIBLCF}
LDADD += -llcf

# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk>
