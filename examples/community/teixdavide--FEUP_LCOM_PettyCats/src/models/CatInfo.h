/**
 * @file CatInfo.h
 * @brief This file contains information about the cats, encased in the struct CatInfo
 * @date 2024-06-02
 */

#ifndef _CATINFO_H_
#define _CATINFO_H_

#include <stdbool.h>
#include <lcom/lcf.h>

/**
 * @brief Possible cat colors
 * 
 */
enum CatColor {
    RED_CAT,
    BROWN_CAT,
    BLUE_CAT,
    GRAY_CAT,
    ORANGE_CAT
};
/**
 * @brief Cat Information struct
 * 
 */
typedef struct {
    enum CatColor color;
    bool isPet;
    uint8_t petCountdown;
    uint16_t moveCountdown;
    bool isIdle;
    bool isSelected;
} CatInfo;
/**
 * @brief Creates a new CatInfo with a random color
 * 
 * @return CatInfo* 
 */
CatInfo* newCatInfo();
/**
 * @brief Deletes a previously created cat info
 * 
 * @param catInfo catInfo to delete
 */
void deleteCatInfo(CatInfo* catInfo);

#endif
