#include "handler.h"

unsigned int hook_id_timer = 0;
unsigned int hook_id_kbd = 1;
unsigned int m_hook_id = 12;
unsigned int hook_id_rtc = 2;
int count_int = 0;

unsigned char packets[3];
static int timer_handle, keyboard_handle, mouse_handle, rtc_handle;

int kbd_subscribe_int(void) {
	int temp = hook_id_kbd;
	if ((sys_irqsetpolicy(KBD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &temp))
			!= OK)
		return -1;
	if(sys_irqenable(&temp) != OK)
		return -1;
	else {
		keyboard_handle = temp;
		return BIT(hook_id_kbd);
	}
}

int kbd_unsubscribe_int(void) {
	int r = sys_irqrmpolicy(&keyboard_handle);
	if (r != 0)
		return -1;
	else
		return r;
}

int mouse_subscribe_int() {
	int temp = m_hook_id;
	if ((sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &temp))
			!= OK)
		return -1;
	else {
		mouse_handle = temp;
		return BIT(m_hook_id);
	}
}

int mouse_unsubscribe_int() {
	if (OK == sys_irqdisable(&mouse_handle))
		if (OK == sys_irqrmpolicy(&mouse_handle))
			return 0;
	return -1;
}

int timer_subscribe_int(void ) {
	int temp = hook_id_timer;
	if(sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &temp) != OK)
		return -1;
	sys_irqenable(&temp);
	timer_handle = temp;
	return BIT(hook_id_timer);
}

int timer_unsubscribe_int() {
	sys_irqdisable(&timer_handle);
	return(sys_irqrmpolicy(&timer_handle));
}

int rtc_subscribe_int() {
	int temp = hook_id_rtc;
	if ((sys_irqsetpolicy(RTC_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &temp)) != OK)
		return -1;
	sys_irqenable(&temp);
	rtc_handle = temp;
	return BIT(hook_id_rtc);
}

