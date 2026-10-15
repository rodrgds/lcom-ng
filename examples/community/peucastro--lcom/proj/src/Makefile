# name of the program (Minix service)
PROG=proj

# additional compilation flags
# "-Wall -Wextra -Werror -I . -std=c11 -Wno-unused-parameter" are already set
CFLAGS += -pedantic -D __LCOM_OPTIMIZED__

# list of library dependencies (LCF)
DPADD += ${LIBLCF}
LDADD += -llcf
LDLIBS += -lm

# define headers dependencies
HDRS = $(find . -name *.h)

# compilation rules for each source file
proj.o: proj.c ${HDRS}
	${CC} ${CFLAGS} -c $< -o $@

utils/utils.o: utils/utils.c utils/utils.h
	${CC} ${CFLAGS} -c $< -o $@

model/sprite/sprite.o: model/sprite/sprite.c model/sprite/sprite.h
	${CC} ${CFLAGS} -c $< -o $@

model/sprite/anim_sprite.o: model/sprite/anim_sprite.c model/sprite/anim_sprite.h
	${CC} ${CFLAGS} -c $< -o $@

model/board/board.o: model/board/board.c model/board/board.h
	${CC} ${CFLAGS} -c $< -o $@

model/entity/entity.o: model/entity/entity.c model/entity/entity.h
	${CC} ${CFLAGS} -c $< -o $@

model/resources/resources.o: model/resources/resources.c model/resources/resources.h
	${CC} ${CFLAGS} -c $< -o $@

model/game/game.o: model/game/game.c model/game/game.h
	${CC} ${CFLAGS} -c $< -o $@

model/game/movement.o: model/game/movement.c model/game/movement.h
	${CC} ${CFLAGS} -c $< -o $@

model/game/combat.o: model/game/combat.c model/game/combat.h
	${CC} ${CFLAGS} -c $< -o $@

view/view.o: view/view.c view/view.h
	${CC} ${CFLAGS} -c $< -o $@

controller/timer/timer.o: controller/timer/timer.c controller/timer/timer.h
	${CC} ${CFLAGS} -c $< -o $@

controller/rtc/rtc.o: controller/rtc/rtc.c controller/rtc/rtc.h
	${CC} ${CFLAGS} -c $< -o $@

controller/kbc/kbc.o: controller/kbc/kbc.c controller/kbc/kbc.h
	${CC} ${CFLAGS} -c $< -o $@

controller/kbc/kbd.o: controller/kbc/kbd.c controller/kbc/kbd.h
	${CC} ${CFLAGS} -c $< -o $@

controller/kbc/mouse.o: controller/kbc/mouse.c controller/kbc/mouse.h
	${CC} ${CFLAGS} -c $< -o $@

controller/graphics/vbe.o: controller/graphics/vbe.c controller/graphics/vbe.h
	${CC} ${CFLAGS} -c $< -o $@

controller/graphics/graphics.o: controller/graphics/graphics.c controller/graphics/graphics.h
	${CC} -O0 -fno-inline -fno-unroll-loops ${CFLAGS} -c $< -o $@

controller/ih/ih.o: controller/ih/ih.c controller/ih/ih.h
	${CC} ${CFLAGS} -c $< -o $@

controller/input/input.o: controller/input/input.c controller/input/input.h
	${CC} ${CFLAGS} -c $< -o $@

controller/ev/ev.o: controller/ev/ev.c controller/ev/ev.h
	${CC} ${CFLAGS} -c $< -o $@

# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk>
