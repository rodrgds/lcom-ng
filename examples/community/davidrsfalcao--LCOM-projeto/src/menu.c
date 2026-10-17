#include "menu.h"

extern char irq_set;

void data_sistema()
{
	RTC_subscribe_int();
	clean_buffer();
	Set_RTC();

	unsigned short resto;
	unsigned short quociente;
	int height;
	int width;

	int i1 = 0;
	quociente = horas();
	int x = 750;

	while (i1 < 2) {

		resto = quociente % 10;

		switch (resto) {

		case 0:
			xpm_cre(&height, &width, x, 730, zeroh);
			break;

		case 1:
			xpm_cre(&height, &width, x, 730, oneh);
			break;

		case 2:
			xpm_cre(&height, &width, x, 730, twoh);
			break;

		case 3:
			xpm_cre(&height, &width, x, 730, threeh);
			break;

		case 4:
			xpm_cre(&height, &width, x, 730, fourh);
			break;

		case 5:
			xpm_cre(&height, &width, x, 730, fiveh);
			break;

		case 6:
			xpm_cre(&height, &width, x, 730, sixh);
			break;

		case 7:
			xpm_cre(&height, &width, x, 730, sevenh);
			break;

		case 8:
			xpm_cre(&height, &width, x, 730, eighth);
			break;

		case 9:
			xpm_cre(&height, &width, x, 730, nineh);
			break;
		}

		quociente = quociente / 10;
		x -= 16;
		i1++;

	}

	xpm_cre(&height, &width, x+48, 730+2, pontos);
	x += 80;
	quociente = minutos();
	int i2 = 0;

	while (i2 < 2) {

			resto = quociente % 10;

			switch (resto) {
			case 0:
				xpm_cre(&height, &width, x, 730, zeroh);
				break;

			case 1:
				xpm_cre(&height, &width, x, 730, oneh);
				break;

			case 2:
				xpm_cre(&height, &width, x, 730, twoh);
				break;

			case 3:
				xpm_cre(&height, &width, x, 730, threeh);
				break;

			case 4:
				xpm_cre(&height, &width, x, 730, fourh);
				break;

			case 5:
				xpm_cre(&height, &width, x, 730, fiveh);
				break;

			case 6:
				xpm_cre(&height, &width, x, 730, sixh);
				break;

			case 7:
				xpm_cre(&height, &width, x, 730, sevenh);
				break;

			case 8:
				xpm_cre(&height, &width, x, 730, eighth);
				break;

			case 9:
				xpm_cre(&height, &width, x, 730, nineh);
				break;
			}

			quociente = quociente / 10;
			x -= 16;
			i2++;

		}

	xpm_cre(&height, &width, x+48, 730+2, pontos);
	x += 80;
	quociente = segundos();
	i2 = 0;

	while (i2 < 2) {

		resto = quociente % 10;

		switch (resto) {
		case 0:
			xpm_cre(&height, &width, x, 730, zeroh);
			break;
		case 1:
			xpm_cre(&height, &width, x, 730, oneh);
			break;

		case 2:
			xpm_cre(&height, &width, x, 730, twoh);
			break;

		case 3:
			xpm_cre(&height, &width, x, 730, threeh);
			break;

		case 4:
			xpm_cre(&height, &width, x, 730, fourh);
			break;

		case 5:
			xpm_cre(&height, &width, x, 730, fiveh);
			break;

		case 6:
			xpm_cre(&height, &width, x, 730, sixh);
			break;

		case 7:
			xpm_cre(&height, &width, x, 730, sevenh);
			break;

		case 8:
			xpm_cre(&height, &width, x, 730, eighth);
			break;

		case 9:
			xpm_cre(&height, &width, x, 730, nineh);
			break;
		}

		quociente = quociente / 10;
		x -= 16;
		i2++;

	}





	 RTC_unsubscribe_int();
}

void draw_opt(img ti, unsigned short option)
{
	if (option)
	{
		ti.y = 320;
		xpm_cre(&ti.height, &ti.width, ti.x, ti.y, ti.sprite);
	}
	else
	{
		ti.y = 250;
		xpm_cre(&ti.height, &ti.width, ti.x, ti.y, ti.sprite);

	}

}

void draw_img(img ti)
{
	xpm_cre(&ti.height, &ti.width, ti.x, ti.y, ti.sprite);

}

int menu()
{
	img t1 = {212,0,0,0,titulo};
	img ng = {412,250,0,0,botao1};
	img sr = {412,320,0,0,botao2};
	img cm1 = {750,15,0,0,c1};
	img cm2 = {700,550,0,0,c1};
	img opt = {350, 250,0,0,apontador};

	// subscribes
	int key = 0;
	int r;
	int ipc_status;
	message msg;


	unsigned char timer_hook_bit;
	if (timer_hook_bit = timer_subscribe_int() < 0) {
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

					if ((key == UP_KEY) || (key == DOWN_KEY))
					{
						option = !option;
					}
					if (key == ENTER) {
						if (option)
							return 1;
						else
							return 0;
					}

				}

				if (msg.NOTIFY_ARG & BIT(timer_hook_bit))
				{
					clear_screan();
					updateBuffer();
					clear_double();
					draw_img(cm1);
					draw_img(t1);
					draw_img(ng);
					draw_img(sr);
					draw_img(cm2);
					data_sistema();
					draw_opt(opt, option);
				}
			}
		}

		if (key == KBD_ESC_KEY)
			break;

		key = 0;


	}


	return 1;

}
