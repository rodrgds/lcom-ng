# name of the program (Minix service)
PROG = proj

VPATH = src:src/entities:src/ui

SRCS = proj.c game.c menu.c font.c board.c background.c hud.c sunflower.c peashooter.c zombie.c sun.c projectile.c wave_manager.c name_input.c scoreboard.c plants_cooldown.c animated_sprite.c nut.c

# Labs
LAB1_DIR = ../lab1
LAB2_DIR = ../lab2
LAB3_DIR = ../lab3
LAB4_DIR = ../lab4
LAB5_DIR = ../lab5

# clean the project/ and libs
deep_clean:
	rm -f ../lab*/*.a ../lab*/*.o ../lab*/.gdbinit ${CLEANFILES}

.BEGIN:
	test "${.TARGETS}" = "deep_clean" -o "${.TARGETS}" = "clean" || ${MAKE} -C ${LAB5_DIR} lib

CPPFLAGS += -I${LAB1_DIR} -I${LAB2_DIR} -I${LAB3_DIR} -I${LAB4_DIR} -I${LAB5_DIR} -Isrc/entities -Isrc/ui -Isrc
LDFLAGS  += -L${LAB1_DIR} -L${LAB2_DIR} -L${LAB3_DIR} -L${LAB4_DIR} -L${LAB5_DIR}

DPADD += ${LAB1_DIR}/librtc.a ${LAB2_DIR}/libtimer.a ${LAB3_DIR}/libkeyboard.a ${LAB4_DIR}/libmouse.a ${LAB5_DIR}/libv_card.a ${LIBLCF}
LDADD += -lrtc -ltimer -lkeyboard -lmouse -lv_card -llcf

.include <minix.lcom.mk>