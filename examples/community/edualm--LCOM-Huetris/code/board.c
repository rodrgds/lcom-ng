/*  board rep:
 *
 *  |xxoxx|
 *  |xxxxx|
 *  +-----+
 *
 *  piece position: {1, 3}
 */

/*  Tetris Guideline: http://tetris.wikia.com/wiki/Tetris_Guideline */

#include "board.h"

#include "tetriminos.h"

#include "scene_sp.h"

#include "timer.h"

#include "kbc.h"

#include "mouse.h"

#include "scene.h"

#include <minix/bitmap.h>

#include <math.h>

#define ROT(x1,y1,x2,y2) rotated[x1][y1] = matrix[x2][y2]

#define false   0
#define true    1

#define kRotMatrixMaxSize 4

static unsigned int points = 0;

static unsigned int level = 1;

static unsigned int players = 1;

static unsigned int lines = 0;

static unsigned int scoreLines[] = {0, 100, 300, 500, 800};

static tetrimino cur_tetrimino = (tetrimino){.color = 0, .width = 0};

void board_init() {
    _cleanBoards();
}

void _cleanBoards() {
    int i, j;
    
    for (i = 0; i < kBoardMaxHeight; i++) {
        for (j = 0; j < kBoardMaxWidth; j++) {
            board[i][j] = 0;
            board_p2[i][j] = 0;
        }
    }
    
    points = 0;
}

void _pushValuesDown(kBoard b, unsigned int line) {
	unsigned int i = 0;
    
	for (; line < (kBoardMaxHeight - 1); line++)
		for (i = 0; i < kBoardMaxWidth; i++) {
            if (b == kBoardOne)
                board[line][i] = board[line + 1][i];
            else
                board_p2[line][i] = board_p2[line + 1][i];
        }
}

void board_setPlayers(int pl) {
    players = (pl > 1 ? 2 : 1);
}

unsigned int board_clearLines(kBoard b) {
    unsigned int ret = 0, i = 0;
    
    for (; i < kBoardMaxHeight; i++) {
        unsigned int nFound = 1, j = 0;
        
        for (; j < kBoardMaxWidth; j++) {
            if (b == kBoardOne) {
                if (!board[i][j]) {
                    nFound = 0;
                    
                    break;
                }
            } else {
                if (!board_p2[i][j]) {
                    nFound = 0;
                    
                    break;
                }
            }
        }
        
        if (!nFound)
            continue;
        
        _pushValuesDown(b, i);
        
        ret++;
        i--;
    }
    
    points += scoreLines[ret] * level / 10;
    
    for (; ret > 0; ret--) {
        lines++;
        
        if (lines % 5)
            level++;
    }
    
    vg_draw_int(scoreBounds.x, scoreBounds.y, 7, points);
    
    return ret;
}

unsigned int board_pieceInGame(kBoard b) {
	unsigned int i = 0, j = 0;
    
    for (; i < kBoardMaxHeight; i++) {
        for (j = 0; j < kBoardMaxWidth; j++) {
            if (b == kBoardOne) {
                if (board[i][j] == kPieceTypeEnabled)
                    return 1;
            } else {
                if (board_p2[i][j] == kPieceTypeEnabled)
                    return 1;
            }
        }
    }
    
    return 0;
}

