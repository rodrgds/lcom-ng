/**
 * @file video.h
 * @brief Header file for video graphics options. Contains and VBE register values and mode settings.
 */

#ifndef _LCOM_VIDEO_H_
#define _LCOM_VIDEO_H_

/* VBE register values */
#define SET_AH_GRAPHIC 0x4F
#define SET_AL_GRAPHIC 0x02
#define SET_LINEAR_MODE BIT(14)
#define ITNO 0x10

#define DIRECT_COLOR_MODE 0x06

#define MODE_105 0x105
#define MODE_110 0x110
#define MODE_115 0x115
#define MODE_11A 0x11A
#define MODE_14C 0x14C

#endif /* _LCOM_VIDEO_H_ */
