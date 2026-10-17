# name of the program (Minix service)
PROG=proj

.PATH: ${.CURDIR}/src
.PATH: ${.CURDIR}/src/controllers/libkbc/drivers
.PATH: ${.CURDIR}/src/controllers/libkbc/formatters
.PATH: ${.CURDIR}/src/controllers/libutils
.PATH: ${.CURDIR}/src/controllers/libtimer
.PATH: ${.CURDIR}/src/controllers/libvideo
.PATH: ${.CURDIR}/src/controllers/librtc
.PATH: ${.CURDIR}/src/states

# source code files to be compiled
SRCS = main.c KBC.c keyboard.c mouse.c KBCInput.c KeyboardInput.c MouseInput.c KBCState.c timer.c utils.c graphics.c InstructionsState.c MenuState.c CharacterState.c states.c projXPM.c GameState.c SettingsState.c rtc.c

# additional compilation flags
# "-Wall -Wextra -Werror -I . -std=c11 -Wno-unused-parameter" are already set
CFLAGS += -pedantic -O2

# list of library dependencies (for Lab 2, only LCF library)
DPADD += ${LIBLCF}
LDADD += -llcf

# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk>