int rtc_unsubscribe_int() {
	int r = sys_irqrmpolicy(&rtc_handle);
	if (r != 0)
		return -1;
	else
		return r;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int menu(){
	Menu *menu = malloc(sizeof(Menu));
	if(menu == NULL)
		return -1;
	Rectangle *a1 = malloc(sizeof(Rectangle));
	if(a1 == NULL)
		return -1;
	Rectangle *a2 = malloc(sizeof(Rectangle));
	if(a2 == NULL)
		return -1;

	Cursor *c = malloc(sizeof(Cursor));
	if(c == NULL)
		return -1;

	Footer *rod = malloc(sizeof(Footer));
	if(rod == NULL)
		return -1;

	a1->xi = 409;
	a1->yi = 150;
	a1->pix = read_xpm(play,&a1->wd, &a1->ht);

	a2->xi = 409;
	a2->yi = 350;
	a2->pix = read_xpm(fechar,&a2->wd,&a2->ht);

	c->size = 15;
	c->color = 10;
	c->color_p = 50;
	c->x = 0;
	c->y = 0;

	rod->xa = 335;
	rod->ya = 735;
	rod->pix = read_xpm(rodape,&rod->wd, &rod->ht);
	rod->speed = 2;

	menu->Opcao1 = a1;
	menu->Opcao2 = a2;
	menu->checkOpcao1 = 0;
	menu->checkOpcao2 = 0;

	menu->xtitulo = 311;
	menu->ytitulo = 10;
	menu->pix = read_xpm(titulo, &menu->wd, &menu->ht);
	menu->cursor = c;
	menu->footer = rod;

	if(timer_subscribe_int() == -1)
		return 1;
	if(kbd_subscribe_int() == -1)
		return 1;
	if(mouse_subscribe_int() == -1)
		return 1;
	int r, ipc_status;
	message msg;
	unsigned long irq_set_kbd = BIT(hook_id_kbd);
	unsigned long irq_set_timer = BIT(hook_id_timer);
	unsigned long irq_set_mouse = BIT(m_hook_id);
	unsigned long data;
	int end = 0;
	int count = 1;
	menu_show(menu);
	actVideoMem();
	mouse_set_stream();
	while(end == 0){
		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) {
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE:
				if(msg.NOTIFY_ARG & irq_set_timer){
					clear_footer(rod);
					footer_mov(rod);
					paint_footer(rod);
					actVideoMem();
				}
				if (msg.NOTIFY_ARG & irq_set_kbd) {
					data = kbd_int_handler();
					if (data == BC_ESC) { //EXIT MENU
						end = 1;
						break;
					}
				}
				if(msg.NOTIFY_ARG & irq_set_mouse){
					int d = mouse_handler(&packets);
					count -= d;
					if(count == 0){
						count = 1;
						//clear_screen();
						clear_cursor(menu->cursor);
						updateCursor(menu->cursor,packets);
						checkCursor(menu);
						if(menu->checkOpcao1 == 1){
							if(mouse_disable_stream() != 1)
								return -1;
							//ENABLE STREAM MODE AGAIN
							unsigned long stat;
							//Write command to mouse
							sys_inb(KBC_CMD_REG,&stat);
							while(1){
								if((stat & IBF) == 0){
									sys_outb(KBC_CMD_REG, WRITE_MOUSE); //0xD4
									break;
								}
								tickdelay(micros_to_ticks(DELAY_US));
							}
							while(1){
								sys_inb(KBC_CMD_REG,&stat);
								if((stat & IBF) == 0){
									sys_outb(IN_BUF, STREAM_MODE); //0xF5
									break;
								}
								tickdelay(micros_to_ticks(DELAY_US));
							}
							sys_inb(OUT_BUF, &stat);
							mouse_unsubscribe_int();
							timer_unsubscribe_int();
							kbd_unsubscribe_int();
							free(a1);
							free(a2);
							free(rod);
							free(c);
							free(menu);
							clear_screen();
							game_handler();
							return 1;
						}
						menu_show(menu);
						actVideoMem();
						if(menu->checkOpcao2 == 1){
							end = 1;
							break;
						}
					}
				}
				break;
			default:
				break;
			}
		} else {}
	}
	if(mouse_disable_stream() != 1)
		return -1;

	//ENABLE STREAM MODE AGAIN
	unsigned long stat;
	//Write command to mouse
	sys_inb(KBC_CMD_REG,&stat);
	while(1){
		if((stat & IBF) == 0){
			sys_outb(KBC_CMD_REG, WRITE_MOUSE); //0xD4
			break;
		}
		tickdelay(micros_to_ticks(DELAY_US));
	}
	while(1){
		sys_inb(KBC_CMD_REG,&stat);
		if((stat & IBF) == 0){
			sys_outb(IN_BUF, STREAM_MODE); //0xF5
			break;
		}
		tickdelay(micros_to_ticks(DELAY_US));
	}
	sys_inb(OUT_BUF, &stat);

	mouse_unsubscribe_int();
	kbd_unsubscribe_int();
	timer_unsubscribe_int();
	free(a1);
	free(a2);
	free(c);
	free(rod);
	free(menu);
	free(buffer);
	vg_exit();
	return 1;
}

void reset_game(Bar *b1, Bar *b2, Ball *a){

	b1->wd = 20;
	b1->ht = 100;
	b1->ya = 334;
	b1->xa = 30;
	b1->speed = 5;
	b1->press = 0;

	b2->wd = 20;
	b2->ht = 100;
	b2->ya = 334;
	b2->xa = 976;
	b2->speed = 5;
	b2->press = 0;

	a->side = 20;
	a->x = 502;
	a->y = 374;
	a->speed = 6;
	a->angle = 1;
	a->dir = rand()%2;
	a->flag_score = 0;
}

int game_handler(){
	Bar *b1 = malloc(sizeof(Bar));
	if(b1 == NULL)
		return -1;
	Bar *b2 = malloc(sizeof(Bar));
	if(b2 == NULL)
		return -1;

	Ball *a = malloc(sizeof(Ball));
	if(a == NULL)
		return -1;

	b1->wd = 20;
	b1->ht = 100;
	b1->ya = 334;
	b1->xa = 30;
	b1->speed = 5;
	b1->press = 0;
	b1->score = 0;

	b2->wd = 20;
	b2->ht = 100;
	b2->ya = 334;
	b2->xa = 976;
	b2->speed = 5;
	b2->press = 0;
	b2->score = 0;

	a->side = 20;
	a->x = 502;
	a->y = 374;
	a->speed = 6;
	a->angle = 1;
	a->vert = 0;
	a->dir = rand()%2;
	a->flag_score = 0;


	game_show(b1,b2,a);
	actVideoMem();
	game(b1,b2,a);
}

