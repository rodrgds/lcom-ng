# name of the program (Minix service)
PROG=proj

.PATH: ${.CURDIR}/src/drivers
.PATH: ${.CURDIR}/src
.PATH: ${.CURDIR}/src/drivers/Graphics
.PATH: ${.CURDIR}/src/drivers/Keyboard_and_Mouse
.PATH: ${.CURDIR}/src/drivers/RTC
.PATH: ${.CURDIR}/src/drivers/Timer
.PATH: ${.CURDIR}/src/drivers/Serial_Port
.PATH: ${.CURDIR}/src/game

# source code files to be compiled 
SRCS = 	main.c timer.c utils.c graphics.c sprite.c KBC.c keyboard.c mouse.c app_controller.c game_controller.c IO_controller.c  

# additional compilation flags
# "-Wall -Wextra -Werror -I . -std=c11 -Wno-unused-parameter" are already set
CFLAGS += -pedantic -D_LCOM_OPTIMIZED_

# list of library dependencies
DPADD += ${LIBLCF}
LDADD += -llcf -lxpm -llm

# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk>
