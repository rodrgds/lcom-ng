#pragma once
#include <stdio.h>

#define NEWLINE printf("\n");

#define BIT(n) (0x01 << n)
#define singleBit(byte) (0x01 & byte)
#define swap(type, i, j) { type t = i; i = j; j = t;}

int fileExists(const char* filename);
const char* path(const char* name);

void writeScore(FILE* fp, int score);
