# Nome do binário. Tem de ser "proj".
PROG=proj

# Indicar as pastas de todos os ficheiros a compilar.
.PATH: ${.CURDIR}/devices/
.PATH: ${.CURDIR}/devices/keyboard
.PATH: ${.CURDIR}/devices/mouse
.PATH: ${.CURDIR}/devices/timer
.PATH: ${.CURDIR}/devices/video
.PATH: ${.CURDIR}/game_levels/
.PATH: ${.CURDIR}/

# Ficheiros ".c" a compilar. Têm de estar contidos nas pastas indicadas no passo anterior.
SRCS = 	main.c timer.c utils.c graphics.c KBC.c keyboard.c mouse.c sprite.c game_logic.c game_view.c level1.c level2.c level3.c level4.c level5.c level6.c level7.c level8.c level9.c level10.c

# Flags de compilação
CFLAGS += -D __LCOM_OPTIMIZED__

# Dependências
DPADD += ${LIBLCF}
LDADD += -Wall -llcf -lxpm -llm
.include <minix.lcom.mk>
