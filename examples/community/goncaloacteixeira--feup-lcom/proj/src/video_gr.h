//
// Created by skidr on 14/11/2019.
//

#ifndef LAB5_VIDEO_GR_H
#define LAB5_VIDEO_GR_H

#include <lcom/lcf.h>

/**
 * @file video_gr.h
 * @brief Project Module with functions to manage the video card
 */

/** @defgroup video_gr video_gr
 * @{
 * Functions for video card management
 */

/**
 * @brief Function to read VBE controller info
 */
int vg_vbe_controller_info(struct vg_vbe_contr_info *vci_p);

/**
 * @brief GET Method for Horizontal Resolution
 */
int getHorResolution();

/**
 * @brief GET Method for Vertical Resolution
 */
int getVerResolution();

/**
 * @brief GET Method for Main Graphics Buffer
 */
char* getGraphicsBuffer();

/**
 * @brief GET Method for Auxiliar Buffer
 */
char* getAuxBuffer();

/**
 * @brief GET Method for VRAM Size
 */
unsigned int getVRAMSize();

/**
 * @brief Function to process double buffering
 */
int doubleBuffering();

/**
 * @brief Function to reset the auxiliar buffer
 */
int resetSecondBuffer();


#endif //LAB5_VIDEO_GR_H