unsigned int board_movePiece(kBoard b, kMovePos pos) {
    if (board_pieceInGame(b)) {
        coordinate pieceCoords[kMaxPieceSize];
        unsigned int curcoor = 0, i = 0, j = 0;
        
        for (; i < kBoardMaxHeight; i++)
            for (j = 0; j < kBoardMaxWidth; j++) {
                if (b == kBoardOne) {
                    if (board[i][j] == kPieceTypeEnabled) {
                        coordinate crd;
                        
                        crd.x = j;
                        crd.y = i;
                        
                        pieceCoords[curcoor] = crd;
                        curcoor++;
                    }
                } else {
                    if (board_p2[i][j] == kPieceTypeEnabled) {
                        coordinate crd;
                        
                        crd.x = j;
                        crd.y = i;
                        
                        pieceCoords[curcoor] = crd;
                        curcoor++;
                    }
                }
            }
        
        printf("curcoor: %d\n", curcoor);
        
        for (i = 0; i < curcoor; i++) {
            coordinate crd = pieceCoords[i];
            
            switch (pos) {
                case kMovePosLeft:
                    if (crd.x <= 0)
                        return 0;
                    
                    crd.x -= 1;
                    
                    break;
                    
                case kMovePosDown:
                    if (crd.y <= 0) {
                        return 0;
                    }
                    
                    crd.y -= 1;
                    
                    break;
                    
                case kMovePosRight:
                    if (crd.x >= (kBoardMaxWidth - 1))
                        return 0;
                    
                    crd.x += 1;
                    
                    break;
                    
                default:
                    return 0;
                    
                    break;
            }
            
            if (b == kBoardOne) {
                if (board[crd.y][crd.x] && board[crd.y][crd.x] != kPieceTypeEnabled)
                    return 0;
            } else {
                if (board_p2[crd.y][crd.x] && board_p2[crd.y][crd.x] != kPieceTypeEnabled)
                    return 0;
            }
            
            
            pieceCoords[i] = crd;
        }
        
        if (b == kBoardOne) {
            for (i = 0; i < kBoardMaxHeight; i++)
                for (j = 0; j < kBoardMaxWidth; j++)
                    if (board[i][j] == kPieceTypeEnabled)
                        board[i][j] = 0;
        } else {
            for (i = 0; i < kBoardMaxHeight; i++)
                for (j = 0; j < kBoardMaxWidth; j++)
                    if (board_p2[i][j] == kPieceTypeEnabled)
                        board_p2[i][j] = 0;
        }
        
        
        if (b == kBoardOne) {
            for (i = 0; i < curcoor; i++)
                board[pieceCoords[i].y][pieceCoords[i].x] = kPieceTypeEnabled;
        } else {
            for (i = 0; i < curcoor; i++)
                board_p2[pieceCoords[i].y][pieceCoords[i].x] = kPieceTypeEnabled;
        }
        
        return 1;
    }
    
    return 0;
}

unsigned int board_gluePiece(kBoard b) {
    unsigned int glued = 0, i = 0, j = 0;
    
    if (b == kBoardOne) {
        for (; i < kBoardMaxHeight; i++)
            for (j = 0; j < kBoardMaxWidth; j++)
                if (board[i][j] == kPieceTypeEnabled) {
                    board[i][j] = cur_tetrimino.color;
                    
                    glued++;
                }
    } else {
        for (; i < kBoardMaxHeight; i++)
            for (j = 0; j < kBoardMaxWidth; j++)
                if (board_p2[i][j] == kPieceTypeEnabled) {
                    board_p2[i][j] = cur_tetrimino.color;
                    
                    glued++;
                }
    }
    
    return glued;
}

unsigned int board_spawnPiece(kBoard b) {
    cur_tetrimino = tetriminos_get();
    
    printf("%d, %d\n", cur_tetrimino.color, cur_tetrimino.width);
    
    unsigned int cb = 0;
    
    unsigned int i = 0, j = 0;
    
    points += level + 1;
    
    //vg_draw_int(scoreBounds.x, scoreBounds.y, 7, points);
    
    for (; cb < cur_tetrimino.blocks; cb++) {
        cur_tetrimino.coords[cb].x += 5;
        cur_tetrimino.coords[cb].y = kBoardMaxHeight - (cur_tetrimino.ymax - cur_tetrimino.coords[cb].y);
    }
    
    if (b == kBoardOne) {
        for (cb = 0; cb < cur_tetrimino.blocks; cb++) {
            if (board[cur_tetrimino.coords[cb].y - 1][cur_tetrimino.coords[cb].x - 1]) {
                if (players == 1)
                    scene_endGame(players, points);
                else
                    scene_endGame(players, 2);
                
                return 1;
            }
            
            board[cur_tetrimino.coords[cb].y - 1][cur_tetrimino.coords[cb].x - 1] = kPieceTypeEnabled;
        }
    } else {
        for (cb = 0; cb < cur_tetrimino.blocks; cb++) {
            if (board_p2[cur_tetrimino.coords[cb].y - 1][cur_tetrimino.coords[cb].x - 1]) {
                scene_endGame(players, 1);
                
                return 1;
            }
            
            board_p2[cur_tetrimino.coords[cb].y - 1][cur_tetrimino.coords[cb].x - 1] = kPieceTypeEnabled;
        }
    }
    
    return 0;
}

