#pragma once

int boardArr[12][12];

void startBoard();

void updateBoard(int seed);

void printBoard();

bool emptyCell(int x, int y);

void paintBoardCell(int x, int y, uint16_t color);

void clearBoardCell(int x, int y, uint16_t oldColor, uint16_t color);

void paintBoard();
