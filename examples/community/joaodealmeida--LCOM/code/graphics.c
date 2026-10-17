#include <minix/drivers.h>
#include "video_gr.h"
#include "pixmap.h"
#include "sprite.h"
#include "timer.h"
#include "buffer.h"
#include "kbd.h"
#include "keyStates.h"

//#define USE_GENERIC_SET_PIXEL

unsigned char* vram_adress;//self explanatory
void set_vramAdress(void * adrss) {vram_adress=adrss;}

unsigned char * getVramAdress(){
	return vram_adress;
}

//could use a set() but in the application we know what mode is going to be used
//unless it could be chosen by user, not a good idea bcause of the sprite colors
static unsigned short numberOfBitsPerPixel=8;
static unsigned short numberOfBytesPerPixel=1;//256 color = 8bits=1byte

//these could use a set() unlike numberOfBytesPerPixel
//static unsigned short H_RES= 1024;//640;//1024;//horizontal resolution
//static unsigned short V_RES= 768;//480;//768;//vertical resolution

void *test_init(unsigned short mode, unsigned short delay) {
	
	/* To be completed */
	//init stuff
		resetIrqSet();
		int irq=0;
		if(irq=timer_subscribe_int()<0) return NULL;
		addIrq(irq);

	void* vram_adrss=NULL;
	vram_adrss=vg_init(mode);

		timer_setCounter( (unsigned long) delay);
		void (*funcs[1]) (unsigned int) = {timer_int_handler};

		initBuffer();

		//wait until timer finishes counting ================
		while (timer_getCounter()!=0){interruptHandler(funcs,1);}

		//unsubcribe and clean stuff
	if (vg_exit()!=OK) printf("\ntest_init:vg_exit() failed");
	if (vram_adrss==NULL) printf("\ntest_init:vg_init() returned null address");
	if(timer_unsubscribe_int()!=OK) return NULL;
	return vram_adrss;
}

void setPixel(unsigned int rawIndex,unsigned long color)//unsigned short x, unsigned short y,unsigned long color)
{
	/*if(numberOfBitsPerPixel==4)//not sure about 16bits (doens't work to test so...
	{
		unsigned int index = (y*H_RES+x)/2;//???
		r=x%2;
		if (!r) color=color<<4;
		unsigned char *pixel;
		pixel= &vram_adress[index];
		*pixel=(unsigned char) color;
	}else*/
#ifdef USE_GENERIC_SET_PIXEL
	unsigned int index = rawIndex*numberOfBytesPerPixel;//(y*H_RES+x)*numberOfBytesPerPixel;
	int i = 0;
	//i think this s the correct order
	//tested with mode 111 and assuming it uses the rgba format it's ok
	for (;i< numberOfBytesPerPixel;i++)
	{
		//unsigned char *pixel;
		//pixel= &vram_adress[index+i];
		//*pixel=(unsigned char) color;
		vram_adress[index+i]= (unsigned char) color;
		color = color>>8;
	}
#else //assumes mode 105
	//unsigned char *pixel;
	//pixel= &vram_adress[rawIndex];
	//*pixel=(unsigned char) color;
	vram_adress[rawIndex] = (unsigned char) color;
#endif
}

int test_square(unsigned short x, unsigned short y, unsigned short size, unsigned long color) {

	//if it is not possible to draw anything the we have an "exception"
	if (x>=H_RES||y>V_RES) return -1;
	//if square is outside bounds we cut it to fit in. no exception is given
	 unsigned short xEnd=x+size-1;
	 unsigned short yEnd=y+size-1;

	 //note:size one is a point
	if(xEnd>=H_RES) xEnd=H_RES-1;
	if(yEnd>=V_RES) yEnd=V_RES-1;

	int ix; //unsigned char *pixel;//assumes mode 105 and thus 8 bytes per pixel
	for (;y<=yEnd;y++)//reuse y as an aux var
		for (ix=x;ix<=xEnd;ix++)
			{
			setPixel(y*H_RES+ix,color);
			//pixel= &vram_adress[y*H_RES+ix];
			//*pixel=(unsigned char) color;//assumes mode 105 and thus 8 bytes per pixel
			}

	return 0;
}


