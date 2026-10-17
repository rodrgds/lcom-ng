#include "vbe.h"
#include "video_gr.h"
#include "variables.h"

/* Private global variables */

unsigned char *video_mem; 		/**< @brief Process address to which VRAM is mapped */
unsigned char *buffer;			/**< @brief Buffer memory map */
static unsigned h_res;          /**< @brief Horizontal screen resolution in pixels */
static unsigned v_res;          /**< @brief Vertical screen resolution in pixels */
static unsigned bits_per_pixel; /**< @brief Number of VRAM bits per pixel */

int vbe_get_mode_info(unsigned short mode, vbe_mode_info_t *vmi_p) {

	mmap_t buf;
	struct reg86u r;
	int success = 0;

	if (lm_alloc(sizeof(vbe_mode_info_t), &buf) == NULL)
		return 1;

	r.u.b.ah = VBE;					// VBE get mode info
	r.u.b.al = INFO_MODE;			// translate the buffer linear address to a far pointer
	r.u.w.es = PB2BASE(buf.phys);	// set a segment base
	r.u.w.di = PB2OFF(buf.phys);	// set the offset accordingly
	r.u.w.cx = mode;
	r.u.b.intno = 0x10;
	if (sys_int86(&r) == OK)
		if (r.u.w.ax == VBE) {
			*vmi_p = *((vbe_mode_info_t *)buf.virtual);
			success = 1;
		}

	if (r.u.b.ah == FAILURE) {
		lm_free(&buf);
		return 1;
	} else if (r.u.b.ah == NOT_SUPPORTED) {
		lm_free(&buf);
		return 1;
	} else if (r.u.b.ah == INVALID) {
		lm_free(&buf);
		return 1;
	}

	lm_free(&buf);

	if (success)
		return 0;
	else
		return 1;
}

int vg_exit() {
	struct reg86u reg86;

	reg86.u.b.intno = 0x10; /* BIOS video services */

	reg86.u.b.ah = 0x00; /* Set Video Mode function */
	reg86.u.b.al = 0x03; /* 80x25 text mode*/

	if (sys_int86(&reg86) != OK) {
		return 1;
	} else
		return 0;
}

void *vg_init(unsigned short mode) {
	vbe_mode_info_t info;
	if (vbe_get_mode_info(mode, &info) == 1)
		return NULL;

	int r;
	struct mem_range mr;
	h_res = info.XResolution;
	v_res = info.YResolution;
	bits_per_pixel = info.BitsPerPixel;

	unsigned int vram_base = 0xE0000000; // VRAM's physical addresss
	unsigned int vram_size =
			(info.XResolution * info.YResolution * info.BitsPerPixel) /
			8; // VRAM's size, but you can use
	// the frame-buffer size, instead

	// this sets the mode
	struct reg86u reg;
	reg.u.w.ax = 0x4F02;          // VBE call, function 02 -- set VBE mode
	reg.u.w.bx = 1 << 14 | 0x105; // set bit 14: linear framebuffer
	reg.u.b.intno = 0x10;
	if (sys_int86(&reg) != OK) {
		return NULL;
	}

	// Allow memory mapping
	mr.mr_base = (phys_bytes)vram_base;
	mr.mr_limit = mr.mr_base + vram_size;
	if (OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
		panic("sys_privctl (ADD_MEM) failed: %d\n", r);

	// Map memory
	video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);
	if (video_mem == MAP_FAILED)
		panic("couldn't map video memory");

	buffer = (char *)malloc(h_res * v_res * bits_per_pixel / 8);

	return video_mem;
}

void copy_to_video_mem() {
	memcpy(video_mem, buffer, getHRes() * getVRes() * getBitsPerPixel() / 8);
}

void clear_buffer() {
	memset(buffer, 0, getHRes() * getVRes() * getBitsPerPixel() / 8);
}

char *getBuffer() { return buffer; }

unsigned getHRes() { return h_res; }

unsigned getVRes() { return v_res; }

unsigned getBitsPerPixel() { return bits_per_pixel; }
