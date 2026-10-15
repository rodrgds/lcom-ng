PROG=proj

.PATH: ${.CURDIR}/controller \
       ${.CURDIR}/input \
       ${.CURDIR}/model \
       ${.CURDIR}/model/champion \
       ${.CURDIR}/model/minion \
       ${.CURDIR}/model/tower \
       ${.CURDIR}/model/state \
       ${.CURDIR}/model/button \
       ${.CURDIR}/viewer \
       ${.CURDIR}/viewer/sprite

SRCS = main.c \
       ChampionController.c \
       Controller.c \
       GameController.c \
       MinionController.c \
       TowerController.c \
       MenuController.c \
       ButtonController.c \
       KeyboardInterpreter.c \
       MouseInterpreter.c \
       TimerInterpreter.c \
       Box.c \
       Position.c \
       Champion.c \
       Minion.c \
       Tower.c \
       GameState.c \
       GameOverState.c \
       State.c \
       MenuState.c \
       Button.c \
       AnimSprite.c \
       BuildSprites.c \
       Sprite.c \
       ChampionViewer.c \
       GameViewer.c \
       HpBarViewer.c \
       MinionViewer.c \
       TowerViewer.c \
       ButtonViewer.c \
       MenuViewer.c \
       Viewer.c \
       shopBarViewer.c \
       ShopState.c \
       shopBarState.c \
       GameOverViewer.c \
       ShopViewer.c \
       ShopController.c \
       ShopBarController.c


CFLAGS += -pedantic
CFLAGS += -I${.CURDIR}/../lab2
CFLAGS += -I${.CURDIR}/../lab3
CFLAGS += -I${.CURDIR}/../lab4
CFLAGS += -I${.CURDIR}/../lab5

DPADD += ${LIBLCF}
LDADD += -llcf \
    -L${.CURDIR}/../lab5 -lvideo \
    -L${.CURDIR}/../lab4 -lmouse \
    -L${.CURDIR}/../lab3 -lkeyboard \
    -L${.CURDIR}/../lab2 -ltimer

.include <minix.lcom.mk>