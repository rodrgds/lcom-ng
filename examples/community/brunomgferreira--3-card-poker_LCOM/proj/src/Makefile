# name of the program (Minix service)
PROG=proj

.PATH: ${.CURDIR}/keyboard_mouse
.PATH: ${.CURDIR}/rtc
.PATH: ${.CURDIR}/timer
.PATH: ${.CURDIR}/video
.PATH: ${.CURDIR}/sprites
.PATH: ${.CURDIR}/states
.PATH: ${.CURDIR}/states/game

# source code files to be compiled
SRCS = proj.c kbc.c keyboard.c mouse.c rtc.c timer.c video.c utils.c sprite.c animSprite.c main.c game.c choosePlayer.c createPlayer.c menu.c card.c menuOption.c

# additional compilation flags
# "-Wall -Wextra -Werror -I . -std=c11 -Wno-unused-parameter" are already set
CFLAGS += -pedantic

# list of library dependencies (for Lab 2, only LCF library)
DPADD += ${LIBLCF}
LDADD += -llcf

# specify the directory for object files
OBJDIR = .

# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk>