unsigned int board_rotatePiece(kBoard b) {
    if (!board_pieceInGame(b))
        return 1;
    
    coordinate pieceCoords[kMaxPieceSize], min_crd = {.x = 64, .y = 64};
    unsigned int curcoor = 0, i = 0, j = 0;
    
    if (b == kBoardOne) {
        for (; i < kBoardMaxHeight; i++)
            for (j = 0; j < kBoardMaxWidth; j++)
                if (board[i][j] == kPieceTypeEnabled) {
                    coordinate crd;
                    
                    crd.x = j;
                    crd.y = i;
                    
                    if (i > 20)
                        return 1;
                    
                    if (min_crd.x > crd.x)
                        min_crd.x = crd.x;
                    
                    if (min_crd.y > crd.y)
                        min_crd.y = crd.y;
                    
                    pieceCoords[curcoor] = crd;
                    curcoor++;
                }
    } else {
        for (; i < kBoardMaxHeight; i++)
            for (j = 0; j < kBoardMaxWidth; j++)
                if (board_p2[i][j] == kPieceTypeEnabled) {
                    coordinate crd;
                    
                    crd.x = j;
                    crd.y = i;
                    
                    if (i > 20)
                        return 1;
                    
                    if (min_crd.x > crd.x)
                        min_crd.x = crd.x;
                    
                    if (min_crd.y > crd.y)
                        min_crd.y = crd.y;
                    
                    pieceCoords[curcoor] = crd;
                    curcoor++;
                }
    }
    
    unsigned int matrix[kRotMatrixMaxSize][kRotMatrixMaxSize] = {{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}};
    unsigned int rotated[kRotMatrixMaxSize][kRotMatrixMaxSize] = {{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}};
    
    for (i = 0; i < curcoor; i++) {
        printf("beg_pc: %d %d\n", pieceCoords[i].x, pieceCoords[i].y);
        
        pieceCoords[i].x -= min_crd.x;
        pieceCoords[i].y -= min_crd.y;
        
        matrix[pieceCoords[i].y][pieceCoords[i].x] = 1;
    }
    
    unsigned int low, high;
    
    for (i = 0; i < 4; i++)
        for (low = 0, high = 3; low < 4; low++, high--)
            rotated[low][i] = matrix[i][high];
    
    coordinate newCoords[kMaxPieceSize];
    
    curcoor = 0;
    
    if (b == kBoardOne) {
        for (i = 0; i < kRotMatrixMaxSize; i++)
            for (j = 0; j < kRotMatrixMaxSize; j++)
                if (rotated[i][j] == 1) {
                    coordinate crd = {.x = j + min_crd.x, .y = i + min_crd.y};
                    
                    if (board[crd.y][crd.x] && board[crd.y][crd.x] != kPieceTypeEnabled)
                        return 1;
                    
                    newCoords[curcoor] = crd;
                    
                    curcoor++;
                }
        
        for (i = 0; i < kBoardMaxHeight; i++)
            for (j = 0; j < kBoardMaxWidth; j++)
                if (board[i][j] == kPieceTypeEnabled)
                    board[i][j] = 0;
        
        for (i = 0; i < curcoor; i++)
            board[newCoords[i].y][newCoords[i].x] = kPieceTypeEnabled;
    } else {
        for (i = 0; i < kRotMatrixMaxSize; i++)
            for (j = 0; j < kRotMatrixMaxSize; j++)
                if (rotated[i][j] == 1) {
                    coordinate crd = {.x = j + min_crd.x, .y = i + min_crd.y};
                    
                    if (board_p2[crd.y][crd.x] && board_p2[crd.y][crd.x] != kPieceTypeEnabled)
                        return 1;
                    
                    newCoords[curcoor] = crd;
                    
                    curcoor++;
                }
        
        for (i = 0; i < kBoardMaxHeight; i++)
            for (j = 0; j < kBoardMaxWidth; j++)
                if (board_p2[i][j] == kPieceTypeEnabled)
                    board_p2[i][j] = 0;
        
        for (i = 0; i < curcoor; i++)
            board_p2[newCoords[i].y][newCoords[i].x] = kPieceTypeEnabled;
    }
    
    if (b == kBoardOne) {
        for (; i < kBoardMaxHeight; i++)
            for (j = 0; j < kBoardMaxWidth; j++)
                if (board[i][j] == kPieceTypeEnabled) {
                    coordinate crd;
                    
                    crd.x = j;
                    crd.y = i;
                    
                    if (min_crd.y < crd.y) {
                        board_movePiece(kBoardOne, kMovePosDown);
                        
                        i = 0;
                        j = 0;
                        
                        continue;
                    }
                }
    } else {
        for (; i < kBoardMaxHeight; i++)
            for (j = 0; j < kBoardMaxWidth; j++)
                if (board_p2[i][j] == kPieceTypeEnabled) {
                    coordinate crd;
                    
                    crd.x = j;
                    crd.y = i;
                    
                    if (min_crd.y < crd.y) {
                        board_movePiece(kBoardTwo, kMovePosDown);
                        
                        i = 0;
                        j = 0;
                        
                        continue;
                    }
                }
    }
    
    return 0;
}

