# Nome do binário. Tem de ser "proj".
PROG=proj

# Indicar as pastas de todos os ficheiros a compilar.
.PATH: ${.CURDIR}/controller/
.PATH: ${.CURDIR}/controller/keyboard
.PATH: ${.CURDIR}/controller/mouse
.PATH: ${.CURDIR}/controller/timer
.PATH: ${.CURDIR}/controller/video
.PATH: ${.CURDIR}/controller/KBC
.PATH: ${.CURDIR}/controller/rtc
.PATH: ${.CURDIR}/model/
.PATH: ${.CURDIR}/view/

# Ficheiros ".c" a compilar. Têm de estar contidos nas pastas indicadas no passo anterior.
SRCS = 	main.c timer.c utils.c graphics.c KBC.c keyboard.c mouse.c sprite.c model.c view.c rtc.c controller.c

# Flags de compilação
CFLAGS += -pedantic -O2 -D_LCOM_OPTIMIZED_

# Dependências
DPADD += ${LIBLCF}
LDADD += -Wall -llcf -lxpm -llm
.include <minix.lcom.mk>