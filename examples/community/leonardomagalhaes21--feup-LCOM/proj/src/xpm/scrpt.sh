#!/bin/bash

old_width=1152
old_height=864

new_width=800
new_height=600


# 0x105	1024x768	Indexed	8
# 0x110	640x480	Direct color	15((1:)5:5:5)
# 0x115	800x600	Direct color	24 (8:8:8)
# 0x11A	1280x1024	Direct color	16 (5:6:5)
# 0x14C	1152x864	Direct color	32 ((8:)8:8:8)

img_width=$((800 * 800 / old_width))
img_height=$((480 * 600 / old_height))
convert "PressStart2P-vaV7.xpm" -resize ${img_width}x${img_height} "PressStart2P-vaV7_115.xpm"

img_width=$((800 * 640 / old_width))
img_height=$((480 * 480 / old_height))
convert "PressStart2P-vaV7.xpm" -resize ${img_width}x${img_height} "PressStart2P-vaV7_110.xpm"

img_width=$((800 * 1280 / old_width))
img_height=$((480 * 1024 / old_height))
convert "PressStart2P-vaV7.xpm" -resize ${img_width}x${img_height} "PressStart2P-vaV7_11A.xpm"


