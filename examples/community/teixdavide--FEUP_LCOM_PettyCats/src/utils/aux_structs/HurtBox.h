/**
 * @file HurtBox.h
 * @brief This file contains a struct and 2 simple functions to create and delete hurt boxes
 * @date 2024-06-02
 * 
 * 
 */
#ifndef _HURTBOX_H_
#define _HURTBOX_H_

#include <stdint.h>
/**
 * @brief HurtBox struct
 * 
 */
typedef struct {
    uint16_t deltaX;
    uint16_t deltaY;
} HurtBox;

/**
 * @brief Creates a new hurtbox with width deltaX and height deltaY, counting from top left corner
 * 
 * @param deltaX width
 * @param deltaY height
 * @return pointer to hurtbox
 */
HurtBox *newHurtBox(uint16_t deltaX, uint16_t deltaY);
/**
 * @brief Frees memory allocated to previously created hurt box
 * 
 * @param hurtBox hurtBox to delete
 */
void deleteHurtBox(HurtBox* hurtBox);



#endif