int game(Bar *b1, Bar *b2, Ball *a){
	if(kbd_subscribe_int() == -1)
		return 1;
	if(timer_subscribe_int() == -1)
		return 1;
	if(rtc_subscribe_int() == -1)
		return 1;
	int r, ipc_status;
	message msg;
	rtc_set_alarm();
	unsigned long irq_set_kbd = BIT(hook_id_kbd);
	unsigned long irq_set_timer = BIT(hook_id_timer);
	unsigned long irq_set_rtc = BIT(hook_id_rtc);
	unsigned long data;
	int end = 0, time = 60;
	while(end == 0 && time != 0){
		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) {
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE:
				if(msg.NOTIFY_ARG & irq_set_rtc){
					rtc_handler();
					rtc_set_alarm();
					if(a->speed != 6){
						a->speed = 6;
					}
					else
						a->speed = 15;
				}
				if(msg.NOTIFY_ARG & irq_set_timer){
					if(a->flag_score){
						clear_bar(b1,b2,0);
						clear_bar(b1,b2,1);
						clear_ball(a);
						reset_game(b1,b2,a/*back*/);
						ball_movement(b1,b2,a);
						show_score(b1,b2);
						paint_ball(a);
						paint_bar(b1);
						paint_bar(b2);
					}
					else {
						clear_ball(a);
						ball_movement(b1,b2,a);
						paint_ball(a);
						if(b1->press == 1){
							clear_bar(b1,b2,0);
							movement(b1);
							paint_bar(b1);
						}
						if(b2->press == 1){
							clear_bar(b1,b2,1);
							movement(b2);
							paint_bar(b2);
						}
						count_int++;
						if((count_int%60) == 0){
							time--;
							show_time(time);
						}
					}
					actVideoMem();
				}
				if (msg.NOTIFY_ARG & irq_set_kbd) {
					data = kbd_int_handler();
					if (data == BC_ESC) { //EXIT GAME
						end = 1;
						break;
					}
					if ((data & BREAK_CODE) == BREAK_CODE){
						if((data == KEY_O_B && b2->scancode == KEY_K) || (data == KEY_K_B && b2->scancode == KEY_O)){
							b2->press = 1;
						}
						else if((data == KEY_W_B && b1->scancode == KEY_S) || (data == KEY_S_B && b1->scancode == KEY_W)){
							b1->press = 1;
						}
						else{
							if((data == KEY_W_B) || (data == KEY_S_B)){
								b1->press = 0;
							}
							if((data == KEY_O_B) || (data == KEY_K_B)){
								b2->press = 0;
							}
						}
					}
					if ((data & BREAK_CODE) == 0) {
						if((data == KEY_W) || (data == KEY_S)){
							b1->press = 1;
							b1->scancode = data;
						}
						if((data == KEY_O) || (data == KEY_K)){
							b2->press = 1;
							b2->scancode = data;
						}
					}
				}
				break;
			default:
				break;
			}
		} else {
		}
	}
	timer_unsubscribe_int();
	kbd_unsubscribe_int();
	rtc_unsubscribe_int();
	free(a);
	winner(b1,b2);
}

void winner(Bar *b1, Bar *b2){
	clear_screen();
	int wd, ht;
	char *p;
	if(b1->score > b2->score){
		p = (char *) read_xpm(winnerx, &wd, &ht);
		test_xpm_show(179, 200, wd, ht, p);
		p = (char *) read_xpm(player1, &wd, &ht);
		test_xpm_show(393, 400, wd, ht, p);
	}
	else if(b1->score < b2->score){
		p = (char *) read_xpm(winnerx, &wd, &ht);
		test_xpm_show(179, 200, wd, ht, p);
		p = (char *) read_xpm(player2, &wd, &ht);
		test_xpm_show(389, 400, wd, ht, p);
	}
	else{
		p = (char *) read_xpm(draw, &wd, &ht);
		test_xpm_show(290, 337, wd, ht, p);
	}
	actVideoMem();
	sleep(2);
	clear_screen();
	free(b1);
	free(b2);
	menu();
}


int main(){
	sef_startup();
	sys_enable_iop(SELF);

	vg_init(LINEAR_FRAME);
	menu();
	free(buffer);
	return 1;
}
