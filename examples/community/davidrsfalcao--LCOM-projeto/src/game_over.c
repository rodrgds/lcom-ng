#include "game_over.h"

extern char irq_set;
extern unsigned long points;


void draw_option(image ti, unsigned short option)
{
	if (option)
	{
		ti.y = 372;
		xpm_cre(&ti.height, &ti.width, ti.x, ti.y, ti.sprite);
	}
	else
	{
		ti.y = 312;
		xpm_cre(&ti.height, &ti.width, ti.x, ti.y, ti.sprite);
	}

}

void pontuation()
{

	int quociente = points, resto;
	int x = 580;
	int height;
	int width;

	int i = 3;

	while (i > 0) {
		resto = quociente % 10;

		switch (resto) {
		case 0:
			xpm_cre(&height, &width, x, 215, zero);
			break;

		case 1:
			xpm_cre(&height, &width, x, 215, one);
			break;

		case 2:
			xpm_cre(&height, &width, x, 215, two);
			break;

		case 3:
			xpm_cre(&height, &width, x, 215, three);
			break;

		case 4:
			xpm_cre(&height, &width, x, 215, four);
			break;

		case 5:
			xpm_cre(&height, &width, x, 215, five);
			break;

		case 6:
			xpm_cre(&height, &width, x, 215, six);
			break;

		case 7:
			xpm_cre(&height, &width, x, 215, seven);
			break;

		case 8:
			xpm_cre(&height, &width, x, 215, eight);
			break;

		case 9:
			xpm_cre(&height, &width, x, 215, nine);
			break;

		}

		quociente = quociente / 10;
		x = x - 40;
		i--;

	}
}

void draw_image(image ti)
{
	xpm_cre(&ti.height, &ti.width, ti.x, ti.y, ti.sprite);

}

int game_over()
{

	image t1 = {184,100,0,0,game_over_img}; // imagem game over
	image t2 = {487,300,0,0,reload}; // imagem reload
	image t3 = {487,360,0,0,img_exit}; // imagem exit
	image t4 = {340,235,0,0,score}; // SCORE:
	image t5 = {455,0,0,0,alv};


	// subscribes
	int key = 0;
	int r;

	int ipc_status;
	message msg;


	unsigned char timer_hook_bit;
	if (timer_hook_bit = timer_subscribe_int() < 0) {
		vg_exit();
		printf("Timer not subscribed!\n");
		return 1;
	}

	unsigned short option = 0;

	while (1) {
		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) {
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE:

				if (msg.NOTIFY_ARG & irq_set) {
					kbd_scan_c(&key);

					if ((key == UP_KEY) || (key == DOWN_KEY)) {
						option = !option;
					}
					if (key == ENTER) {
						if (option)
							return 1;
						else
						{
							points = 0;
							return 0;
						}
					}

				}

				if (msg.NOTIFY_ARG & BIT(timer_hook_bit))
				{
					clear_screan();
					updateBuffer();
					clear_double();
					draw_image(t1);
					draw_image(t2);
					draw_image(t3);
					draw_image(t4);
					pontuation();
					draw_option(t5,option);

				}
			}
		}


		if (key == KBD_ESC_KEY)
			break;

		key = 0;

	}

	return 1;

}
