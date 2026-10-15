#ifndef _MACROS_H_
#define _MACROS_H_

#define R(n) (0xFF & (n >> 16))
#define G(n) (0xFF & (n >>  8))
#define B(n) (0xFF & n)
#define SET_R(n) ((n&0xFF) << 16)
#define SET_G(n) ((n&0xFF) << 8)
#define SET_B(n) ((n&0xFF))
#define SET_COLOR(r,g,b) (SET_R(r) | SET_G(g) | SET_B(b))

// Colors
#define BLACK               0x00

#define OK 0

#endif /* _MACROS_H_ */
