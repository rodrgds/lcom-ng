# Nome do binário. Tem de ser "proj".
PROG=proj

# Indicar as pastas de todos os ficheiros a compilar.
.PATH: ${.CURDIR}/devices/
.PATH: ${.CURDIR}/devices/keyboard
.PATH: ${.CURDIR}/devices/mouse
.PATH: ${.CURDIR}/devices/timer
.PATH: ${.CURDIR}/devices/graphics
.PATH: ${.CURDIR}/devices/rtc
.PATH: ${.CURDIR}/devices/util
.PATH: ${.CURDIR}/game/
.PATH: ${.CURDIR}/game/draw
.PATH: ${.CURDIR}/game/elements
.PATH: ${.CURDIR}/game/elements/sprite
.PATH: ${.CURDIR}/game/interrupts_actions
.PATH: ${.CURDIR}/game/allocations_manager
.PATH: ${.CURDIR}/game/moves


# Ficheiros ".c" a compilar. Têm de estar contidos nas pastas indicadas no passo anterior.
SRCS = 	proj.c timer.c utils.c graphics.c keyboard.c mouse.c rtc.c sprite.c moves.c interrupts.c KBC.c elements.c draw.c game.c allocations_manager.c

# Flags de compilação
CFLAGS += -pedantic -D_LCOM_OPTIMIZED_ -O2

# Dependências
DPADD += ${LIBLCF}
LDADD += -Wall -llcf -lxpm -llm
.include <minix.lcom.mk>