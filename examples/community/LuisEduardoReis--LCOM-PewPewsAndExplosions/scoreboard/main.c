#include <minix/syslib.h>
#include <minix/drivers.h>

#include "Serial.h"
#include "video_gr.h"
#include <time.h>

#define BCK_COLOR 0x34
#define SCR_COLOR 0x36

void drawScore(int score);

int main(int argc, char** argv){
	sef_startup();

	ser_disable_int(COM2_BASE, IER_RECIEVEDDATA);
	vg_init(0x101);
	vg_fill(BCK_COLOR);

	printf("ScoreBoard On\n");
	char val[4];
	while(1) {
		int r = ser_poll_recieve_int(COM2_BASE, 8,1,0, 9600);
		usleep(20000);

		if (r == -1) continue;
		if (r == -2) break;

		vg_fill(BCK_COLOR);
		drawScore(r);
	}

	vg_exit();

	printf("Off\n");
	return 0;
}




char digits[10][7][5] = {{
"0000",
"0  0",
"0  0",
"0  0",
"0  0",
"0  0",
"0000",},
{
"   0",
"   0",
"   0",
"   0",
"   0",
"   0",
"   0",},
{
"0000",
"   0",
"   0",
"0000",
"0   ",
"0   ",
"0000",},
{
"0000",
"   0",
"   0",
"0000",
"   0",
"   0",
"0000",},
{
"0  0",
"0  0",
"0  0",
"0000",
"   0",
"   0",
"   0",},
{
"0000",
"0   ",
"0   ",
"0000",
"   0",
"   0",
"0000",},
{
"0000",
"0   ",
"0   ",
"0000",
"0  0",
"0  0",
"0000",},
{
"0000",
"   0",
"   0",
"   0",
"   0",
"   0",
"   0",},
{
"0000",
"0  0",
"0  0",
"0000",
"0  0",
"0  0",
"0000",},
{
"0000",
"0  0",
"0  0",
"0000",
"   0",
"   0",
"   0",},
};


void drawScore(int score) {
	char score_str[32]; int n;
	sprintf(score_str,"%d%n",score, &n);

	char c; int i, l = 24;
	for(i = 0; (c = score_str[i]) != 0 && i < 5; i++) {
		int x,y, ox,oy;
		ox = 640/2 + i*5*l - (5*l*n)/2;
		oy = 64;
		for(y = 0; y < 7; y++) {
			for(x = 0; x < 4; x++) {
				if (digits[c - '0'][y][x] == '0')
				vg_square(l*x + ox, l*(x+1) + ox,
						  l*y + oy, l*(y+1) + oy, SCR_COLOR );
		}}
	}
}





