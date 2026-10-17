#ifndef _PIXMAP_H_
#define _PIXMAP_H_
#include <lcom/lcf.h>
#include <stdint.h>
#include "pixmap.h"

const char *const solid_snake_segment[] = {
    "10 10 1", /* width height num_colors */
    "g 2",     /* green */
    "gggggggggg",
    "gggggggggg",
    "gggggggggg",
    "gggggggggg",
    "gggggggggg",
    "gggggggggg",
    "gggggggggg",
    "gggggggggg",
    "gggggggggg",
    "gggggggggg"
};

const char *const apple[] = {
    "18 18 6 1", /* width height num_colors */
    ". c #000000",     /* Black (background/outline) */
    "r c #FF0000",     /* Red (base apple color) */
    "D c #CC0000",     /* Dark Red (shadows) */
    "L c #FF6666",     /* Light Red (highlights) */
    "B c #8B4513",     /* Brown (stem) */
    "G c #008000",     /* Green (leaf) */
    "..................",
    "..................",
    ".........BB.......",
    ".......BGGGB......",
    ".......GGGG.......",
    "......GG..........",
    "....rrLLLLrrr.....",
    "...rLLLLLLLLrr....",
    "..rLLLLLLLLLLrr...",
    ".rLLLLLLLLLLLLLr..",
    ".rLLLLLLLLLLLLLr..",
    ".rLLLLLLLLLLLLLr..",
    ".rDDDDDDDDDDDDDr..",
    "..rDDDDDDDDDDDr...",
    "...rDDDDDDDDDr....",
    "....rDDDDDDDr.....",
    ".....rDDDDDr......",
    ".................."
};


const char *const snake_tongue_down[] = {
  "18 18 4 1", /* width height num_colors */
  ". c #000000",     /* Black (background/transparent) */
  "R c #E4002B",     /* Red (base tongue color) */
  "D c #A0001F",     /* Dark Red (shading) */
  "L c #FF5577",     /* Light Red (highlight) */
  "..................",
  "..................",
  "..................",
  "..................",
  ".......RLLR.......",
  "......RRLLRR......",
  "......RRDLDR......",
  ".....RRD..DRR.....",
  "....DR....DRD.....",
  "...DR......DRD....",
  "..DR........DRD...",
  ".DR..........DRD..",
  ".D............D...",
  "..................",
  "..................",
  "..................",
  "..................",
  ".................."
};

const char *const snake_tongue_up[] = {
  "18 18 4 1", /* width height num_colors */
  ". c #000000",     /* Black (background/transparent) */
  "R c #E4002B",     /* Red (base tongue color) */
  "D c #A0001F",     /* Dark Red (shading) */
  "L c #FF5577",     /* Light Red (highlight) */
  "..................",
  "..................",
  "..................",
  "..................",
  "..................",
  ".D............D...",
  ".DR..........DRD..",
  "..DR........DRD...",
  "...DR......DRD....",
  "....DR....DRD.....",
  ".....RRD..DRR.....",
  "......RRDLDR......",
  "......RRLLRR......",
  ".......RLLR.......",
  "..................",
  "..................",
  "..................",
  ".................."
};

const char *const snake_tongue_left[] = {
  "18 18 4 1", /* width height num_colors */
  ". c #000000",     /* Black (background/transparent) */
  "R c #E4002B",     /* Red (base tongue color) */
  "D c #A0001F",     /* Dark Red (shading) */
  "L c #FF5577",     /* Light Red (highlight) */
  "..................",
  "..................",
  "......D...........",
  ".....DRD..........",
  "......DRD.........",
  ".......DRDR.......",
  "........DRRRR.....",
  ".........DDDRR....",
  "...........LLL....",
  "...........DLL....",
  "..........DRRR....",
  "..........RRR.....",
  ".........RR.......",
  "........RD........",
  ".......RD.........",
  "......RD..........",
  ".....DD...........",
  ".................."

};

