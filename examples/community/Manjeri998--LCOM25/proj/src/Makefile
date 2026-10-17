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
.PATH: ${.CURDIR}/Model/Sprite
.PATH: ${.CURDIR}/Model/Mouse
.PATH: ${.CURDIR}/Model/Background
.PATH: ${.CURDIR}/Controller/MouseController
.PATH: ${.CURDIR}/View/InstructionsView
.PATH: ${.CURDIR}/View/GameOverView
.PATH: ${.CURDIR}/View/GameView
.PATH: ${.CURDIR}/Controller/MainMenuController
.PATH: ${.CURDIR}/Controller/GameOverController
.PATH: ${.CURDIR}/Controller/GameController
.PATH: ${.CURDIR}/Model/SelectorMine


# source code files to be compiled
SRCS = mouse.c keyboard.c kbc.c proj.c timer.c video_graphics.c func_aux.c mouseView.c sprite.c mouseModel.c instructionsView.c background.c mainMenuView.c mainMenuController.c selectorMine.c gameOverController.c gameOverView.c gameController.c gameView.c mouseController.c

# additional compilation flags
# "-Wall -Wextra -Werror -I . -std=c11 -Wno-unused-parameter" are already set
CFLAGS += -pedantic -D _LCOM_OPTIMIZED_

# list of library dependencies (for Lab 2, only LCF library)
DPADD += ${LIBLCF}
LDADD += -llcf -lxpm -llm

# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk>