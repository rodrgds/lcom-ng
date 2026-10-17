#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>
#include <sys/mman.h>
#include <sys/types.h>

#include "vbe.h"
#include "proj.h"

/* Constants for VBE 0x105 mode */

#define VRAM_PHYS_ADDR    0xD0000000
#define H_RES             1024
#define V_RES		  	  768
#define BITS_PER_PIXEL	  8

/* Private global variables */
char *ptr ;
static char *video_mem;		/* Process address to which VRAM is mapped */

static unsigned h_res;		/* Horizontal screen resolution in pixels */
static unsigned v_res;		/* Vertical screen resolution in pixels */
static unsigned bits_per_pixel; /* Number of VRAM bits per pixel */

void * vg_init(unsigned long mode) {

	struct reg86u reg;


	sef_startup();

	reg.u.w.ax = 0x4F02; // VBE call, function 02 -- set VBE mode
	reg.u.w.bx = 1<<14|0x105; // set bit 14: linear framebuffer
	reg.u.b.intno = 0x10;
	if( sys_int86(& reg) != OK ) {
		printf("set_vbe_mode: sys_int86() failed \n");
		return NULL;
	}

	h_res = H_RES;
	v_res = V_RES;
	bits_per_pixel = BITS_PER_PIXEL;

	int r;
	struct mem_range mr;

	  /* Allow memory mapping */

	  mr.mr_base = (phys_bytes)VRAM_PHYS_ADDR;
	  mr.mr_limit = mr.mr_base + (H_RES*V_RES);

	  if( OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
		  panic("video_txt: sys_privctl (ADD_MEM) failed: %d\n", r);

	  /* Map memory */

	  video_mem = vm_map_phys(SELF, (void *)mr.mr_base, (H_RES*V_RES));

	  if(video_mem == MAP_FAILED)
		  panic("video_txt couldn't map video memory");



	return (void *)VRAM_PHYS_ADDR;


}

int vg_fill(unsigned long color) {
	int i ;

	ptr= video_mem;

	for(i=0 ; i<(h_res*v_res) ; i++)
	{
		*ptr=color ;
		ptr++ ;
	}


	return 0;
}

int vg_eraseShip(unsigned long color) {
	int x, y ;
	char *pointer;
	pointer = video_mem;


	for(y = 0 ; y < v_res; y++)
	{
		for(x = 0; x < h_res; x++)
		{
			if(x < 110) *pointer = color;
			pointer++;
		}
	}
/*
	for(i=0 ; i<(h_res*v_res) ; i++)
	{
		*ptr=color ;
		ptr++ ;
	}*/


	return 0;
}

int vg_set_pixel(unsigned long x, unsigned long y, unsigned long color) {

	char *pointer;
	pointer = video_mem;

	pointer = (pointer + (y*H_RES)+x);
	*pointer = color;

	//ptr = video_mem + x*h_res + y;
	//*ptr = color ;

	return 0;
}

long vg_get_pixel(unsigned long x, unsigned long y) {

	long result;
	char *pointer;
	pointer = video_mem;
//	pointer = video_mem + (x-1)*v_res + y ;
	pointer = (pointer + (y*H_RES) + x);
	result = *pointer;
	return result;
}

int vg_draw_line(unsigned long xi, unsigned long yi, 
		 unsigned long xf, unsigned long yf, unsigned long color) {

		int deltaX = abs(xf-xi);
		int deltaY = abs(yf - yi);
		int i,temp;

		if(deltaX > deltaY)
		{
			if(xf - xi < 0)
			{
				temp = xi;
				xi = xf;
				xf = temp;

				temp = yi;
				yi = yf;
				yf = temp;
			}
			for(i = 0; i <= deltaX; i++)
			{
				double y_point = (double)deltaY * (double)i / (double)deltaX;
				if(yf - yi < 0)
				{
					vg_set_pixel(xi + i,yi - (int)(y_point + 0.5), color);
				}
				else vg_set_pixel(xi + i,yi + (int)(y_point + 0.5), color);
			}
		}
		else
		{
			if(yf - yi < 0)
			{
				temp = xi;
				xi = xf;
				xf = temp;

				temp = yi;
				yi = yf;
				yf = temp;
			}
			for(i = 0; i <= deltaY; i++)
			{
				double x_point = (double)deltaX * (double)i / (double)deltaY;
				if(xf - xi < 0)
				{
					vg_set_pixel(xi - (int)(x_point + 0.5), yi + i, color);
				}
				else vg_set_pixel(xi + (int)(x_point + 0.5), yi + i, color);
			}

		}
	return 0;
}





void drawMainShip(int verticalPos, int erase)
{
	// ERASE : 1 APAGA
	int width, height, x, y;
	char *spaceship_map;
	spaceship_map = (char*)read_xpm(spaceship, &width, &height);

		for(x = verticalPos; x < height+verticalPos ; x++)
		{
			for(y = 0; y <width  ; y++, spaceship_map++)
			{
				if(erase == 0) vg_set_pixel(y,x,*spaceship_map);
				else vg_set_pixel(y,x,0x00);
			}
		}

}

void drawMissile(missile input, int erase)
{
	// ERASE : 1 APAGA
	int width, height, x, y;
	char *missile_map;
	//printf("entrou");
	missile_map =  (char*)read_xpm(missil2, &width, &height);
	//printf("saiu");

	//printf("vertical: %d, horizontal: %d \n \n", input.verticalPos, input.horizontalPos);
	for(x = input.verticalPos; x < height + input.verticalPos ; x++)
	{
		for(y = input.horizontalPos; y <width+input.horizontalPos; y++, missile_map++)
		{
			if(erase == 0) vg_set_pixel(y,x,*missile_map);
			else vg_set_pixel(y,x,0x00);
			//printf("entrou\n");
		}
	}
	//printf("X: %d | Y:%d \n",x,y);
}

void drawEnemy(enemy input, int erase)
{
	// ERASE : 1 APAGA
	int width, height, x, y;
	char *enemy;
	enemy = (char*)read_xpm(enemy_pix, &width, &height);

	for(x = input.verticalPos; x < height + input.verticalPos ; x++)
		{
			for(y = input.horizontalPos; y <width+input.horizontalPos; y++, enemy++)
			{
				if(erase == 0) vg_set_pixel(y,x,*enemy);
				else vg_set_pixel(y,x,0x00);
				//printf("entrou\n");
			}
		}
}


void drawNumber(int numero, int x_pos, int y_pos, int erase, int small)
{
	//ERASE 1: Apaga
	//SMALL 1: NUMERO PEQUENO
	int width, height, x, y;
	char *imagem;
	//printf("Numero : %d \n",numero);
	switch(numero)
	{
		case 0: {
					if(small == 1) imagem = (char*)read_xpm(zero_small, &width, &height);
					else imagem = (char*)read_xpm(zero, &width, &height);
				}
				break;

		case 1: {
					if(small == 1) imagem = (char*)read_xpm(one_small, &width, &height);
					else imagem = (char*)read_xpm(one, &width, &height);
				}
				break;

		case 2: {
					if(small == 1) imagem = (char*)read_xpm(two_small, &width, &height);
					else imagem = (char*)read_xpm(two, &width, &height);
				}
				break;

		case 3: {
					if(small == 1) imagem = (char*)read_xpm(three_small, &width, &height);
					else imagem = (char*)read_xpm(three, &width, &height);
				}
				break;

		case 4: {
					if(small == 1) imagem = (char*)read_xpm(four_small, &width, &height);
					else imagem = (char*)read_xpm(four, &width, &height);
				}
				break;

		case 5: {
					if(small == 1) imagem = (char*)read_xpm(five_small, &width, &height);
					else imagem = (char*)read_xpm(five, &width, &height);
				}
				break;

		case 6: {
				if(small == 1) imagem = (char*)read_xpm(six_small, &width, &height);
				else imagem = (char*)read_xpm(six, &width, &height);
				}
				break;

		case 7: {
					if(small == 1) imagem = (char*)read_xpm(seven_small, &width, &height);
					else imagem = (char*)read_xpm(seven, &width, &height);
				}
				break;

		case 8: {
					if(small == 1) imagem = (char*)read_xpm(eight_small, &width, &height);
					else imagem = (char*)read_xpm(eight, &width, &height);
				}
				break;

		case 9: {
					if(small == 1) imagem = (char*)read_xpm(nine_small, &width, &height);
					else imagem = (char*)read_xpm(nine, &width, &height);
				}
				break;

		default: imagem = (char*)read_xpm(zero, &width, &height);
				break;
	}
	//printf("Passou o Switch\n");
	for(x = x_pos; x < height + x_pos ; x++)
	{
		//printf("x: %d",x);
		for(y = y_pos; y <width + y_pos; y++, imagem++)
		{
			//printf("y: %d \n",y);
			if(erase == 0) vg_set_pixel(y,x,*imagem);
			else vg_set_pixel(y,x,0x00);
			//printf("entrou\n");
		}
	}

}

void drawLetter(char letra, int x_pos, int y_pos, int erase)
{
	//ERASE 1: Apaga
	int width, height, x, y;
	char *imagem;
	//printf("Numero : %d \n",numero);
	switch(letra)
	{
		case 'A': imagem = (char*)read_xpm(letra_A, &width, &height);
				break;
		case 'B': imagem = (char*)read_xpm(letra_B, &width, &height);
				break;
		case 'C': imagem = (char*)read_xpm(letra_C, &width, &height);
				break;
		case 'D': imagem = (char*)read_xpm(letra_D, &width, &height);
				break;
		case 'E': imagem = (char*)read_xpm(letra_E, &width, &height);
				break;
		case 'F': imagem = (char*)read_xpm(letra_F, &width, &height);
				break;
		case 'G': imagem = (char*)read_xpm(letra_G, &width, &height);
				break;
		case 'H': imagem = (char*)read_xpm(letra_H, &width, &height);
				break;
		case 'I': imagem = (char*)read_xpm(letra_I, &width, &height);
				break;
		case 'J': imagem = (char*)read_xpm(letra_J, &width, &height);
				break;
		case 'K': imagem = (char*)read_xpm(letra_K, &width, &height);
				break;
		case 'L': imagem = (char*)read_xpm(letra_L, &width, &height);
				break;
		case 'M': imagem = (char*)read_xpm(letra_M, &width, &height);
				break;
		case 'N': imagem = (char*)read_xpm(letra_N, &width, &height);
				break;
		case 'O': imagem = (char*)read_xpm(letra_O, &width, &height);
				break;
		case 'P': imagem = (char*)read_xpm(letra_P, &width, &height);
				break;
		case 'Q': imagem = (char*)read_xpm(letra_Q, &width, &height);
				break;
		case 'R': imagem = (char*)read_xpm(letra_R, &width, &height);
				break;
		case 'S': imagem = (char*)read_xpm(letra_S, &width, &height);
				break;
		case 'T': imagem = (char*)read_xpm(letra_T, &width, &height);
				break;
		case 'V': imagem = (char*)read_xpm(letra_V, &width, &height);
				break;
		case 'U': imagem = (char*)read_xpm(letra_U, &width, &height);
				break;
		case 'W': imagem = (char*)read_xpm(letra_W, &width, &height);
				break;
		case 'X': imagem = (char*)read_xpm(letra_X, &width, &height);
				break;
		case 'Y': imagem = (char*)read_xpm(letra_Y, &width, &height);
				break;
		case 'Z': imagem = (char*)read_xpm(letra_Z, &width, &height);
				break;
		default: imagem = (char*)read_xpm(letra_A, &width, &height);
				break;
	}
	//printf("Passou o Switch\n");
	for(x = x_pos; x < height + x_pos ; x++)
	{
		//printf("x: %d",x);
		for(y = y_pos; y <width + y_pos; y++, imagem++)
		{
			//printf("y: %d \n",y);
			if(erase == 0) vg_set_pixel(y,x,*imagem);
			else vg_set_pixel(y,x,0x00);
			//printf("entrou\n");
		}
	}

}

void drawInterface(int numero_id, int x_pos, int y_pos, int erase)
{
	//ERASE 1: Apaga
	int width, height, x, y;
	char *imagem;
	switch(numero_id)
	{
		case 0: imagem = (char*)read_xpm(insert_name, &width, &height);
				break;

	}
	for(x = x_pos; x < height + x_pos ; x++)
	{
		//printf("x: %d",x);
		for(y = y_pos; y <width + y_pos; y++, imagem++)
		{
			//printf("y: %d \n",y);
			if(erase == 0) vg_set_pixel(y,x,*imagem);
			else vg_set_pixel(y,x,0x00);
			//printf("entrou\n");
		}
	}
}

void drawPontuacao(int valor,int x_pos, int y_pos, int small)
{
	//6 Algarismos- y: 590
	// Primeiro algarismo x: 780
	//	824, 868, 912, 956, 1000
	int largura = 44;
	int aux, temp, n_algarismos;
	int last_x_pos = x_pos;
	int last_y_pos = y_pos;
	aux = valor;
	n_algarismos = 0;

	if(small = 1) largura = 20;

	while(aux >= 10)
	{
		temp = aux%10;
		aux = aux/10;
		//printf("Temp: %d | aux: %d | last_y_pos: %d | last_x_pos: %d | \n",temp,aux,last_y_pos,last_x_pos);
		drawNumber(temp, last_y_pos, last_x_pos, 0, small );
		last_x_pos = last_x_pos - largura - 2; // 2 de espacamento


		n_algarismos++;
	}
	drawNumber(aux, last_y_pos, last_x_pos, 0, small );
	last_x_pos = last_x_pos - largura - 2;
	n_algarismos++;

	while(n_algarismos < 6)
	{
		drawNumber(0, last_y_pos, last_x_pos, 0,small );
		last_x_pos = last_x_pos - largura  - 2; // 2 de espacamento
		n_algarismos++;
	}
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