const char *const snake_tongue_right[] = {
  "18 18 4 1", /* width height num_colors */
  ". c #000000",     /* Black (background/transparent) */
  "R c #E4002B",     /* Red (base tongue color) */
  "D c #A0001F",     /* Dark Red (shading) */
  "L c #FF5577",     /* Light Red (highlight) */
  "..................",
  "..................",
  "...........D......",
  "..........DRD.....",
  ".........DRD......",
  ".......RDRD.......",
  ".....RRRRD........",
  "....RRDDD.........",
  "....LLL...........",
  "....DLL...........",
  "....RRRD..........",
  ".....RRR..........",
  ".......RR.........",
  "........RD........",
  ".........RD.......",
  "..........RD......",
  "...........DD.....",
  ".................."

};

const char *const digit_0[] = {
    "9 9 2 1",
    ". c #000000",
    "w c #FFFFFF",
    ".wwwwwww.",
    ".w.....w.",
    ".w....ww.",
    ".w...w.w.",
    ".w..w..w.",
    ".w.w...w.",
    ".ww....w.",
    ".w.....w.",
    ".wwwwwww."
};

const char *const digit_1[] = {
    "9 9 2 1",
    ". c #000000",
    "w c #FFFFFF",
    "....ww...",
    "...www...",
    "....ww...",
    "....ww...",
    "....ww...",
    "....ww...",
    "....ww...",
    "....ww...",
    ".wwwwwww."
};

const char *const digit_2[] = {
    "9 9 2 1",
    ". c #000000",
    "w c #FFFFFF",
    ".wwwwwww.",
    ".w.....w.",
    "......w..",
    ".....w...",
    "...ww....",
    "..w......",
    ".w.......",
    ".w.....w.",
    ".wwwwwww."
};

const char *const digit_3[] = {
    "9 9 2 1",
    ". c #000000",
    "w c #FFFFFF",
    ".wwwwwww.",
    ".w.....w.",
    "......w..",
    ".....w...",
    "...ww....",
    "......w..",
    "......w..",
    ".w....w..",
    ".wwwwwww."
};

const char *const digit_4[] = {
    "9 9 2 1",
    ". c #000000",
    "w c #FFFFFF",
    ".....ww..",
    "....www..",
    "...w.ww..",
    "..w..ww..",
    ".w...ww..",
    ".wwwwwww.",
    ".....ww..",
    ".....ww..",
    ".....ww.."
};

const char *const digit_5[] = {
    "9 9 2 1",
    ". c #000000",
    "w c #FFFFFF",
    ".wwwwwww.",
    ".w.......",
    ".w.......",
    ".wwwwww..",
    "......w..",
    "......w..",
    ".w....w..",
    ".w....w..",
    ".wwwwww.."
};

const char *const digit_6[] = {
    "9 9 2 1",
    ". c #000000",
    "w c #FFFFFF",
    "..wwwwww.",
    ".w.......",
    ".w.......",
    ".wwwwww..",
    ".w....w..",
    ".w....w..",
    ".w....w..",
    ".w....w..",
    "..wwww..."
};

const char *const digit_7[] = {
    "9 9 2 1",
    ". c #000000",
    "w c #FFFFFF",
    ".wwwwwww.",
    "......w..",
    ".....w...",
    ".....w...",
    "....w....",
    "...w.....",
    "..w......",
    "..w......",
    "..w......"
};

const char *const digit_8[] = {
    "9 9 2 1",
    ". c #000000",
    "w c #FFFFFF",
    ".wwwwwww.",
    ".w.....w.",
    ".w.....w.",
    ".wwwwww..",
    ".w.....w.",
    ".w.....w.",
    ".w.....w.",
    ".w.....w.",
    ".wwwwwww."
};

const char *const digit_9[] = {
    "9 9 2 1",
    ". c #000000",
    "w c #FFFFFF",
    ".wwwwww..",
    ".w....w..",
    ".w....w..",
    ".w....w..",
    "..wwwww..",
    "......w..",
    "......w..",
    "......w..",
    ".wwww...."
};

xpm_map_t get_digit_xpm(int digit) {
    switch (digit) {
        case 0: return digit_0;
        case 1: return digit_1;
        case 2: return digit_2;
        case 3: return digit_3;
        case 4: return digit_4;
        case 5: return digit_5;
        case 6: return digit_6;
        case 7: return digit_7;
        case 8: return digit_8;
        case 9: return digit_9;
        default: return digit_0; // fallback
    }
}

#endif /* _PIXMAP_H_ */
