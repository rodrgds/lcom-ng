#include "scene.h"

#include "tetriminos.h"

#include "liblcom.h"

#include "interrupt.h"

#include "board.h"

#include "kbc.h"

#include "mouse.h"

#include "timer.h"

#include "MM640.h"
#include "GOS640.h"

#include "read_xpm.h"

void scene_init() {
    int_init();
    
    scene_initVideoGraphics();
    scene_initTimer();
    scene_initKeyboard();
    scene_initMouse();
    
    scene_setInterruptMode(kInterruptModeMainMenu);
    
    int_start_handler();
}

void scene_initGame(int players) {
    scene_cleanScreen();
    
    tetriminos_init();
    
    board_setPlayers(players);
    
    scene_setInterruptMode(kInterruptModeInGame);
}

void scene_endGame(int players, int sw) {
    scene_cleanScreen();
    
    if (players == 1)
        scene_drawEndGameSingle(sw);
    else
        scene_drawEndGameMulti(sw);
    
    scene_setInterruptMode(kInterruptModeEndGame);
}

void scene_terminate() {
    scene_intReset();
    scene_exitVideoGraphics();
    
    exit(0);
}

void scene_intReset() {
    int_unsubscribe(0);
    int_unsubscribe(1);
    int_unsubscribe(2);
}

void scene_setInterruptMode(kInterruptMode mode) {
    switch (mode) {
        case kInterruptModeMainMenu:
            scene_drawMainMenu();
            
            break;
            
        case kInterruptModeEndGame:
            //scene_drawEndGame();
            
            break;
              
        case kInterruptModeInGame:
            
            break;
            
        default:
            
            break;
    }
    
    int_setMode(mode);
}

int scene_drawMainMenu() {
    int h, v;
    
    if (vg_get_res(&h, &v))
        return 1;
    
    int width, height;
    
    char *map;
    
    map = (char*)read_xpm(MM640, &width, &height);
    
    int xcenter = (h - width) / 2;
    int ycenter = (v - height) / 2;
    
    int i,j;
    
    for (j = 0; j < height; j++)
        for (i = 0; i < width; i++, map++)
            vg_set_pixel(i + xcenter, j + ycenter, *map);
    
    return 0;
}

int scene_drawEndGame(int players) {
    int h, v;
    
    if (vg_get_res(&h, &v))
        return 1;
    
    int width, height;
    
    char *map;
    
    if (players == 1)
        map = (char*)read_xpm(GOS640, &width, &height);
    else
        map = (char*)read_xpm(GOS640, &width, &height);
    
    int xcenter = (h - width) / 2;
    int ycenter = (v - height) / 2;
    
    int i,j;
    
    for (j = 0; j < height; j++)
        for (i = 0; i < width; i++, map++)
            vg_set_pixel(i + xcenter, j + ycenter, *map);
    
    return 0;
}

void scene_drawEndGameSingle(int sw) {
    scene_drawEndGame(1);
    
    vg_draw_int(endGameScoreBounds.x, endGameScoreBounds.y, 7, sw);
}

void scene_drawEndGameMulti(int sw) {
    scene_drawEndGame(2);
    
    vg_draw_player(endGameScoreBounds.x, endGameScoreBounds.y, 7);
    vg_draw_number(endGameScoreBounds.x + 8 * 7, endGameScoreBounds.y, 7, sw);
}

void scene_initVideoGraphics() {
    /*
     *  Colors look weird in VMware on the first vg_init().
     *
     *  A good workaround is to do an init/exit/init cycle.
     */
    
    printf("\tEntering Video Mode (And coming back!)...\n");
    
    vg_init(kVideoMode);
    vg_exit();
    
    printf("\tEntering Video Mode (For real!)...\n");
    
    vg_init(kVideoMode);
}

int scene_initTimer() {
    return timer_subscribe();
}

int scene_initKeyboard() {
    return int_subscribe(KB_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &board_keyboardHandler);
}

int scene_initMouse() {
    int subscription = int_subscribe(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &board_mouseHandler);
    mouse_enable();
    return subscription;
}

int scene_cleanTimer() {
    return timer_unsubscribe();
}

int scene_cleanKeyboard() {
    return int_unsubscribe(1);
}

int scene_cleanMouse() {
    return int_unsubscribe(2);
}

void scene_drawNext(int players) {
    unsigned int i = 0, j = 0;
    
    rect pieceSize = _nextPieceRepSize();
    
    tetrimino next = tetriminos_next();
    
    unsigned int cb = 0;
    
    for (; cb < next.blocks; cb++) {
        if (players == 1) {
            pieceSize.x = nextPieceRect.x + pieceSize.w * next.coords[cb].y + pieceSize.w;
            pieceSize.y = nextPieceRect.y + pieceSize.h * next.coords[cb].x;
        } else {
            pieceSize.x = nextPieceRectMP.x + pieceSize.w * next.coords[cb].y + pieceSize.w;
            pieceSize.y = nextPieceRectMP.y + pieceSize.h * next.coords[cb].x;
        }
        
        int w = next.width;
        int h = next.ymax + 1;
        
        if (w == 1)
            pieceSize.x += pieceSize.w;
        
        if (h == 3 || h == 2)
            pieceSize.y += pieceSize.h;
        else if (h == 1)
            pieceSize.y += pieceSize.h * 2;
        
        if (h == 4) {
            pieceSize.x -= pieceSize.w;
            pieceSize.y += pieceSize.h;
        }
        
        _drawRect(pieceSize, next.color, 1);
    }
}

void scene_cleanScreen() {
    vg_fill(kColorBlack);
}

rect _boardPieceRepSize() {
    rect ret_rect = {.x = boardRect.x, .y = boardRect.y, .w = boardRect.w / kBoardMaxWidth, .h = boardRect.w / kBoardMaxWidth};
    
    return ret_rect;
}

rect _nextPieceRepSize() {
    rect ret_rect = {.x = nextPieceRect.x, .y = nextPieceRect.y, .w = boardRect.w / kBoardMaxWidth, .h = boardRect.w / kBoardMaxWidth};
    
    return ret_rect;
}

unsigned int _drawRect(rect toDraw, unsigned long color, unsigned int fill) {
    return vg_draw_rect(toDraw.x, toDraw.y, toDraw.x + toDraw.w, toDraw.y + toDraw.h, color, fill);
}

void scene_exitVideoGraphics() {
    vg_exit();
}
