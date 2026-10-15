# name of the program (Minix service)
PROG=proj

#folders of the files 
.PATH: ${.CURDIR}/controller/
.PATH: ${.CURDIR}/model/
.PATH: ${.CURDIR}/view/

# source code files to be compiled
SRCS = proj.c utils.c timer.c KBC.c keyboard.c mouse.c graphics.c rtc.c  game.c sprite.c arena.c menu.c start.c rules.c help.c mode.c game_over.c game_view.c sprite_view.c arena_view.c menu_view.c start_view.c rules_view.c help_view.c mode_view.c game_over_view.c

# additional compilation flags
# "-Wall -Wextra -Werror -I . -std=c11 -Wno-unused-parameter" are already set
CFLAGS += -pedantic 

# list of library dependencies (for Lab 2, only LCF library)
DPADD += ${LIBLCF}
LDADD += -llcf

# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk>
