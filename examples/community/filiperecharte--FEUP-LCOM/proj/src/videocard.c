#include "videocard.h"

vbe_mode_info_t vmi_p;

int r;
struct minix_mem_range mr; /* physical memory range */
unsigned int vram_base; /* VRAM�s physical addresss */
unsigned int vram_size; /* VRAM�s size */
void* video_mem; /* frame-buffer VM address */
void* buffer; /* Double-buffer */

static uint16_t hres; /* XResolution */
static uint16_t vres; /* YResolution */

uint8_t bytes_per_pixel;

void* (vg_init)(uint16_t mode) {	
	if (vbe_get_mode_info(mode, &vmi_p) != 0)
		return NULL;

	hres = vmi_p.XResolution;
	vres = vmi_p.YResolution;

	bytes_per_pixel = (vmi_p.BitsPerPixel + 7) / 8;

	vram_base = vmi_p.PhysBasePtr;
	vram_size = (hres * vres * bytes_per_pixel);

					 
	/* Allow memory mapping */
	mr.mr_base = (phys_bytes)vram_base;
	mr.mr_limit = mr.mr_base + vram_size;
	
	if (OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
		panic("sys_privctl (ADD_MEM) failed: %d\n", r);
	
	/* Map memory */
	video_mem = vm_map_phys(SELF, (void*)mr.mr_base, vram_size);
	buffer = malloc(vram_size);
	
	if (video_mem == NULL)
		panic("could not map video memory");

	if (mode_setup(mode) != 0)
		return NULL;

	return video_mem;
}

int mode_setup(uint16_t mode) {
	
	struct reg86 r;
	
	memset(&r, 0, sizeof(r));	/* zero the structure */


	r.ax = 0x4F02; // VBE call, function 02 -- set VBE mode
	r.bx = 1 << 14 | mode; // set bit 14: linear framebuffer
	r.intno = 0x10;
	if (sys_int86(&r) != OK) {
		printf("set_vbe_mode: sys_int86() failed \n");
		return 1;
	}

	return 0;
}

int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {
	
	
	for (int i = 0; i < height; i++) {
		if (vg_draw_hline(x, y + i, width, color) != 0) {
			printf("Error drawing line\n");
			return 1;
		}
	}

	return 0;
}

int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color) {
	for (int i = 0; i < len; i++) {
		if (fill_pixel(x + i, y, color) != 0) {
			printf("Error filling pixel\n");
			return 1;
		}
	}

	return 0;
}

int fill_pixel(uint16_t x, uint16_t y, uint32_t color) {

	uint8_t *adr = buffer;

	adr += (hres * y * bytes_per_pixel);
	adr += (x * bytes_per_pixel);

	for (int i = 0; i < bytes_per_pixel; i++)
	{
		*adr = (color >> (i * 8)); // Color the pixel
		adr++;
	}

	return 0;
}

uint16_t get_hres() {
	return hres;
}

uint16_t get_vres() {
	return vres;
}

int draw_xpm(uint8_t *xpm_adr, xpm_image_t img, uint16_t x, uint16_t y) {
	uint32_t color = 0;

	for (uint16_t i = 0; i < img.height; i++) // Row
	{
		for (uint16_t j = 0; j < img.width; j++) { // Column

			for (int z = 0; z < bytes_per_pixel; z++)
			{
				color += img.bytes[i * img.width + j + z] << (8 * z);
			}
			
			if (color != xpm_transparency_color(XPM_5_6_5)) { // Ignore transparent blocks
				if (fill_pixel(x + j, y + i, color) != 0) { // Color at a given pixel
					printf("Error filling pixel\n");
					return 1;
				}
			}

			// Reset

			img.bytes += bytes_per_pixel - 1;
			color = 0;
		}
	}

	return 0;
}

void increment(int16_t value, uint16_t *xi, uint16_t *yi, uint16_t xf, uint16_t yf)
{
	if (xf != *xi)
	{
		if (xf - *xi >= value) { // Distance
			*xi += value;
		}
		else if (*xi - xf >= value) { // Distance
			*xi -= value;
		}
		else {
			*xi = xf; // Reached end
		}
	}

	if (yf != *yi)
	{
		if (yf - *yi >= value) { // Distance
			*yi += value;
		}
		else if (*yi - yf >= value) { // Distance
			*yi -= value;
		}
		else {
			*yi = yf; // Reached end
		}
	}
	
	return;
}

void clear() {
	memset(video_mem, 0, bytes_per_pixel * hres * vres);
}

void clear_buffer() {
	memset(buffer, 0, bytes_per_pixel * hres * vres);
}


void draw_buffer() {
	memcpy(video_mem, buffer, vram_size);
}

void* getBuffer() {
	return buffer;
}

size_t getSize() {
	return vram_size;
}
