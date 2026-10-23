# Nome do programa
PROG = proj

# Ficheiros fonte do programa
SRCS = main.c game_state.c player.c renderer.c timer.c utils.c enemy.c mouse.c bullet.c level_config.c

.PATH: ../lab2 ../lab4

DPADD += ../lab1/librtc.a
DPADD += ../lab3/libkbd.a
DPADD += ../lab5/libvideo.a
DPADD += ${LIBLCF}

LDADD += ../lab1/librtc.a
LDADD += -L../lab3 -lkbd
LDADD += -L../lab5 -lvideo
LDADD += -lm
LDADD += -llcf

# Flags
CFLAGS += -pedantic
CFLAGS += -I.
CFLAGS += -Iinclude
CFLAGS += -I../lab1
CFLAGS += -I../lab2
CFLAGS += -I../lab3
CFLAGS += -I../lab4
CFLAGS += -I../lab5
CFLAGS += -Imedia/

# Regras para compilar os laboratórios como bibliotecas
../lab1/librtc.a:
	$(MAKE) -C ../lab1
	
# lab2 primeiro, os outros dependem dele
../lab2/libutils.a:
	$(MAKE) -C ../lab2

../lab3/libkbd.a: ../lab2/libutils.a
	$(MAKE) -C ../lab3

../lab4/libmouse.a: ../lab2/libutils.a
	$(MAKE) -C ../lab4

../lab5/libvideo.a: ../lab2/libutils.a ../lab4/libmouse.a
	$(MAKE) -C ../lab5

.include <minix.lcom.mk>