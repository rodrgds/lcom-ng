#include <minix/syslib.h>
#include <minix/drivers.h>



#include "game.h"

extern entity* player;
extern int WIDTH,HEIGHT;
extern int score;

char* filepath;

int init_Path(char** dest, char* servicePath);

int update_scoreboard();
int stop_scoreboard();



int main(int argc, char** argv){
	sef_startup();
	if (init_Path(&filepath, argv[0]) != 0) return 1;

	if (init() != 0) return 1;

	kbd_set_leds(0xFF);
	LoadScores();

	int exit = 0;
	int ipc_status, r;
	message msg;
	while(!exit) {
	    /* Get a request message. */
	    if ( driver_receive(ANY, &msg, &ipc_status) != 0 ) {
	        printf("driver_receive failed with: %d", r);
	        continue;
	    }
	    if (is_ipc_notify(ipc_status) && _ENDPOINT_P(msg.m_source) == HARDWARE) { /* received notification */
	    	//Keyboard Interrupt
	    	if (msg.NOTIFY_ARG & 1<<KBD_INT_BIT) {
	    	   	char in;
	    	   	kbd_int_handler(&in); //test for input error
	    	   	if (in == KEY_ESC_DOWN) exit = 1;

	    	   	input_handle_kbd(in);
	    	}
	    	//Mouse Interrupt
	    	if (msg.NOTIFY_ARG & 1<<MOUSE_INT_BIT) {
	    	   	mousePacket packet;
	    	   	if(mouse_int_handler(&packet) == 0)
	    	   		input_handle_mouse(packet);
	    	}

	        //Timer Interrupt
	        if (msg.NOTIFY_ARG & 1<<TIMER_INT_BIT) {
	        	// Tick
	        	input_swap_buffers();

	        	tickEntities();
	        	gameTick();
	        	bruteForceCollisions();
	          	destroyEntities();

	          	//Draw
	          	vg_fill(0x000000);
	          	drawBackground();
	        	drawEntities();
	        	gameDraw();
	        	vg_swap_buffers();

	        	update_scoreboard();
	        }

	    }
	}

	stop();
	stop_scoreboard();
	SaveScores();
	free(filepath);

	printf("Thank you for Playing ;)\n");

	return 0;
}


static int update_score = -1;
int update_scoreboard() {
	if (score!=update_score) {
		ser_poll_send_int(COM2_BASE,8,1,0,9600, score);
		update_score = score;
   	}
}
int stop_scoreboard() {
	ser_poll_send_int(COM2_BASE,8,1,0,9600, -2);
}

int init_Path(char** dest, char* servicePath) {
	char* l = strrchr(servicePath, '/');
	if (l == NULL) return 1;
	*dest = malloc(256);
	char* p1 = servicePath, *p2 = *dest;
	while(p1 != l) {
		*p2 = *p1;
		p1++; p2++;
	}
	*p2 = 0;
	return 0;
};
