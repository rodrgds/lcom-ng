#ifndef _LCOM_GPU_H_
#define _LCOM_GPU_H_

#include <lcom/lcf.h>
#include <stdint.h>
#include <stdio.h>

/// @brief Sets the MINIX mode to text.
/// @return Returns 0 on success and 1 otherwise.
int set_text_mode();

/// @brief Sets the MINIX graphic mode.
/// @param submode The graphic mode to change to.
/// @return Returns 0 on success and 1 otherwise.
int set_graphic_mode(uint16_t submode);

/// @brief Inicializes the frame buffer to be used by MINIX.
/// @param mode The graphic mode witch the frame buffer is to.
/// @return Returns 0 on success and 1 otherwise. 
int set_frame_buffer(uint16_t mode);

/// @brief Draws the content stored in the frame buffer.
/// @return Returns 0 on success and 1 otherwise.
int draw_frame_buffer();

#endif
