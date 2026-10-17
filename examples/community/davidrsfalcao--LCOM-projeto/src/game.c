#include "game.h"

extern char irq_set;

unsigned long global_counter = 0;
unsigned long points = 0;



void pontuation_atual() {

	int quociente = points, resto;
	int x = 900;
	int height;
	int width;

	int i = 3;

	while (i > 0) {
		resto = quociente % 10;

		switch (resto) {
		case 0:
			xpm_cre(&height, &width, x, 20, zeroh);
			break;

		case 1:
			xpm_cre(&height, &width, x, 20, oneh);
			break;

		case 2:
			xpm_cre(&height, &width, x, 20, twoh);
			break;

		case 3:
			xpm_cre(&height, &width, x, 20, threeh);
			break;

		case 4:
			xpm_cre(&height, &width, x, 20, fourh);
			break;

		case 5:
			xpm_cre(&height, &width, x, 20, fiveh);
			break;

		case 6:
			xpm_cre(&height, &width, x, 20, sixh);
			break;

		case 7:
			xpm_cre(&height, &width, x, 20, sevenh);
			break;

		case 8:
			xpm_cre(&height, &width, x, 20, eighth);
			break;

		case 9:
			xpm_cre(&height, &width, x, 20, nineh);
			break;

		}

		quociente = quociente / 10;
		x = x - 16;
		i--;

	}
}

void draw(space_ship nv1, asteroid ast1, asteroid ast2)
{
	draw_spaceship(nv1);
	draw_asteroids(ast1, ast2);
	pontuation_atual();

}

int game()
{
	space_ship nv1 = {470,690,0,0,spaceship}; //criar nave e definir posicao inicial

	short displacement = 30; // movimento mínimo da nave


	asteroid ast1 = {0,0,0,0,c1}; //criar asteroid 1 e definir posicao inical
	asteroid ast2 = {0,0,0,0,c1}; //criar asteroid 2 e definir posicao inical


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

					if (key == KBD_ESC_KEY)
						break;

					if (key == RIGHT_KEY) {
						move_spaceship(&nv1, displacement);

					}

					if (key == LEFT_KEY) {
						move_spaceship(&nv1, -displacement);
					}


					key = 0;
				}

				if (msg.NOTIFY_ARG & BIT(timer_hook_bit))
				{
					global_counter++;
					asteroid_generator(&ast1,&ast2);
					clear_screan();
					updateBuffer();
					clear_double();
					draw(nv1, ast1, ast2);
					if (collision_spaceship_asteroid(nv1,ast1,ast2))
					{
						int res = game_over();
						return res;
					}
				}
			}
		}

		if (key == KBD_ESC_KEY)
			break;


		key = 0;
	}

	return 1;
}

