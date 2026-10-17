#include "liblcom.h"

#include "board.h"

#include "tetriminos.h"

#include "scene.h"

#include "scene_sp.h"

#include "video_gr.h"

void sceneSingle_drawControls() {
    _drawRect(boardRect, 7/*white*/, 0);
    
    rect br2 = boardRect;

    unsigned int times_to_run = 3, x = 0;

    for (; x < times_to_run; x++) {
    	br2.x -= 1;
        br2.y -= 1;
    	br2.w += 2;
        br2.h += 2;

    	_drawRect(br2, 7, 0);
    }
    
    _drawRect(nextPieceRect, 7, 0);
    _drawRect(scoreRect, 7, 0);
    _drawRect(currTimeRect, 7, 0);
}

void sceneSingle_drawBoard() {
    unsigned int i = 0, j = 0;

    rect pieceSize = _boardPieceRepSize();
    
    for (; i < (kBoardMaxHeight - 2); i++) {
        for (j = 0; j < kBoardMaxWidth; j++) {
            if (board_getColorAt(kBoardOne, j, i)) {
                pieceSize.x = boardRect.x + pieceSize.w * (j - 1) + 
pieceSize.w + 2;
                pieceSize.y = boardRect.y + pieceSize.h * 
((kBoardMaxHeight - 2) - i) - pieceSize.h + 6;

                _drawRect(pieceSize, board_getColorAt(kBoardOne, j, i), 1);
            }
        }
    }
    
    int sc;
    
    board_getScore(&sc);
    
    vg_draw_int(scoreBounds.x, scoreBounds.y, 7, sc);
    
    scene_drawNext(1);
    sceneSingle_drawRTC();
}

void sceneSingle_drawRTC() {
    int h, m, s;
    
    rtc_get_time(&h, &m, &s);
    
    vg_draw_time(timeBounds.x, timeBounds.y, 7, h, m, s);
}
