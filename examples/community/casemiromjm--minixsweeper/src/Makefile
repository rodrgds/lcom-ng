# name of the program (Minix service)
PROG=proj

# Caminhos para os diretórios dos ficheiros-fonte
.PATH: ${.CURDIR}
.PATH: ${.CURDIR}/sprites
.PATH: ${.CURDIR}/model
.PATH: ${.CURDIR}/view
.PATH: ${.CURDIR}/controller


# source code files to be compiled
SRCS = \
  proj.c \
  utils.c \
  timer.c \
  sprite.c \
  game.c \
  map.c \
  menu.c \
  graphics.c \
  view.c \
  controller.c \
  kbc.c \
  keyboard.c \
  mouse.c \
  rtc.c

# additional compilation flags
CFLAGS += -pedantic

# library dependencies
DPADD += ${LIBLCF}
LDADD += -llcf

# include LCOM's makefile
.include <minix.lcom.mk>
