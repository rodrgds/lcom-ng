#ifndef PROJ_HUD_FONT_DATA_H
#define PROJ_HUD_FONT_DATA_H

#include "ninjix_platform.h"

#define HUD_GLYPH_XPM(name, row1, row2, row3, row4, row5, row6, row7) \
  static const xpm_row_t name[] = { \
    "5 7 2 1", \
    "  c None", \
    "# c #F2F4F8", \
    row1, row2, row3, row4, row5, row6, row7 \
  }

HUD_GLYPH_XPM(hud_glyph_space,
              "     ",
              "     ",
              "     ",
              "     ",
              "     ",
              "     ",
              "     ");

HUD_GLYPH_XPM(hud_glyph_0,
              " ### ",
              "#   #",
              "#  ##",
              "# # #",
              "##  #",
              "#   #",
              " ### ");

HUD_GLYPH_XPM(hud_glyph_1,
              "  #  ",
              " ##  ",
              "# #  ",
              "  #  ",
              "  #  ",
              "  #  ",
              "#####");

HUD_GLYPH_XPM(hud_glyph_2,
              " ### ",
              "#   #",
              "    #",
              "   # ",
              "  #  ",
              " #   ",
              "#####");

HUD_GLYPH_XPM(hud_glyph_3,
              " ### ",
              "#   #",
              "    #",
              " ### ",
              "    #",
              "#   #",
              " ### ");

HUD_GLYPH_XPM(hud_glyph_4,
              "   # ",
              "  ## ",
              " # # ",
              "#  # ",
              "#####",
              "   # ",
              "   # ");

HUD_GLYPH_XPM(hud_glyph_5,
              "#####",
              "#    ",
              "#    ",
              "#### ",
              "    #",
              "#   #",
              " ### ");

HUD_GLYPH_XPM(hud_glyph_6,
              " ### ",
              "#   #",
              "#    ",
              "#### ",
              "#   #",
              "#   #",
              " ### ");

HUD_GLYPH_XPM(hud_glyph_7,
              "#####",
              "    #",
              "   # ",
              "  #  ",
              " #   ",
              " #   ",
              " #   ");

HUD_GLYPH_XPM(hud_glyph_8,
              " ### ",
              "#   #",
              "#   #",
              " ### ",
              "#   #",
              "#   #",
              " ### ");

HUD_GLYPH_XPM(hud_glyph_9,
              " ### ",
              "#   #",
              "#   #",
              " ####",
              "    #",
              "#   #",
              " ### ");

HUD_GLYPH_XPM(hud_glyph_A,
              " ### ",
              "#   #",
              "#   #",
              "#####",
              "#   #",
              "#   #",
              "#   #");

HUD_GLYPH_XPM(hud_glyph_B,
              "#### ",
              "#   #",
              "#   #",
              "#### ",
              "#   #",
              "#   #",
              "#### ");

HUD_GLYPH_XPM(hud_glyph_C,
              " ### ",
              "#   #",
              "#    ",
              "#    ",
              "#    ",
              "#   #",
              " ### ");

HUD_GLYPH_XPM(hud_glyph_D,
              "#### ",
              "#   #",
              "#   #",
              "#   #",
              "#   #",
              "#   #",
              "#### ");

HUD_GLYPH_XPM(hud_glyph_E,
              "#####",
              "#    ",
              "#    ",
              "#### ",
              "#    ",
              "#    ",
              "#####");

HUD_GLYPH_XPM(hud_glyph_F,
              "#####",
              "#    ",
              "#    ",
              "#### ",
              "#    ",
              "#    ",
              "#    ");

HUD_GLYPH_XPM(hud_glyph_G,
              " ### ",
              "#   #",
              "#    ",
              "# ###",
              "#   #",
              "#   #",
              " ### ");

HUD_GLYPH_XPM(hud_glyph_H,
              "#   #",
              "#   #",
              "#   #",
              "#####",
              "#   #",
              "#   #",
              "#   #");

HUD_GLYPH_XPM(hud_glyph_I,
              "#####",
              "  #  ",
              "  #  ",
              "  #  ",
              "  #  ",
              "  #  ",
              "#####");

HUD_GLYPH_XPM(hud_glyph_J,
              "#####",
              "    #",
              "    #",
              "    #",
              "    #",
              "#   #",
              " ### ");

