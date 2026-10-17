#include "graphics.h"

//unsigned int hook_id_timer = 2;



char *vg_init(unsigned short mode){

	//setting into graphic mode
	struct reg86u r;
	r.u.w.ax = VBE_MODE; // VBE call, function 02 -- set VBE mode
	r.u.w.bx = 1<<14|LINEAR_FRAME; // set bit 14: linear framebuffer
	r.u.b.intno = 0x10;
	if( sys_int86(&r) != OK ) {
		printf("set_vbe_mode: sys_int86() failed \n");
		return (char *)1;
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

	buffer = malloc(H_RES*V_RES*BITS_PER_PIXEL/8);


	return (char*) 0;
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

void actVideoMem(){
	memcpy(video_mem,buffer,H_RES*V_RES*BITS_PER_PIXEL/8);
}

void painter(unsigned short x, unsigned short y, unsigned long color){
	if(x >= H_RES || y >= V_RES)
		return;
	long endr = y*(H_RES)+ x;
	*(buffer + endr) = color;
}

void test_xpm_show(int xi, int yi, int wd, int ht, char *pix){
	unsigned short contx = xi + wd;
	unsigned short conty = yi + ht;
	unsigned short cx = xi, cy = yi;
	long endr;
	while(cy < conty){
		while(cx < contx){
			endr = cy*(H_RES)+ cx;
			*(buffer + endr) = *pix;
			pix++;
			cx++;
		}
		cx = xi;
		cy++;
	}
}

void menu_show(Menu *menu){
	char *menu_pix = menu->pix;
	unsigned short xi = menu->xtitulo, yi = menu->ytitulo;
	unsigned short xf = xi + menu->wd, yf = yi + menu->ht;
	while(yi < yf){
		while(xi < xf){
			long endr = yi*1024 + xi;
			*(buffer + endr) = *menu_pix;
			menu_pix++;
			xi++;
		}
		xi = menu->xtitulo;
		yi++;
	}

	xi = menu->Opcao1->xi;
	xf = menu->Opcao1->xi + menu->Opcao1->wd;
	yi = menu->Opcao1->yi;
	yf = menu->Opcao1->yi + menu->Opcao1->ht;
	char *pix = menu->Opcao1->pix;
	while(yi < yf){
		while(xi < xf){
			long endr = yi*1024 + xi;
			*(buffer + endr) = *pix;
			pix++;
			xi++;
		}
		xi = menu->Opcao1->xi;
		yi++;
	}

	xi = menu->Opcao2->xi;
	xf = menu->Opcao2->xi + menu->Opcao2->wd;
	yi = menu->Opcao2->yi;
	yf = menu->Opcao2->yi + menu->Opcao2->ht;
	pix = menu->Opcao2->pix;
	while(yi < yf){
		while(xi < xf){
			long endr = yi*1024 + xi;
			*(buffer + endr) = *pix;
			pix++;
			xi++;
		}
		xi = menu->Opcao2->xi;
		yi++;
	}

	xi = menu->cursor->x;
	xf = xi + menu->cursor->size;
	yi = menu->cursor->y;
	yf = yi + menu->cursor->size;

	while(yi < yf){
		while(xi < xf){
			long endr = yi*1024 + xi;
			if(menu->cursor->left)
				*(buffer + endr) = menu->cursor->color_p;
			else
				*(buffer + endr) = menu->cursor->color;
			xi++;
		}
		xi = menu->cursor->x;
		yi++;
	}
	xi = menu->footer->xa;
	xf = xi + menu->footer->wd;
	yi = menu->footer->ya;
	yf = yi + menu->footer->ht;
	pix = menu->footer->pix;
	while(yi < yf){
		while(xi < xf){
			long endr = yi*1024 + xi;
			*(buffer + endr) = *pix;
			pix++;
			xi++;
		}
		xi = menu->footer->xa;
		yi++;
	}

	getDate();
	int wd, ht;
	char *p = (char *) read_xpm(lcom, &wd, &ht);
	test_xpm_show(10, 10, wd, ht, p);
}

void clear_footer(Footer *r){
	short xi,yi,xf,yf;
	xi = r->xa;
	yi = r->ya;
	xf = r->xa + r->wd;
	yf = r->ya + r->ht;
	while(yi < yf){
		while(xi < xf){
			painter(xi,yi,0);
			xi++;
		}
		xi = r->xa;
		yi++;
	}
}

void clear_bar(Bar *b1, Bar *b2, int x){
	short xi;
	short yi;
	short xf;
	short yf;
	if (x == 0)
	{
		xi = b1->xa;
		yi = b1->ya;
		xf = xi + b1->wd;
		yf = yi + b1->ht;
		while(yi < yf){
			while(xi < xf){
				painter(xi,yi,0);
				xi++;
			}
			xi = b1->xa;
			yi++;
		}
	}
	if (x == 1)
	{
		xi = b2->xa;
		yi = b2->ya;
		xf = xi + b2->wd;
		yf = yi + b2->ht;
		while(yi < yf){
			while(xi < xf){
				painter(xi,yi,0);
				xi++;
			}
			xi = b2->xa;
			yi++;
		}
	}
}

void clear_ball(Ball *a){
	short xi = a->x;
	short yi = a->y;
	short xf = a->x + a->side;
	short yf = a->y + a->side;
	while(yi < yf){
		while(xi < xf){
			painter(xi,yi,0);
			xi++;
		}
		xi = a->x;
		yi++;
	}
}

void clear_cursor(Cursor *c){
	short xi = c->x;
	short yi = c->y;
	short xf = c->x + c->size;
	short yf = c->y + c->size;
	while(yi < yf){
		while(xi < xf){
			painter(xi,yi,0);
			xi++;
		}
		xi = c->x;
		yi++;
	}
}

void clear_screen(){
	short xi = 0;
	short yi = 0;
	short xf = 1024;
	short yf = 768;
	while(yi < yf){
		while(xi < xf){
			painter(xi,yi,0);
			xi++;
		}
		xi = 0;
		yi++;
	}
}

void show_stuff(int a, int x, int y, int x1){
	int wd, ht;

	switch(a/10){
	case 0:{
		char *pix = (char *)read_xpm(zeroT, &wd, &ht);
		test_xpm_show(x, y, wd, ht, pix);break;}
	case 1:{
		char *pix = (char *)read_xpm(umT, &wd, &ht);
		test_xpm_show(x, y, wd, ht, pix); break;}
	case 2:{
		char *pix =(char *) read_xpm(doisT, &wd, &ht);
		test_xpm_show(x, y, wd, ht, pix);break;}
	case 3:{
		char *pix = (char *)read_xpm(tresT, &wd, &ht);
		test_xpm_show(x, y, wd, ht, pix); break;}
	case 4:{
		char *p =(char *) read_xpm(quatroT, &wd, &ht);
		test_xpm_show(x, y, wd, ht, p); break;}
	case 5:{
		char *p =(char *) read_xpm(cincoT, &wd, &ht);
		test_xpm_show(x, y, wd, ht, p); break;}

	case 6:{
		char *p =(char *) read_xpm(seisT, &wd, &ht);
		test_xpm_show(x, y, wd, ht, p); break;}

	case 7:{
		char *p = (char *) read_xpm(seteT, &wd, &ht);
		test_xpm_show(x, y, wd, ht, p); break;}

	case 8:{
		char *p = (char *) read_xpm(oitoT, &wd, &ht);
		test_xpm_show(x, y, wd, ht, p); break;}

	case 9:{
		char *p = (char *) read_xpm(noveT, &wd, &ht);
		test_xpm_show(x, y, wd, ht, p); break;}
	}

	switch(a%10){
	case 0:{
		char *p = (char *)read_xpm(zeroT, &wd, &ht);
		test_xpm_show(x1, y, wd, ht, p);break;}

	case 1:{
		char *p =(char *) read_xpm(umT, &wd, &ht);
		test_xpm_show(x1, y, wd, ht, p); break;}

	case 2:{
		char *p = (char *)read_xpm(doisT, &wd, &ht);
		test_xpm_show(x1, y, wd, ht, p); break;}

	case 3:{
		char *p = (char *)read_xpm(tresT, &wd, &ht);
		test_xpm_show(x1, y, wd, ht, p); break;}

	case 4:{
		char *p =(char *) read_xpm(quatroT, &wd, &ht);
		test_xpm_show(x1, y, wd, ht, p); break;}

	case 5:{
		char *p =(char *) read_xpm(cincoT, &wd, &ht);
		test_xpm_show(x1, y, wd, ht, p); break;}

	case 6:{
		char *p =(char *) read_xpm(seisT, &wd, &ht);
		test_xpm_show(x1, y, wd, ht, p); break;}

	case 7:{
		char *p = (char *) read_xpm(seteT, &wd, &ht);
		test_xpm_show(x1, y, wd, ht, p); break;}

	case 8:{
		char *p = (char *) read_xpm(oitoT, &wd, &ht);
		test_xpm_show(x1, y, wd, ht, p); break;}

	case 9:{
		char *p = (char *) read_xpm(noveT, &wd, &ht);
		test_xpm_show(x1, y, wd, ht, p); break;}
	}
}

void show_date_and_hours(Date *d){
	int wd, ht;

	//day
	show_stuff(d->day, 20, 10, 44);
	//bar
	char *p = (char *) read_xpm(barra, &wd, &ht);
	test_xpm_show(68, 10, wd, ht,p);
	//month
	show_stuff(d->month, 92, 10, 116);
	//bar
	test_xpm_show(140, 10, wd, ht,p);
	//year
	show_stuff(d->year, 164, 10, 188);

	//hours
	show_stuff(d->hours,812,10,836);
	//two-points
	char *t = (char *) read_xpm(dp,&wd,&ht);
	test_xpm_show(860,10,wd,ht,t);
	//minutes
	show_stuff(d->minutes,884,10,908);
	//two-points
	test_xpm_show(932,10,wd,ht,t);
	//seconds
	show_stuff(d->seconds,956,10,980);
}

void show_date(Date *d){
	int wd, ht;

	//day
	show_stuff(d->day, 800, 10, 824);
	//bar
	char *p = (char *) read_xpm(barra, &wd, &ht);
	test_xpm_show(848, 10, wd, ht,p);
	//month
	show_stuff(d->month, 872, 10, 896);
	//bar
	test_xpm_show(920, 10, wd, ht,p);
	//year
	show_stuff(d->year, 944, 10, 968);
}

void show_time(int time){
	int wd, ht;

	switch(time/10){
	case 0:{
		char *pix = (char *)read_xpm(zero, &wd, &ht);
		test_xpm_show(416, 10, wd, ht, pix);break;}
	case 1:{
		char *pix = (char *)read_xpm(um, &wd, &ht);
		test_xpm_show(416, 10, wd, ht, pix); break;}
	case 2:{
		char *pix =(char *) read_xpm(dois, &wd, &ht);
		test_xpm_show(416, 10, wd, ht, pix);break;}
	case 3:{
		char *pix = (char *)read_xpm(tres, &wd, &ht);
		test_xpm_show(416, 10, wd, ht, pix); break;}
	case 4:{
		char *pix =(char *) read_xpm(quatro, &wd, &ht);
		test_xpm_show(416, 10, wd, ht, pix); break;}
	case 5:{
		char *pix =(char *) read_xpm(cinco, &wd, &ht);
		test_xpm_show(416, 10, wd, ht, pix); break;}
	case 6:{
		char *pix = (char *)read_xpm(seis, &wd, &ht);
		test_xpm_show(416, 10, wd, ht, pix); break;}
	}

	switch(time%10){
	case 0:{
		char *p = (char *)read_xpm(zero, &wd, &ht);
		test_xpm_show(513, 10, wd, ht, p);break;}

	case 1:{
		char *p =(char *) read_xpm(um, &wd, &ht);
		test_xpm_show(513, 10, wd, ht, p); break;}

	case 2:{
		char *p = (char *)read_xpm(dois, &wd, &ht);
		test_xpm_show(513, 10, wd, ht, p); break;}

	case 3:{
		char *p = (char *)read_xpm(tres, &wd, &ht);
		test_xpm_show(513, 10, wd, ht, p); break;}

	case 4:{
		char *p =(char *) read_xpm(quatro, &wd, &ht);
		test_xpm_show(513, 10, wd, ht, p); break;}

	case 5:{
		char *p =(char *) read_xpm(cinco, &wd, &ht);
		test_xpm_show(513, 10, wd, ht, p); break;}

	case 6:{
		char *p =(char *) read_xpm(seis, &wd, &ht);
		test_xpm_show(513, 10, wd, ht, p); break;}

	case 7:{
		char *p = (char *) read_xpm(sete, &wd, &ht);
		test_xpm_show(513, 10, wd, ht, p); break;}

	case 8:{
		char *p = (char *) read_xpm(oito, &wd, &ht);
		test_xpm_show(513, 10, wd, ht, p); break;}

	case 9:{
		char *p = (char *) read_xpm(nove, &wd, &ht);
		test_xpm_show(513, 10, wd, ht, p); break;}
	}
}

void show_score(Bar *b1, Bar *b2){
	int wd, ht;
	char *a;
	switch(b1->score/10){
	case 0:{
		a = (char *)read_xpm(zero, &wd, &ht);
		break;}

	case 1:{
		a =(char *) read_xpm(um, &wd, &ht);
		break;}

	case 2:{
		a = (char *)read_xpm(dois, &wd, &ht);
		break;}

	case 3:{
		a = (char *)read_xpm(tres, &wd, &ht);
		break;}

	case 4:{
		a =(char *) read_xpm(quatro, &wd, &ht);
		break;}

	case 5:{
		a =(char *) read_xpm(cinco, &wd, &ht);
		break;}

	case 6:{
		a =(char *) read_xpm(seis, &wd, &ht);
		break;}

	case 7:{
		a = (char *) read_xpm(sete, &wd, &ht);
		break;}

	case 8:{
		a = (char *) read_xpm(oito, &wd, &ht);
		break;}

	case 9:{
		a = (char *) read_xpm(nove, &wd, &ht);
		break;}
	}
	test_xpm_show(100, 90, wd, ht, a);
	char *b;
	switch(b1->score%10){
	case 0:{
		b = (char *)read_xpm(zero, &wd, &ht);
		break;}

	case 1:{
		b =(char *) read_xpm(um, &wd, &ht);
		break;}

	case 2:{
		b = (char *)read_xpm(dois, &wd, &ht);
		break;}

	case 3:{
		b = (char *)read_xpm(tres, &wd, &ht);
		break;}

	case 4:{
		b =(char *) read_xpm(quatro, &wd, &ht);
		break;}

	case 5:{
		b =(char *) read_xpm(cinco, &wd, &ht);
		break;}

	case 6:{
		b =(char *) read_xpm(seis, &wd, &ht);
		break;}

	case 7:{
		b = (char *) read_xpm(sete, &wd, &ht);
		break;}

	case 8:{
		b = (char *) read_xpm(oito, &wd, &ht);
		break;}

	case 9:{
		b = (char *) read_xpm(nove, &wd, &ht);
		break;}
	}
	test_xpm_show(200, 90, wd, ht, b);
	char *c;
	switch(b2->score/10){
	case 0:{
		c = (char *)read_xpm(zero, &wd, &ht);
		break;}

	case 1:{
		c =(char *) read_xpm(um, &wd, &ht);
		break;}

	case 2:{
		c = (char *)read_xpm(dois, &wd, &ht);
		break;}

	case 3:{
		c = (char *)read_xpm(tres, &wd, &ht);
		break;}

	case 4:{
		c =(char *) read_xpm(quatro, &wd, &ht);
		break;}

	case 5:{
		c =(char *) read_xpm(cinco, &wd, &ht);
		break;}

	case 6:{
		c =(char *) read_xpm(seis, &wd, &ht);
		break;}

	case 7:{
		c = (char *) read_xpm(sete, &wd, &ht);
		break;}

	case 8:{
		c = (char *) read_xpm(oito, &wd, &ht);
		break;}

	case 9:{
		c = (char *) read_xpm(nove, &wd, &ht);
		break;}
	}
	test_xpm_show(729, 90, wd, ht, c);
	char *d;
	switch(b2->score%10){
	case 0:{
		d = (char *)read_xpm(zero, &wd, &ht);
		break;}

	case 1:{
		d =(char *) read_xpm(um, &wd, &ht);
		break;}

	case 2:{
		d = (char *)read_xpm(dois, &wd, &ht);
		break;}

	case 3:{
		d = (char *)read_xpm(tres, &wd, &ht);
		break;}

	case 4:{
		d =(char *) read_xpm(quatro, &wd, &ht);
		break;}

	case 5:{
		d =(char *) read_xpm(cinco, &wd, &ht);
		break;}

	case 6:{
		d =(char *) read_xpm(seis, &wd, &ht);
		break;}

	case 7:{
		d = (char *) read_xpm(sete, &wd, &ht);
		break;}

	case 8:{
		d = (char *) read_xpm(oito, &wd, &ht);
		break;}

	case 9:{
		d = (char *) read_xpm(nove, &wd, &ht);
		break;}
	}
	test_xpm_show(829, 90, wd, ht, d);
}

void test_rectangle_draw(unsigned short x, unsigned short y, unsigned short sizex, unsigned short sizey, unsigned long color) {
	unsigned short contx = x + sizex;
	unsigned short conty = y + sizey;
	unsigned short cx = x, cy = y;
	long endr;
	while(cy < conty){
		while(cx < contx){
			endr = cy*(H_RES)+ cx;
			*(buffer + endr) = color;
			cx++;
		}
		cx = x;
		cy++;
	}
}

void game_show(Bar *b1, Bar *b2, Ball *a){
	int time = 60;
	int wd, ht;
	char *p;

	show_time(time);
	p = (char *) read_xpm(player1, &wd, &ht);
	test_xpm_show(82, 35, wd, ht, p);
	p = (char *) read_xpm(player2, &wd, &ht);
	test_xpm_show(704, 35, wd, ht, p);
	p = (char *)read_xpm(zero, &wd, &ht);
	test_xpm_show(100, 90, wd, ht, p);
	p = (char *)read_xpm(zero, &wd, &ht);
	test_xpm_show(200, 90, wd, ht, p);
	p = (char *)read_xpm(zero, &wd, &ht);
	test_xpm_show(729, 90, wd, ht, p);
	p = (char *)read_xpm(zero, &wd, &ht);
	test_xpm_show(829, 90, wd, ht, p);


	test_rectangle_draw(10, 150, 1004, 20, 63);

	short xf = b1->xa + b1->wd;
	short yf = b1->ya + b1->ht;
	short xi = b1->xa;
	short yi = b1->ya;
	while(yi < yf){
		while(xi < xf){
			painter(xi, yi, 63);
			xi++;
		}
		xi = b1->xa;
		yi++;
	}

	xf = b2->xa + b2->wd;
	yf = b2->ya + b2->ht;
	xi = b2->xa;
	yi = b2->ya;

	while(yi < yf){
		while(xi < xf){
			painter(xi, yi, 63);
			xi++;
		}
		xi = b2->xa;
		yi++;
	}

	xf = a->x + a->side;
	yf = a->y + a->side;
	xi = a->x;
	yi = a->y;

	while(yi < yf){
		while(xi < xf){
			painter(xi, yi, 63);
			xi++;
		}
		xi = a->x;
		yi++;
	}
}

void paint_bar(Bar *b1){
	short xf = b1->xa + b1->wd;
	short yf = b1->ya + b1->ht;
	short xi = b1->xa;
	short yi = b1->ya;
	while(yi < yf){
		while(xi < xf){
			painter(xi, yi, 63);
			xi++;
		}
		xi = b1->xa;
		yi++;
	}
}

void paint_ball(Ball *a){
	short xf = a->x + a->side;
	short yf = a->y + a->side;
	short xi = a->x;
	short yi = a->y;
	while(yi < yf){
		while(xi < xf){
			painter(xi, yi, 63);
			xi++;
		}
		xi = a->x;
		yi++;
	}
}

void paint_footer(Footer *r){
	short xf = r->xa + r->wd;
	short yf = r->ya + r->ht;
	short xi = r->xa;
	short yi = r->ya;
	long endr;
	char *aux = r->pix;
	while(yi < yf){
		while(xi < xf){
			endr = yi*(H_RES)+ xi;
			*(buffer + endr) = *aux;
			aux++;
			xi++;
		}
		xi = r->xa;
		yi++;
	}
}
