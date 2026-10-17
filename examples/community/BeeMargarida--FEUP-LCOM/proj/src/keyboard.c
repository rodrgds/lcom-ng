#include "keyboard.h"


int kbd_int_handler() {
	unsigned long stat;
	unsigned long data;
	while (1) {
		sys_inb(STAT_REG, &stat); /* assuming it returns OK */
		// loop while 8042 output buffer is empty
		if (stat & OBF) {
			sys_inb(OUT_BUF, &data); /* assuming it returns OK */
			if ((stat & (PAR_ERR | TO_ERR)) == 0)
				return data;
			else
				return -1;
		}
		tickdelay(micros_to_ticks(DELAY_US));
	}
}


int movement(Bar *b){
	if(b->press == 1){
		if (b->scancode == KEY_W || b->scancode == KEY_O ){ //up movement
			if((b->ya - b->speed) <= 170){
				b->ya = 171;
			}
			else
				b->ya -= b->speed;
		}
		if (b->scancode == KEY_S || b->scancode == KEY_K){ //down movement
			if((b->ya + b->ht + b->speed) >= 767){
				b->ya = 767 - b->ht;
			}
			else
				b->ya = b->ya + b->speed;
		}
	}
}

void footer_mov(Footer *r){
    if(r->xa - r->speed <= 0){
        r->xa = 1023;
        return;
    }
    r->xa -= r->speed;
}

void ball_movement(Bar *b1, Bar *b2, Ball *a){
	getDateandHours();
	if((a->x - a->speed) <= 0){
		b2->score += 1;
		a->flag_score = 1;
		return;
	}
	if((a->x + a->speed) >= 1024){
		b1->score += 1;
		a->flag_score = 1;
		return;
	}
	if((a->x - a->speed) < b1->xa){
		a->x = a->x - a->speed;
		if(a->vert == 1)
			a->y = a->y - a->speed;
		else
			a->y = a->y + a->speed;
		b2->score += 1;
		a->flag_score = 1;
	}
	if((a->x + a->speed) > b2->xa + b2->wd){
		a->x = a->x + a->speed;
		if(a->vert == 1)
			a->y = a->y - a->speed;
		else
			a->y = a->y + a->speed;
		b1->score += 1;
		a->flag_score = 1;
	}
	if((a->x - a->speed) <= (b1->xa + b1->wd)){
		if((a->y > b1->ya && a->y < b1->ya + b1->ht) || (a->y + a->side > b1->ya && a->y + a->side < b1->ya + b1->ht)){
			a->x = a->x + a->speed;
			a->dir = 1; //right
			if(a->vert == 1){
				a->y -= a->speed + a->angle;
			}
			else{
				a->y += a->speed + a->angle;
			}
		}
	}
	if((a->x + a->side + a->speed) >= b2->xa){
		if((a->y > b2->ya && a->y < b2->ya + b2->ht) || (a->y + a->side > b2->ya && a->y + a->side < b2->ya + b2->ht)){
			a->x = a->x - a->speed;
			a->dir = 0;//left
			if(a->vert == 1){
				a->y -= a->speed + a->angle;
			}
			else {
				a->y += a->speed + a->angle;
			}
		}
	}
	if (a->y + a->speed >= 768){
		a->vert = 1;
		if(a->dir == 1){
			a->x = a->x + a->angle;
		}
		else
			a->x = a->x - a->angle;
		a->y = a->y - a->speed;
	}
	if(a->y - a->speed <= 170){
		a->vert = 0;
		if(a->dir == 1){
			a->x = a->x + a->angle;
		}
		else
			a->x = a->x - a->angle;
		a->y = a->y + a->speed;
	}
	else{
		if(a->dir == 0){
			a->x = a->x - a->speed;
			if(a->vert == 1)
				a->y = a->y - a->speed;
			else
				a->y = a->y + a->speed;

		}
		else{
			a->x = a->x + a->speed;
			if(a->vert == 1)
				a->y = a->y - a->speed;
			else
				a->y = a->y + a->speed;
		}
	}
}
