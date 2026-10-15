PROG=proj

.PATH: monster/
.PATH: data_structures/

SRCS= \
	application.c \
	game.c \
	./inputHandler.c \
	./timer.c \
	./boxCollider.c \
	./rigidBody.c \
	./arena.c \
	cursorHandler.c \
	button.c \
	animatedSprite.c \
	tile.c \
	./vec2d.c \
	./player.c \
	./utils.c \
	./playerController.c \
	./playerIdle.c \
	./playerModel.c \
	./playerState.c \
	./playerRunning.c \
	./collectable.c \
	./projectile.c \
	./genericTower.c \
	./basicTower.c \
	monsterController.c \
	monsterView.c \
	monsterModel.c \
	monster.c \
	path.c \
	vector.c \
	pawnMonster.c \
	./gameState.c \
	./piercingTower.c \
	./fastTower.c \
	./mortarTower.c \
	./eletrictTower.c \
	./basicLongTower.c \
	alienMonster.c \
	spearManMonster.c \
	bossMonster.c \
	monsterGen.c \
	waves.c \
	./healthBar.c \
	./statsBar.c \
	./numbers.c \
	./buildMenu.c \
	./waveFunctions.c

OBJS = ${SRCS:.c=.o}

CPPFLAGS += -Iinclude -I../../lab5 -DLAB3
CFLAGS += -Wall -Wextra -Werror -pedantic -std=c11

DPADD += ${LIBLCF}
LDADD += -llcf -L.
LDADD += -L../../lab2 -ltimer
LDADD += -L../../lab3 -lkbc
LDADD += -L../../lab4 -lmouse
LDADD += -L../../lab5 -lterminal
LDADD += -L../../lab5 -lspriteXPM

all:
	mkdir -p ../build
	mv $(OBJS) ../build

.include <minix.lcom.mk>
