#pragma once

#define BIT(n) (0x01 << n)
#define singleBit(byte) (0x01 & byte)
#define swap(type, i, j) {type t = i; i = j; j= t;}

/**
 * @brief Checks if a file exists.
 * @param filename Name of file.
 * @return 0 upon success and non-zero otherwise
 */
int fileExists(const char* filename);

/**
 * @brief Gets image path.
 * @param image Image name.
 * @return image path.
 */
const char* getImagePath(const char* image);
