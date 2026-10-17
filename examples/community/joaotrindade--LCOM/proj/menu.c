#include "menu.h"


void drawMenu(int erase)
{
	// ERASE : 1 APAGA
	int width, height, x, y;
	char *menu;
	menu = (char*)read_xpm(menu_pix, &width, &height);

	for(x = 256; x < height + 256 ; x++)
		{
			for(y = 341; y <width+341; y++, menu++)
			{
				if(erase == 0) vg_set_pixel(y,x,*menu);
				else vg_set_pixel(y,x,0x00);
				//printf("entrou\n");
			}
		}
}

void drawCursor(int pos, int erase)
{
	// ERASE : 1 APAGA
	int width, height, x, y;
	int start_x, start_y;
	char *cursor;
	cursor = (char*)read_xpm(cursor_mono, &width, &height);

	start_x = 256 + 82 - 5 ;
	start_y = 341 + 85 - 15; //Pos inicial pixmap menu + avanço da letra - tamanho cursor
	if (pos == 2) start_x += 45;
	if (pos == 3) start_x += 100;
	for(x = start_x; x < height + start_x ; x++)
	{
		for(y = start_y; y <width + start_y; y++, cursor++)
		{
			if(erase == 0) vg_set_pixel(y,x,*cursor);
			else vg_set_pixel(y,x,0x00);
			//printf("entrou\n");
		}
	}

}


/*

 CONJUNTO DE CODIGO PARA TIRAR A DATA

 	 long int dia1, mes1, ano1, hora1, minuto1;
	long int *p_dia, *p_mes, *p_ano, *p_hora, *p_minuto;
	//printf("entrou apontadores\n");
	p_dia = &dia1;
	p_mes = &mes1;
	p_ano = &ano1;
	p_hora = &hora1;
	p_minuto = &minuto1;
	//printf("passou apontadores\n");
	get_data(p_dia,p_mes,p_ano,p_hora,p_minuto);
	//printf("saiu");
	printf("%x - %x - %x ! %x : %x",dia1,mes1,ano1,hora1,minuto1);

 */

int main(){

	int ipc_status, irq_set, option, exit;
	unsigned char scancode;
	message msg;
	option = 1;
	exit = 0;
	//sef_startup();


	vg_init(0x105);
	irq_set = kbc_subscribe_int();
	timer_subscribe_int();

	drawMenu(0);
	drawCursor(option,0);
	 long int dia1, mes1, ano1, hora1, minuto1;
	long int *p_dia, *p_mes, *p_ano, *p_hora, *p_minuto;
	//printf("entrou apontadores\n");
	p_dia = &dia1;
	p_mes = &mes1;
	p_ano = &ano1;
	p_hora = &hora1;
	p_minuto = &minuto1;
	//printf("passou apontadores\n");
	get_data(p_dia,p_mes,p_ano,p_hora,p_minuto);
	//printf("saiu");
	printf("%x - %x - %x ! %x : %x",dia1,mes1,ano1,hora1,minuto1);

	while(exit != 1) {
		if(driver_receive(ANY, &msg, &ipc_status) != 0) {
			printf("driver_receive failed\n");
			continue;
		}
		if (is_ipc_notify(ipc_status)) {

			switch (_ENDPOINT_P(msg.m_source)) {
				case HARDWARE :
					if (msg.NOTIFY_ARG & irq_set)
					{
						scancode = read_scancode();
						if (scancode == 0x48) // FOUND SETA CIMA
						{
							//CENAS
							if (option > 1)
							{
								drawCursor(option,1);
								option--;
								drawCursor(option,0);

							}
						}
						if (scancode == 0x50) // FOUND SETA BAIXO
						{
							if (option < 3)
							{
								drawCursor(option,1);
								option++;
								drawCursor(option,0);
							}

						}
						if (scancode == 0x39) // FOUND B.ESPACOS
						{
							if (option == 1) // NOVO JOGO
							{
								jogo(irq_set, ipc_status, msg);
								vg_fill(0x00);
								drawMenu(0);
								drawCursor(option,0);

							}
							if (option == 2) // HIGHSCORES
							{

								print_scores(irq_set, ipc_status, msg);
								vg_fill(0x00);
								drawMenu(0);
								drawCursor(option,0);

							}
							if (option == 3) exit = 1;

						}
					}
			}
		}
	}
	kbc_unsubscribe_int();
	timer_unsubscribe_int();
	vg_exit();
	return 0;

}


