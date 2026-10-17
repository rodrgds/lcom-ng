# name of the program (Minix service)
PROG=proj

.PATH: ${.CURDIR}/src
.PATH: ${.CURDIR}/src/Model
.PATH: ${.CURDIR}/src/Controller/Timer
.PATH: ${.CURDIR}/src/Controller/Mouse_Keyboard
.PATH: ${.CURDIR}/src/Controller/VBE
.PATH: ${.CURDIR}/src/GUI
.PATH: ${.CURDIR}/src/Model/Elements
.PATH: ${.CURDIR}/src/Controller/Game
.PATH: ${.CURDIR}/src/Model/Map



# source code files to be compiled
SRCS = main.c \
		utils.c \
		GPU.c\
		timer.c \
		keyboard.c\
		mouse.c \
		gui.c \
		Cursor.c \
		CursorController.c \
		GameController.c \
		PlayerController.c \
		MapController.c \
		BackpackController.c \
		MenusController.c \
		Map.c \
		player.c \
		bitmap.c \
		Backpack.c \
		Element.c \
		Sun.c \
		Moon.c \
		Cloud.c \


# additional compilation flags
# "-Wall -Wextra -Werror -I . -std=c11 -Wno-unused-parameter" are already set
CFLAGS += -pedantic -DLAB3 -O2 -D_LCOMOPTIMIZED

# list of library dependencies (for Lab 2, only LCF library)
DPADD += ${LIBLCF, liblm.a, libxpm.a}
LDADD += -llcf -lm

# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk>
