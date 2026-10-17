#include "liblcom.h"

#include "board.h"

#include "tetriminos.h"

#include "scene.h"
#include "scene_mp.h"

void sceneMultiple_drawControls() {
    _drawRect(boardRect, 7, 0);
    
    rect br2 = boardRect;

    unsigned int times_to_run = 3, x = 0;

    for (; x < times_to_run; x++) {
    	br2.x -= 1;
        br2.y -= 1;
    	br2.w += 2;
        br2.h += 2;

    	_drawRect(br2, 7, 0);
    }
    
    _drawRect(boardRectSecond, 7, 0);
    
    br2 = boardRectSecond;
    
    x = 0;
    
    for (; x < times_to_run; x++) {
    	br2.x -= 1;
        br2.y -= 1;
    	br2.w += 2;
        br2.h += 2;
        
    	_drawRect(br2, 7, 0);
    }
    
    _drawRect(nextPieceRectMP, 7, 0);

    //  To Be Completed (with scores and crap)
}

void sceneMultiple_drawBoard() {
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
            
            if (board_getColorAt(kBoardTwo, j, i)) {
                pieceSize.x = boardRectSecond.x + pieceSize.w * (j - 1) +
                pieceSize.w + 2;
                pieceSize.y = boardRectSecond.y + pieceSize.h *
                ((kBoardMaxHeight - 2) - i) - pieceSize.h + 6;
                
                _drawRect(pieceSize, board_getColorAt(kBoardTwo, j, i), 1);
            }
        }
    }
    
    scene_drawNext(2);
}