void board_timerHandler() {
    if (players) {
        if (!board_pieceInGame(kBoardOne)) {
            board_spawnPiece(kBoardOne);
            
            return;
        }
        
        if (!board_movePiece(kBoardOne, kMovePosDown)) {
            board_gluePiece(kBoardOne);
            
            board_clearLines(kBoardOne);
        }
        
        if (players == 2) {
            if (!board_pieceInGame(kBoardTwo)) {
                board_spawnPiece(kBoardTwo);
                
                return;
            }
            
            if (!board_movePiece(kBoardTwo, kMovePosDown)) {
                board_gluePiece(kBoardTwo);
                
                board_clearLines(kBoardTwo);
            }
        }
        
        scene_cleanScreen();
        
        if (players == 1) {
            sceneSingle_drawControls();
            
            sceneSingle_drawBoard();
        } else {
            sceneMultiple_drawControls();
            
            sceneMultiple_drawBoard();
        }
    }
}

void board_keyboardHandler() {
    unsigned long kb_read = keyboard_read();
	if (board_pieceInGame(kBoardOne)) {
        
		switch (kb_read) {
			case kKeyboardKeyUpArrow:
                printf("move piece up\n");
				
				board_rotatePiece(kBoardOne);
                
				break;
                
			case kKeyboardKeyLeftArrow:
				printf("move piece left\n");
				
				board_movePiece(kBoardOne, kMovePosLeft);
                
				break;
                
			case kKeyboardKeyRightArrow:
				printf("move piece right\n");
				
				board_movePiece(kBoardOne, kMovePosRight);
                
				break;
                
			case kKeyboardKeyDownArrow:
				printf("move piece down\n");
				
				board_movePiece(kBoardOne, kMovePosDown);
                
				break;
                
            case kKeyboardKeySpaceBar:
                while (true)
                    if (!board_movePiece(kBoardOne, kMovePosDown))
                        break;
                
                break;
                
			case kKeyboardKeyEsc:
				vg_exit();
                
                exit(0);
                
				break;
                
			default:
                /*if (kb_read != 0x80 | kKeyboardKeyUpArrow ||
                    kb_read != 0x80 | kKeyboardKeyDownArrow ||
                    kb_read != 0x80 | kKeyboardKeyRightArrow ||
                    kb_read != 0x80 | kKeyboardKeyLeftArrow)
                    vg_exit();*/
                printf("key pressed: 0x%X\n", kb_read);
                
				break;
		}
        
        scene_cleanScreen();
        
        if (players == 1) {
            sceneSingle_drawControls();
            
            sceneSingle_drawBoard();
        } else {
            sceneMultiple_drawControls();
            
            sceneMultiple_drawBoard();
        }
	}
}

static int _mplasty = 0;
static int _pb254 = 0;

void board_mouseHandler() {
    unsigned long rcv_data;
    mouse_read(&rcv_data);
    if (players == 2) {
        static unsigned char full_pack[] = { 0, 0, 0 };
        static int counter = 0;
        
        full_pack[counter] = rcv_data;
        
        if (!counter && !bit_isset(full_pack[0], 3))
            return;
        
        if (counter != 2) {
            counter++;
            
            return;
        }
        
        counter = 0;
        
        unsigned int needs_redraw = 0;
        
        if (bit_isset(full_pack[0], 0)) {
            board_movePiece(kBoardTwo, kMovePosLeft);
            
            needs_redraw = 1;
        } else if (bit_isset(full_pack[0], 1)) {
            board_movePiece(kBoardTwo, kMovePosRight);
            
            needs_redraw = 1;
        } else if (bit_isset(full_pack[0], 2)) {
            board_rotatePiece(kBoardTwo);
            
            needs_redraw = 1;
        } else {
            printf("Mouse Y: %04d\n", full_pack[2]);
            
            if (full_pack[2] > 50)
                _pb254 = 1;
            else
                _pb254 = 0;
            
            if (full_pack[2] < _mplasty) {
                if ((full_pack[2] < 200 && full_pack[2] > 80) && _pb254) {
                    while (true)
                        if (!board_movePiece(kBoardTwo, kMovePosDown))
                            break;
                    
                    printf("should push down.");
                    
                    _pb254 = 0;
                    
                    needs_redraw = 1;
                }
            }
            
            _mplasty = full_pack[2];
        }
        
        if (needs_redraw) {
            scene_cleanScreen();
            
            sceneMultiple_drawControls();
            
            sceneMultiple_drawBoard();
        }
    }
}

unsigned int board_getScore(int *sc) {
    *sc = points;
    
    return points;
}

void _echoBoard() {
	unsigned int i,j;
	for (i = 0; i < kBoardMaxHeight; i++) {
        for (j = 0; j < kBoardMaxWidth; j++) {
            printf("%d", board[i][j]);
        }
        
        printf("\n");
    }
}

unsigned int board_getColorAt(kBoard b, unsigned int x, unsigned int y) {
    if (b == kBoardOne)
        return board[y][x];
    
    return board_p2[y][x];
}
