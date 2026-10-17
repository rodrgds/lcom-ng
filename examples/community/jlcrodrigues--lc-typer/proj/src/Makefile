# name of the program (Minix service)
PROG=proj

.PATH: ${.CURDIR}/drivers/driver
.PATH: ${.CURDIR}/drivers/kbc
.PATH: ${.CURDIR}/drivers/rtc
.PATH: ${.CURDIR}/drivers/timer
.PATH: ${.CURDIR}/drivers/utils
.PATH: ${.CURDIR}/drivers/video

.PATH: ${.CURDIR}/states
.PATH: ${.CURDIR}/typer

# source code files to be compiled
DRIVERS = driver.c keyboard.c mouse.c rtc.c timer.c utils.c video_gr.c sprite.c
TYPER = proj.c game.c menu.c game_over.c mouse_sprite.c button.c scoreboard.c
SRCS = $(DRIVERS) $(TYPER)

# additional compilation flags
# "-Wall -Wextra -Werror -I . -std=c11 -Wno-unused-parameter" are already set

INCLUDE_TYPER = -I./typer -I./states
INCLUDE_DRIVERS = -I./drivers -I./drivers/driver -I./drivers/kbc -I./drivers/timer -I./drivers/utils -I./drivers/video -I./drivers/rtc
INCLUDE_ASSETS = -I./assets -I./assets/xpm -I./assets/font -I./assets/phrases
INCLUDE_FLAGS = $(INCLUDE_TYPER) $(INCLUDE_DRIVERS) $(INCLUDE_ASSETS)


CFLAGS += -pedantic
CPPFLAGS += -pedantic $(INCLUDE_FLAGS) # -D __LCOM_OPTIMIZED__ 

# list of library dependencies (only LCF library)
DPADD += ${LIBLCF}
LDADD += -llcf

# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk>
