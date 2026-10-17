# name of the program (Minix service)
PROG=proj

# player data
.PATH: ${.CURDIR}/player_data

# device controllers
.PATH: ${.CURDIR}/device_controllers
.PATH: ${.CURDIR}/device_controllers/timer
.PATH: ${.CURDIR}/device_controllers/video
.PATH: ${.CURDIR}/device_controllers/kbc
.PATH: ${.CURDIR}/device_controllers/rtc
.PATH: ${.CURDIR}/device_controllers/uart
# sprites
.PATH: ${.CURDIR}/sprites/xpms
# models
.PATH: ${.CURDIR}/model
.PATH: ${.CURDIR}/model/stateModels
#view
.PATH: ${.CURDIR}/viewer
.PATH: ${.CURDIR}/viewer/menus
.PATH: ${.CURDIR}/viewer/gameElements
.PATH: ${.CURDIR}/viewer/minigames
# controllers
.PATH: ${.CURDIR}/controller
.PATH: ${.CURDIR}/controller/menus
.PATH: ${.CURDIR}/controller/gameElements
.PATH: ${.CURDIR}/controller/minigames
# utilities
.PATH: ${.CURDIR}/utilities


# source code files to be compiled
SRCS = proj.c timer.c kbc.c utils.c keyboard.c mouse.c video.c guiDrawer.c cursor.c mainMenu.c button.c mainMenuController.c mainMenuViewer.c mainRoom.c mainRoomController.c mainRoomViewer.c minigotchi.c minigameMenu.c minigameMenuController.c minigameMenuViewer.c item.c hotbar.c database.c nameMinigotchi.c nameMinigotchiViewer.c nameMinigotchiController.c bar.c vector.c rtc.c queue.c uart.c ticTacToeController.c ticTacToeViewer.c ticTacToe.c rockPaperScissors.c rockPaperScissorsController.c rockPaperScissorsViewer.c

# additional compilation flags
# "-Wall -Wextra -Werror -I . -std=c11 -Wno-unused-parameter" are already set
CFLAGS += -pedantic
CPPFLAGS += -pedantic -D __LCOM_OPTIMIZED__

# list of library dependencies (for Lab 2, only LCF library)
DPADD += ${LIBLCF}
LDADD += -llcf

# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk>
