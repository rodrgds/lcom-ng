#include <lcom/lcf.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include "images/background.xpm"
#include "images/red1.xpm"
#include "images/blue1.xpm"
#include "images/green1.xpm"
#include "images/yellow1.xpm"
#include "images/white1.xpm"
#include "images/bluePlayer.xpm"
#include "images/redPlayer.xpm"
#include "images/yellowPlayer.xpm"
#include "images/greenPlayer.xpm"
#include "images/start.xpm"
#include "images/startS.xpm"
#include "images/stats.xpm"
#include "images/statsS.xpm"
#include "images/help.xpm"
#include "images/helpS.xpm"
#include "images/exit.xpm"
#include "images/exitS.xpm"
#include "images/0.xpm"
#include "images/1.xpm"
#include "images/2.xpm"
#include "images/3.xpm"
#include "images/4.xpm"
#include "images/5.xpm"
#include "images/6.xpm"
#include "images/7.xpm"
#include "images/8.xpm"
#include "images/9.xpm"
#include "images/doispontos.xpm"
#include "images/cursor.xpm"
#include "images/lever1.xpm"
#include "images/lever2.xpm"
#include "images/lever3.xpm"
#include "images/lever4.xpm"
#include "images/lever5.xpm"
#include "images/slots.xpm"
#include "images/helpMenu1.xpm"
#include "images/redHeart.xpm"
#include "images/blueHeart.xpm"
#include "images/greenHeart.xpm"
#include "images/yellowHeart.xpm"
#include "images/buyMenu.xpm"
#include "images/buyYes.xpm"
#include "images/buyNo.xpm"
#include "images/gameOver.xpm"
#include "images/resumeButton.xpm"
#include "images/pauseMenu.xpm"
#include "images/menuButton.xpm"
#include "images/menuButtonOrange.xpm"
#include "images/resumeButtonOrange.xpm"
#include "images/menu.xpm"
#include "images/youWon.xpm"
/** @defgroup game GeneralGame
 * @{
 * Setting up pixmaps required for the game and random generator function
 */

xpm_map_t bkgrd = background;
xpm_map_t menuBkgrd = menuXPM;
xpm_map_t menuHelp = helpMenu_xpm;
xpm_map_t whiteSquare = whiteSqr;
xpm_map_t redSquare = redSqr;
xpm_map_t blueSquare = blueSqr;
xpm_map_t greenSquare = greenSqr;
xpm_map_t yellowSquare = yellowSqr;
xpm_map_t GmOver = gameOver;
xpm_map_t lever1 = lever1_xpm;
xpm_map_t lever2 = lever2_xpm;
xpm_map_t lever3 = lever3_xpm;
xpm_map_t lever4 = lever4_xpm;
xpm_map_t lever5 = lever5_xpm;
xpm_map_t slotsbg = slots_xpm;
xpm_map_t redPlr = redPlayer;
xpm_map_t bluePlr = bluePlayer;
xpm_map_t yellowPlr = yellowPlayer;
xpm_map_t greenPlr = greenPlayer;
xpm_map_t redHrt = redHeart;
xpm_map_t blueHrt = blueHeart;
xpm_map_t greenHrt = greenHeart;
xpm_map_t yellowHrt = yellowHeart;
xpm_map_t startSel = startS;
xpm_map_t startNorm = start;
xpm_map_t helpNorm = help;
xpm_map_t helpSel = helpS;
xpm_map_t exitNorm = exitXPM;
xpm_map_t exitSel = exitS;
xpm_map_t menuBtn = menuButton;
xpm_map_t pauseM = pauseMenu;
xpm_map_t resumeBtn = resumeButton;
xpm_map_t menuBtnOr = menuButtonOr;
xpm_map_t resumeBtnOr = resumeButtonOr;
xpm_map_t zero = zero_xpm;
xpm_map_t um = um_xpm;
xpm_map_t two = two_xpm;
xpm_map_t tres = tres_xpm;
xpm_map_t four = four_xpm;
xpm_map_t five = five_xpm;
xpm_map_t six = six_xpm;
xpm_map_t sete = sete_xpm;
xpm_map_t oito = oito_xpm;
xpm_map_t nove = nove_xpm;
xpm_map_t del = separator_xpm;
xpm_map_t MenuBuy = buyMenu;
xpm_map_t MenuYes = yesBuy;
xpm_map_t MenuNo = noBuy;
xpm_map_t Victory = victory_XPM;

/**
 * @brief Generates a random number
 * @param min Minimum number to generate
 * @param max Maximum number to generate
 * @return Generated number
 */
int (RNG)(uint8_t min, uint8_t max){
    srand(time(0));
    return (rand()%(max-min+1))+min;
}
/** @} end of GeneralGame */

