# Nome do binário. Tem de ser "proj".
PROG=proj

# Indicar as pastas de todos os ficheiros a compilar.
.PATH: ${.CURDIR}/keyboard
#.PATH: ${.CURDIR}/mouse
.PATH: ${.CURDIR}/timer
.PATH: ${.CURDIR}/graphics
#.PATH: ${.CURDIR}/rtc



# Ficheiros ".c" a compilar. Têm de estar contidos nas pastas indicadas no passo anterior.
SRCS = piece.c interrupts.c powerups.c ball.c main.c timer.c utils.c video_gr.c kbc.c keyboard.c entity.c menu.c

# Flags de compilação
CFLAGS += -pedantic -D_LCOM_OPTIMIZED_ -O2

# Dependências
DPADD += ${LIBLCF}
LDADD += -Wall -llcf -lxpm -llm
.include <minix.lcom.mk>