HUD_GLYPH_XPM(hud_glyph_K,
              "#   #",
              "#  # ",
              "# #  ",
              "##   ",
              "# #  ",
              "#  # ",
              "#   #");

HUD_GLYPH_XPM(hud_glyph_L,
              "#    ",
              "#    ",
              "#    ",
              "#    ",
              "#    ",
              "#    ",
              "#####");

HUD_GLYPH_XPM(hud_glyph_M,
              "#   #",
              "## ##",
              "# # #",
              "#   #",
              "#   #",
              "#   #",
              "#   #");

HUD_GLYPH_XPM(hud_glyph_N,
              "#   #",
              "##  #",
              "##  #",
              "# # #",
              "#  ##",
              "#  ##",
              "#   #");

HUD_GLYPH_XPM(hud_glyph_O,
              " ### ",
              "#   #",
              "#   #",
              "#   #",
              "#   #",
              "#   #",
              " ### ");

HUD_GLYPH_XPM(hud_glyph_P,
              "#### ",
              "#   #",
              "#   #",
              "#### ",
              "#    ",
              "#    ",
              "#    ");

HUD_GLYPH_XPM(hud_glyph_Q,
              " ### ",
              "#   #",
              "#   #",
              "#   #",
              "# # #",
              "#  # ",
              " ## #");

HUD_GLYPH_XPM(hud_glyph_R,
              "#### ",
              "#   #",
              "#   #",
              "#### ",
              "# #  ",
              "#  # ",
              "#   #");

HUD_GLYPH_XPM(hud_glyph_S,
              " ####",
              "#    ",
              "#    ",
              " ### ",
              "    #",
              "    #",
              "#### ");

HUD_GLYPH_XPM(hud_glyph_T,
              "#####",
              "  #  ",
              "  #  ",
              "  #  ",
              "  #  ",
              "  #  ",
              "  #  ");

HUD_GLYPH_XPM(hud_glyph_U,
              "#   #",
              "#   #",
              "#   #",
              "#   #",
              "#   #",
              "#   #",
              " ### ");

HUD_GLYPH_XPM(hud_glyph_V,
              "#   #",
              "#   #",
              "#   #",
              "#   #",
              "#   #",
              " # # ",
              "  #  ");

HUD_GLYPH_XPM(hud_glyph_W,
              "#   #",
              "#   #",
              "#   #",
              "#   #",
              "# # #",
              "## ##",
              "#   #");

HUD_GLYPH_XPM(hud_glyph_X,
              "#   #",
              "#   #",
              " # # ",
              "  #  ",
              " # # ",
              "#   #",
              "#   #");

HUD_GLYPH_XPM(hud_glyph_Y,
              "#   #",
              "#   #",
              " # # ",
              "  #  ",
              "  #  ",
              "  #  ",
              "  #  ");

HUD_GLYPH_XPM(hud_glyph_Z,
              "#####",
              "    #",
              "   # ",
              "  #  ",
              " #   ",
              "#    ",
              "#####");

HUD_GLYPH_XPM(hud_glyph_colon,
              "     ",
              "  #  ",
              "     ",
              "     ",
              "  #  ",
              "     ",
              "     ");

HUD_GLYPH_XPM(hud_glyph_dot,
              "     ",
              "     ",
              "     ",
              "     ",
              "     ",
              "  #  ",
              "  #  ");

HUD_GLYPH_XPM(hud_glyph_hyphen,
              "     ",
              "     ",
              "     ",
              " ### ",
              "     ",
              "     ",
              "     ");

HUD_GLYPH_XPM(hud_glyph_slash,
              "    #",
              "   # ",
              "   # ",
              "  #  ",
              " #   ",
              " #   ",
              "#    ");

HUD_GLYPH_XPM(hud_glyph_question,
              " ### ",
              "#   #",
              "    #",
              "   # ",
              "  #  ",
              "     ",
              "  #  ");

HUD_GLYPH_XPM(hud_glyph_plus,
              "     ",
              "  #  ",
              "  #  ",
              " ### ",
              "  #  ",
              "  #  ",
              "     ");

HUD_GLYPH_XPM(hud_glyph_gt,
              "     ",
              "##   ",
              " ##  ",
              "  ## ",
              " ##  ",
              "##   ",
              "#    ");
HUD_GLYPH_XPM(hud_glyph_lt,
              "     ",
              "   ##",
              "  ## ",
              " ##  ",
              "  ## ",
              "   ##",
              "     ");

#endif
