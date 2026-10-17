#ifndef __VT_INFO_H
#define __VT_INFO_H

typedef struct {
	/* VRAM info */
	unsigned long vram_size;    /* 0x2000 for color or 0x0800 for mono */
	void * vram_base;
	/* Text mode resolution */
	unsigned scr_width;		/* # columns of the screen */
	unsigned scr_lines;		/* # lines of the screen */
} vt_info_t;

void vt_info_get(vt_info_t *vi_p);
void vt_info_display(vt_info_t *vi_p);

#endif /* __VT_INFO_H */
