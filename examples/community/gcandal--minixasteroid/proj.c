#include "proj.h"

//int kbdhook=5, timerhook=2, mousehook=3, rtc_hook=4;
//
//Cores:
//27 pilhadentro
//17 interior
//25 exterior
//43 nave
//63 asteroid
//30 shot

int main(int argc, char **argv) {

	printf("Comecouu\n");

	//Used for peripheric functioning (including interrupts)
	endpoint_t ep;
	char name[256];
	int priv_f;
	int ipc_status;
	unsigned long stat;
	message msg;

	//Setting the system to exclusive peripheric use by the program
	sef_startup();
	sys_whoami(&ep, name, 256, &priv_f);
	sys_enable_iop(ep);
	mouse_subscribe_int();
	rtc_subscribe_int();
	timer_subscribe_int();
	kbd_subscribe_int();

	vg_init(0x105);
	vg_fill(BACKGROUND);
	vg_memtobuffer();

	//Initializing sprites, using randomness on asteroids
	batteries[0]=create_sprite(battery,BATTERYXPOS,BATTERYYPOS);
	draw_sprite(batteries[0]);
	batteries[1]=create_sprite(batteryinside,BATTERYXPOS+83,BATTERYYPOS+12);
	draw_sprite(batteries[1]);
	batteries[2]=create_sprite(batteryinside,BATTERYXPOS+63,BATTERYYPOS+12);
	draw_sprite(batteries[2]);
	batteries[3]=create_sprite(batteryinside,BATTERYXPOS+43,BATTERYYPOS+12);
	draw_sprite(batteries[3]);
	batteries[4]=create_sprite(batteryinside,BATTERYXPOS+23,BATTERYYPOS+12);
	draw_sprite(batteries[4]);

	timesprt[2]=create_sprite(digits[0],0,0);
	draw_sprite(timesprt[2]);
	timesprt[1]=create_sprite(digits[0],50,0);
	draw_sprite(timesprt[1]);
	timesprt[0]=create_sprite(digits[0],100,0);
	draw_sprite(timesprt[0]);

	int i;
	srand(time(NULL));

	for(i=0; i<4; i++) {
		shotsprt[i]=create_sprite(shot,V_RES+500,0);
	}
	for(i=0; i<NUM_ASTEROIDS; i++) {
		asteroids[i]=create_sprite(asteroid,rand()%1024,-90);
		asteroids[i]=create_sprite(asteroid,rand()%1024, -90*((i%9)+1) );
	}

	battleship=create_sprite(spaceship,posx_inicial,posy_inicial);
	draw_battleship(battleship);

	vg_buffertomem();



	do {
		if ( driver_receive(ANY, &msg, &ipc_status) != 0 ) {
			printf("driver_receive failed with: %d", 55555);
			continue;
		}
		if (is_ipc_notify(ipc_status)) {
			switch (_ENDPOINT_P(msg.m_source)) {

			case HARDWARE:
				if (msg.NOTIFY_ARG & 4) {

					timer_int_handler();
					intcounter++;
				}

				if (msg.NOTIFY_ARG & 32) {

					int response=kbd_int_handler(shots);

					if(response!=-1)
						if(response) {
							shotsprt[4-shots]->x=(battleship->x)+17;
							shotsprt[4-shots]->y=battleship->y;
							shots--;
							changed=1;
						} else finished=5;

				}

				if (msg.NOTIFY_ARG & 16) {
					readtime=rtc_int_handler(readtime,timesalarm,DELTA);

					if(!readtime) {
						printf("Alarm!\n");
						asteroidvel++;
					}

				}

				if (msg.NOTIFY_ARG & 8) {
					changed=mouse_int_handler(&(battleship->x),&(battleship->y),&posx_inicial,&posy_inicial,H_RES,V_RES);
				}

				break;

			default:
				break;
			}
		}
	} while(finished<4);

	vg_fill_buffer(BACKGROUND);

	//Setting GameOver screen, repositioning timesprt
	game_over=create_sprite(gameover,(H_RES/2)-150,(V_RES/2)-60);
	draw_sprite(game_over);
	for(i=0; i<3; i++) {
		timesprt[i]->x+=(H_RES/2)-50;
		timesprt[i]->y+=(V_RES/2)+30;
		draw_sprite(timesprt[i]);
	}
	vg_buffertomem();

	//Delaying program execution, to give the user time to see the score (which is the time he survived)
	intcounter=0;
	do {
			if ( driver_receive(ANY, &msg, &ipc_status) != 0 ) {
				printf("driver_receive failed with: %d", 55555);
				continue;
			}
			if (is_ipc_notify(ipc_status)) {
				switch (_ENDPOINT_P(msg.m_source)) {

				case HARDWARE:
					if (msg.NOTIFY_ARG & 4) {
						intcounter++;
					}
				}
			}
	}while(intcounter<210);

	//Returning the system to its normal state
	rtc_unsubscribe_int();
	mouse_unsubscribe_int();
	timer_unsubscribe_int();
	kbd_unsubscribe_int();
	vg_exit();

	return 0;
}

void timer_int_handler() {
	int a,b;

	short tobedestroyed[NUM_ASTEROIDS]={0}, destshots[5]={0};

	if(changed==1) {
		finished+=draw_battleship(battleship);

		for(a=0; a<3; a++) {
			draw_sprite(timesprt[a]);
		}
		for(a=0; a<=shots; a++) {
			draw_sprite(batteries[a]);
		}

		vg_buffertomem();
		vg_fill_buffer(BACKGROUND);
		changed=0;
	}

	if(intcounter%ASTEROID_PERIOD==0 && finished<4) {

		for(a=4-shots; a>0; a--) {
			draw_sprite(shotsprt[a-1]);
		}

		for(a=0; a<NUM_ASTEROIDS; a++) {
			asteroids[a]->y+=asteroidvel;

			if(asteroids[a]->y>768) {
				asteroids[a]->y=-90;
				asteroids[a]->x=rand()%1024;
			}else tobedestroyed[a]=draw_asteroid(asteroids[a]);

		}

		for(a=0; a<NUM_ASTEROIDS; a++)
			if(tobedestroyed[a]) {
				erase_sprite(asteroids[a],0);

				for(b=0; b<5; b++)
					if(destshots[b]==0) {
						destshots[b]=(asteroids[a]->y)+56;
						break;
					}


				asteroids[a]->x=V_RES+500;
				tobedestroyed[a]=0;
			}

		for(a=4; a>0; a--) {
			for(b=0; b<5; b++)
				if(shotsprt[a-1]->y-destshots[b]<30) {
					erase_sprite(shotsprt[a-1],0);
					shotsprt[a-1]->x=V_RES+500;
				}

			shotsprt[a-1]->y-=shotvel;
		}

		changed=1;
	}

	if(intcounter%SHOT_PERIOD==0 && shots<5) {
		if(shots<4)
			shots++;

		changed=1;
	}

	if(intcounter%120==0) {

		if(times[0]==9) {
			times[0]=0;

			if(times[1]==9) {
				times[1]=0;

				if(times[2]==9)
					times[2]=0;
				else times[2]++;

			}else times[1]++;

		}else times[0]++;

		for(a=0; a<3; a++) {
			timesprt[a]=create_sprite(digits[times[a]],(2-a)*50,0);
		}

		changed=1;
	}
}
