#ifndef __VIDEO_GR_H
#define __VIDEO_GR_H

/**
 * @brief Initializes the video module in graphics mode
 * 
 * Uses the VBE INT 0x10 interface to set the desired
 *  graphics mode, maps VRAM to the process' address space and
 *  initializes static global variables with the resolution of the screen, 
 *  and the number of colors
 * 
 * @param mode 16-bit VBE mode to set
 * @return Virtual address VRAM was mapped to. NULL, upon failure.
 */
void *vg_init(unsigned short mode);

 /**
 * @brief Returns to default Minix 3 text mode (0x03: 25 x 80, 16 colors)
 * 
 * @return 0 upon success, non-zero upon failure
 */
int vg_exit(void);

/**
 * @return a pointer to the primary buffer
 */
char* getVideoMem();
/**
 * @return a pointer to the secondary buffer
 */
char* getSBuffer();
/**
 * @return the horizontal resolution
 */
unsigned getHRes();
/**
 * @return the vertical resolution
 */
unsigned getVRes();
/**
 * @brief cleans the main buffer
 */
void clear_screan();
/**
 * @brief updates the main buffer with the information that was on the secondary
 */
void updateBuffer();
/**
 * @brief cleans the secondary buffer
 */
void clear_double();

#endif /* __VIDEO_GR_H */