int test_line(unsigned short xi, unsigned short yi, 
		           unsigned short xf, unsigned short yf, unsigned long color) {

	//Exception/error given if some point is outside screen
	if(xi>=H_RES) return 1;
	if(yi>=V_RES) return 1;
	if(xf>=H_RES) return 1;
	if(yf>=V_RES) return 1;
	//set initial and final pixels.//not need actually
	//setPixel(yi*H_RES+xi,color);
	//setPixel(yf*H_RES+xf,color);
	//=====start calculating stuff====
	int dx=xf-xi;
	int dy=yf-yi;

	//a very special case
	if(dx==0&&dy==0)
	{
		setPixel ( yi*H_RES+xi,color);
		return 0;
	}

	//iterates a "linear eq" part or line segment whatever you want to call it
	float m;//slope dminor/dmax
	if (dx*dx>dy*dy)//it's only going to have a pixel painted per x colum
	{
		m= ((float) dy)/dx; //y=m*x+yi
		//if final<initial remainder-> swap /to use "linear equations" (the same way represented in paper)
		if (xi>xf) { xi^=xf; xf^=xi; xi^=xf; yi^=yf; yf^=yi; yi^=yf; dx*=-1;}
		//m auto remains = /switched sign on both dx and dy

		//reuse dx to iterate, use 0.1f as precaution instead of because it's a float
		for(;dx>=0;dx--)
		{
			yf=(unsigned int) (dx*m+yi+0.5f);//reuse if as being y coordenate on screen
			setPixel ( yf*H_RES+xi+dx,color);
		}

	} else //it's only going to have a pixel painted per y colum
	{
		m=((float) dx)/dy; //y=m*x+yi
		//if final<initial -> swap
		if (yi>yf) { xi^=xf; xf^=xi; xi^=xf; yi^=yf; yf^=yi; yi^=yf;dy*=-1;}

		for(;dy>=0;dy--)//reuse dy
		{
		xf=(unsigned int) (dy*m+xi+0.5f);//reuse if as being x coordenate on screen
		setPixel ( (yi+dy)*H_RES+xf,color);
		}
	}
	return 0;
}
/*
int test_xpm(unsigned short xi, unsigned short yi, char *xpm[]) {
	
	//create pixmap
	Pixmap pm;
	pm.pixmap = read_xpm(xpm, &pm.width, &pm.height);
	addPixmap(&pm);
	//create sprite
	spritePtr sp = createStaticSprite(xi, yi,0,0,0);
	//Sprite sp={ .x = xi, .y = yi, .index = 0 };
	//draw sprite
	draw_sprite_onScreen(sp);
}	

int test_move(unsigned short xi, unsigned short yi, char *xpm[], 
				unsigned short hor, short delta, unsigned short time) {
	//question:is delta the increment to be done after a timer interrupt?
	//or the total after the time is completed. increment would be (delta/time/60)

	//init stuff=================

	//subscriptions
	resetIrqSet();
	int irq=0;
	if(irq=timer_subscribe_int()<0) return NULL;
	addIrq(irq);
	if(irq=Kbd_subscribe_int()<0)return-1;
	addIrq(irq);

	//load pixmap and create sprite
	Pixmap pm;
	pm.pixmap = read_xpm(xpm, &pm.width, &pm.height);

	//check 4 exceptions
	//check bounds in beginning and end of movement
	if (xi+pm.width>H_RES) return 1;
	if (xi+pm.width>V_RES) return 1;
	if (hor)
	{
		if (delta>0) { if (xi+pm.width+delta*time*60>H_RES) return 1;}
		else if ( ((int) xi)+delta*time*60<0) {	return 1;}
	}
	else{
		if (delta>0) { if (yi+pm.height+delta*time*60>V_RES) return 1;}
		else if ( ((int) yi)+delta*time*60<0) {	return 1;}
	}

	//continue loading and sprite creation
	addPixmap(&pm);
	spritePtr sp = createStaticSprite(xi, yi,0,0,0);
	draw_sprite_onScreen(sp);//self explanatory

	//create a buffer
	initBuffer();

	//setup generic handler
	timer_setCounter( (unsigned long) time);
	void (*funcs[2]) (unsigned int) = {timer_int_handler,Kbd_int_handler};

	//var that confirms that the defined time has passed
	//note:should use a different method on project! may need different counters at the same time!
	char doneCounting=0;

	//do stuff================
	while (ks_keyReleased(KEY_ESCAPE)==OK){

		interruptHandler(funcs,2);//only has timer interrupt so it works fine

		//update keys
		ks_updateKeyStates(Kbd_getLastScanCodeRead());

		//update sprite
		//uses the var counter defined in the timer since it is simple stuff
		if (timer_getCounter()==0) ;
		if (hor) sp->x+=delta;
		else sp->y+=delta;

		//update buffer
		if (!doneCounting){
		doneCounting = (timer_getCounter()==0);
		clearBuffer();
		draw_sprite_onBuffer(sp);
		}

		//draw buffer
		printBuffer(vram_adress);
	}

	//unsubcribe and clean stuff
	freeBuffer();
	if(timer_unsubscribe_int()!=OK) return NULL;
	if(Kbd_unsubscribe_int()!=OK) return -1;

	return 0;
}					

int test_controller(){
	vg_info();
}
*/
