#include <lcom/lcf.h>
#include "board.h"
#include "video.h"

int const FIRST_CELL_X = 205;
int const FIRST_CELL_Y = 201;
int const CELL_WIDTH = 59;
int const CELL_HEIGHT = 63;
uint16_t CELL_COLOR = 0x001f;
uint16_t WHITE_CELL = 0xffff;

void startBoard(){
    for (unsigned i = 0; i < 12; i++)
        for (unsigned j = 0; j < 12; j++)
            boardArr[i][j] = 0;
}

void updateBoard(int seed){

    //srand(time(NULL));
    for (unsigned i = 11; i > 0; i--)
        for (unsigned j = 0; j < 12; j++)
            boardArr[i][j] = boardArr[i - 1][j];
    
    int newSqs = (rand() % (seed)) % 12;
    printf("number %d", newSqs);

    for (int i = 0; i < newSqs; i++){
        int col = (rand() % (seed)) % 12;
        printf("position %d", col);
        boardArr[0][col] = 1;
    }
}

void printBoard(){
    for (unsigned i = 0; i < 12; i++)
        for (unsigned j = 0; j < 12; j++)
            printf("%d", boardArr[i][j]);
}

bool emptyCell(int x, int y){
    y = y + 63;
    if (x < 175 || x > 870 || y < 180 || y > 925)
        return false;
    if (getpixel(x, y) > 0xdddd)
        return true;
    return false;
}

void paintBoardCell(int x, int y, uint16_t color){
    if (getpixel(x,y) > 0xdddd){
        changePixel(x,y, color);
        paintBoardCell(x+1, y, color);
        paintBoardCell(x, y+1, color);
        paintBoardCell(x-1, y, color);
        paintBoardCell(x, y-1, color);
    }
}

void clearBoardCell(int x, int y, uint16_t oldColor, uint16_t color){
    if (getpixel(x,y) == oldColor){
        changePixel(x,y, color);
        clearBoardCell(x+1, y, oldColor, color);
        clearBoardCell(x, y+1, oldColor, color);
        clearBoardCell(x-1, y, oldColor, color);
        clearBoardCell(x, y-1, oldColor, color);
    }
}

void paintBoard(){
    for (unsigned i = 0; i < 12; i++)
        for (unsigned j = 0; j < 12; j++){
            if (boardArr[i][j]){
                uint16_t cellColor = getpixel(FIRST_CELL_X + j * CELL_WIDTH, FIRST_CELL_Y + i * CELL_HEIGHT);
                if (cellColor == 0xffff)
                    paintBoardCell(FIRST_CELL_X + j * CELL_WIDTH, FIRST_CELL_Y + i * CELL_HEIGHT, CELL_COLOR);
                
                else
                {
                    clearBoardCell(FIRST_CELL_X + j * CELL_WIDTH, FIRST_CELL_Y + i * CELL_HEIGHT, CELL_COLOR, WHITE_CELL);
                }
                
            }
        }
}
