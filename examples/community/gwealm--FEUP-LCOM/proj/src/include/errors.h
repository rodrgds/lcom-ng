#pragma once

#include <lcom/lcf.h>

#define OKAY (0)
#define VEC_EDEST (-1)
#define VEC_EALLOC (-2)
#define VEC_EBOUNDS (-3)

#define KBC_EREAD (-1)
#define KBC_EWRITE (-2)
#define KBC_EPORT (-3)
#define KBC_ETIMEOUT (-4)
#define KBC_EACK (-5)
#define KBC_EVALIDITY (-6)

#define VBE_EMODE (-1)
#define VBE_EALLOC (-2)
#define VBE_EMAP (-3)
#define VBE_EFUNC (-4)
#define VBE_EBOUNDS (-5)

#define MOUSE_EINT (-1)
#define MOUSE_EBOUNDS (-2)
#define KBD_EINT (-1)

#define GRAPHICS_ELOAD (-1)

#define TIMER_EINT (-1)
#define TIMER_EREAD (-2)
#define TIMER_EWRITE (-3)

#define GAME_ELOAD (-1)
#define GAME_ESTATE (-2)
