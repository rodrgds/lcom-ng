# Nome do binário. Tem de ser "proj".
PROG=proj

.PATH: ${.CURDIR}/model/
.PATH: ${.CURDIR}/view/
.PATH: ${.CURDIR}/controller/
.PATH: ${.CURDIR}/devices/
.PATH: ${.CURDIR}/font/

SRCS = 	main.c game.c timer.c video.c utils.c mouse.c keyboard.c kbc.c crosshair.c crosshair_view.c crosshair_controller.c target.c target_view.c target_controller.c state.c menu.c menu_view.c menu_controller.c game_view.c numbers.c

# Flags adicionais de compilação
CFLAGS += -pedantic
PPFLAGS += -DPROJ

# Dependências de bibliotecas
DPADD += ${LIBLCF}
LDADD += -llcf

.include <minix.lcom.mk>