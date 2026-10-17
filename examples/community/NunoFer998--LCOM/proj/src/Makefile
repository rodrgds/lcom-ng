# name of the program (Minix service)
PROG=proj

.PATH: ${.CURDIR}/Controller/Devices/Mouse_Keyboard/Mouse
.PATH: ${.CURDIR}/Controller/Devices/Mouse_Keyboard/Keyboard
.PATH: ${.CURDIR}/Controller/Devices/Mouse_Keyboard
.PATH: ${.CURDIR}/Controller/Devices/Timer
.PATH: ${.CURDIR}/Controller/Devices/VideoCard
.PATH: ${.CURDIR}/Controller/Devices
.PATH: ${.CURDIR}/View/MouseView
.PATH: ${.CURDIR}/View/MainMenuView
.PATH: ${.CURDIR}/Model/Background
.PATH: ${.CURDIR}/Model/Sprite
.PATH: ${.CURDIR}/Model/Mouse
.PATH: ${.CURDIR}/Controller/MouseController
.PATH: ${.CURDIR}/View/InstructionsView
.PATH: ${.CURDIR}/View/GameOverView
.PATH: ${.CURDIR}/View/GameView
.PATH: ${.CURDIR}/Controller/MainMenuController
.PATH: ${.CURDIR}/Controller/GameOverController
.PATH: ${.CURDIR}/Controller/GameController
.PATH: ${.CURDIR}/Model/SelectorMine


# source code files to be compiled
SRCS = keyboard.c kbc.c sprite.c  background.c proj.c timer.c selectorMine.c mouse.c video_graphics.c func_aux.c gameView.c gameOverView.c mainMenuView.c mouseView.c instructionsView.c gameController.c gameOverController.c mainMenuController.c mouseController.c mouseModel.c

# additional compilation flags
# "-Wall -Wextra -Werror -I . -std=c11 -Wno-unused-parameter" are already set
CFLAGS += -pedantic -D _LCOM_OPTIMIZED_

# list of library dependencies (for Lab 2, only LCF library)
DPADD += ${LIBLCF}
LDADD += -llcf -lxpm -llm

# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk>