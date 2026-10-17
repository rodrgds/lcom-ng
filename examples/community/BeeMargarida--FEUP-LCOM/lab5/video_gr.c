#include "video_gr.h"


/* Constants for VBE 0x105 mode */

/* The physical address may vary from VM to VM.
 * At one time it was 0xD0000000
 *  #define VRAM_PHYS_ADDR    0xD0000000 
 * Currently on lab B107 is 0xF0000000
 * Better run my version of lab5 as follows:
 *     service run `pwd`/lab5 -args "mode 0x105"
 */


/* Private global variables */

static char *video_mem;		/* Process address to which VRAM is mapped */

static unsigned h_res;		/* Horizontal screen resolution in pixels */
static unsigned v_res;		/* Vertical screen resolution in pixels */
static unsigned bits_per_pixel; /* Number of VRAM bits per pixel */


int *vg_init(unsigned short mode){

	//setting into graphic mode
	struct reg86u r;
	r.u.w.ax = VBE_MODE; // VBE call, function 02 -- set VBE mode
	r.u.w.bx = 1<<14|LINEAR_FRAME; // set bit 14: linear framebuffer
	r.u.b.intno = 0x10;
	if( sys_int86(&r) != OK ) {
		printf("set_vbe_mode: sys_int86() failed \n");
		return 1;
	}

	int ri;
	struct mem_range mr;
	unsigned int vram_base = VRAM_PHYS_ADDR; /* VRAM’s physical addresss */
	unsigned int vram_size = (H_RES*V_RES*BITS_PER_PIXEL); /* VRAM’s size, but you can use
	the frame-buffer size, instead */

	/* Allow memory mapping */
	mr.mr_base = (phys_bytes) vram_base;
	mr.mr_limit = mr.mr_base + vram_size;
	if( OK != (ri = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
		panic("sys_privctl (ADD_MEM) failed: %d\n", ri);
	/* Map memory */
	video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);
	if(video_mem == MAP_FAILED)
		panic("couldn’t map video memory");


	return *video_mem;
}

int vg_exit() {
	struct reg86u reg86;

	reg86.u.b.intno = 0x10; /* BIOS video services */

	reg86.u.b.ah = 0x00;    /* Set Video Mode function */
	reg86.u.b.al = 0x03;    /* 80x25 text mode*/

	if( sys_int86(&reg86) != OK ) {
		printf("\tvg_exit(): sys_int86() failed \n");
		return 1;
	} else
		return 0;
}

void painter(unsigned short x, unsigned short y, unsigned long color){
	long endr = y*(H_RES)+ x;
	*(video_mem + endr) = color;
}

int test_square_draw(unsigned short x, unsigned short y, unsigned short size, unsigned long color) {
	unsigned short contx = x + size;
	unsigned short conty = y + size;
	unsigned short cx = x, cy = y;
	long endr;
	while(cy < conty){
		while(cx < contx){
			endr = cy*(H_RES)+ cx;
			*(video_mem + endr) = color;
			cx++;
		}
		cx = x;
		cy++;
	}
}

int test_line_draw(unsigned short xi, unsigned short yi, unsigned short xf, unsigned short yf, unsigned long color){
	int x;
	long endr;
	int dx = xf-xi;
	int dy = yf-yi;
	if(dx < 0)
		dx = -dx;
	if(dy < 0)
		dy = -dy;

	if((dy <= dx || dy > dx) && (dx != 0 && dy != 0)){
		if(xf < xi){
			unsigned short t = xf;
			xf = xi;
			xi = t;
			t = yf;
			yf = yi;
			yi = t;
		}
		if(yf > yi){
			x = 2*dy - dx;
			painter(xi,yi,color);
			xi = xi+1;
			while(xi <= xf){
				if(x<0)
					x += 2*dy;
				else{
					x += 2*(dy-dx);
					yi++;
				}
				painter(xi,yi,color);
				xi++;
			}
		}
		if(yf < yi){
			x = 2*dy - dx;
			painter(xi,yi,color);
			xi = xi+1;
			while(xi <= xf){
				if(x<0)
					x += 2*dy;
				else{
					x += 2*(dy-dx);
					yi--;
				}
				painter(xi,yi,color);
				xi++;
			}
		}
	}
	else{
		if(yf < yi){
			unsigned short t = xf;
			xf = xi;
			xi = t;
			t = yf;
			yf = yi;
			yi = t;
		}
		if(dx == 0){
			while(yi <= yf){
				painter(xi,yi,color);
				yi++;
			}
		}
		if(dy == 0){
			while(xi <= xf){
				painter(xi,yi,color);
				xi++;
			}
		}

	}

}
void clear_screen(){
	char *tmp;
	tmp = video_mem;
	int size = H_RES * V_RES;
	int i;
	for(i=0;i < size; i++){
		*tmp = (char) 0; //black color
		tmp++;
	}
}

void xpm_show(short xi, short yi, short xm, short ym, char *pix){
	long endr;
	short x = xi;
	while(yi < ym){
		while(xi < xm){
			endr = yi*H_RES + xi;
			*(video_mem + endr) = *pix;
			pix++;
			xi++;
		}
		xi = x;
		yi++;
	}
}

void test_xpm_show(int xi, int yi, int wd, int ht, char *pix){
	unsigned short contx = xi + wd;
	unsigned short conty = yi + ht;
	unsigned short cx = xi, cy = yi;
	long endr;
	while(cy < conty){
		while(cx < contx){
			endr = cy*(H_RES)+ cx;
			*(video_mem + endr) = *pix;
			pix++;
			cx++;
		}
		cx = xi;
		cy++;
	}
}
