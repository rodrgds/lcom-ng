#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <lcom/lcf.h>
#include "mouse.h"
#include "lc-graphics-main/includes/xpm.h"


#include "../../doc/bet/bet1.h"
#include "../../doc/bet/bet2.h"
#include "../../doc/bet/bet3.h"
#include "../../doc/bet/bet4.h"
#include "../../doc/bet/bet5.h"

#include "../../doc/menu/menu0.h"
#include "../../doc/menu/menu1.h"
#include "../../doc/menu/menu2.h"

#include "../../doc/playing/playing.h"
#include "../../doc/playing/playing1.h"
#include "../../doc/playing/playing2.h"
#include "../../doc/playing/playing3.h"

#include "../../doc/result/bjBet.h"
#include "../../doc/result/bjExit.h"
#include "../../doc/result/dealerBet.h"
#include "../../doc/result/dealerExit.h"
#include "../../doc/result/pushBet.h"
#include "../../doc/result/pushExit.h"
#include "../../doc/result/winBet.h"
#include "../../doc/result/winExit.h"

#include "../../doc/xpm/2C.h"
#include "../../doc/xpm/2D.h"
#include "../../doc/xpm/2H.h"
#include "../../doc/xpm/2S.h"
#include "../../doc/xpm/3C.h"
#include "../../doc/xpm/3D.h"
#include "../../doc/xpm/3H.h"
#include "../../doc/xpm/3S.h"
#include "../../doc/xpm/4C.h"
#include "../../doc/xpm/4D.h"
#include "../../doc/xpm/4H.h"
#include "../../doc/xpm/4S.h"
#include "../../doc/xpm/5C.h"
#include "../../doc/xpm/5D.h"
#include "../../doc/xpm/5H.h"
#include "../../doc/xpm/5S.h"
#include "../../doc/xpm/6C.h"
#include "../../doc/xpm/6D.h"
#include "../../doc/xpm/6H.h"
#include "../../doc/xpm/6S.h"
#include "../../doc/xpm/7C.h"
#include "../../doc/xpm/7D.h"
#include "../../doc/xpm/7H.h"
#include "../../doc/xpm/7S.h"
#include "../../doc/xpm/8C.h"
#include "../../doc/xpm/8D.h"
#include "../../doc/xpm/8H.h"
#include "../../doc/xpm/8S.h"
#include "../../doc/xpm/9C.h"
#include "../../doc/xpm/9D.h"
#include "../../doc/xpm/9H.h"
#include "../../doc/xpm/9S.h"
#include "../../doc/xpm/10C.h"
#include "../../doc/xpm/10D.h"
#include "../../doc/xpm/10H.h"
#include "../../doc/xpm/10S.h"
#include "../../doc/xpm/AC.h"
#include "../../doc/xpm/AD.h"
#include "../../doc/xpm/AH.h"
#include "../../doc/xpm/AS.h"
#include "../../doc/xpm/JC.h"
#include "../../doc/xpm/JD.h"
#include "../../doc/xpm/JH.h"
#include "../../doc/xpm/JS.h"
#include "../../doc/xpm/QC.h"
#include "../../doc/xpm/QD.h"
#include "../../doc/xpm/QH.h"
#include "../../doc/xpm/QS.h"
#include "../../doc/xpm/KC.h"
#include "../../doc/xpm/KD.h"
#include "../../doc/xpm/KH.h"
#include "../../doc/xpm/KS.h"
#include "../../doc/xpm/red_back.h"

#include "../../doc/numbers/_0.h"
#include "../../doc/numbers/_1.h"
#include "../../doc/numbers/_2.h"
#include "../../doc/numbers/_3.h"
#include "../../doc/numbers/_4.h"
#include "../../doc/numbers/_5.h"
#include "../../doc/numbers/_6.h"
#include "../../doc/numbers/_7.h"
#include "../../doc/numbers/_8.h"
#include "../../doc/numbers/_9.h"


#include "../../doc/cursor.h"

void (loadPixmap)();
void (loadCards)();
void (loadNumbers)();
void (loadCursor)();
int (graphics_init)();
int (graphics_exit)();
void (clear_screen)();
void (draw_mouse)();
int (draw_screen)();
void (draw_playing)();
void (draw_result)();
void (draw_card)(int index, int pos, int y_offset);
void (drawNumbers)(int number, int pos, int x_offset, int y_offset);
void (drawBalance)();
void (drawBet)();
void (drawScores)(int over);

#endif //GRAPHICS_H
