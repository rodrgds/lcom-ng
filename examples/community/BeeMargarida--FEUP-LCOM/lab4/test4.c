#include "test4.h"
#include "timer.h"

unsigned int m_hook_id = 12, hook_id_timer=0, counter;
unsigned char packet[3];
long dx,dy;

int mouse_subscribe_int() {

	if ((sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &m_hook_id))
			< 0)
		return -1;
	else
		return MOUSE_IRQ;

}

int mouse_unsubscribe_int() {
	if (OK == sys_irqdisable(&m_hook_id))
		if (OK == sys_irqrmpolicy(&m_hook_id))
			return 0;
	return -1;
}

void mouse_print() {
	printf("B1=%lu", packet[0]);
	printf(" B2=%lu", packet[1]);
	printf(" B3=%lu", packet[2]);
	unsigned int xov, yov,lb,mb,rb;
	int x,y;
	if ((LB & packet[0]) == 0){
		lb = 0;
	}
	else
		lb = 1;
	printf(" LB=%u", lb);

	if ((MB & packet[0]) == 0){
		mb = 0;
	}
	else
		mb = 1;
	printf(" MB=%u", mb);

	if ((RB & packet[0]) == 0){
		rb = 0;
	}
	else
		rb = 1;
	printf(" RB=%u", rb);

	if ((XOV & packet[0]) == 0){
		xov = 0;
	}
	else
		xov = 1;
	printf(" XOV=%u", xov);

	if ((YOV & packet[0]) == 0){
		yov = 0;
	}
	else
		yov = 1;
	printf(" YOV=%u", yov);

	if((XSIGN & packet[0]) != 0){
		x = (0xFF & ~packet[1]) + 1;
		printf(" X=-%d", x);
	}
	else
		printf(" X=%lu", packet[1]);

	if((YSIGN & packet[0]) != 0){
		y = (0xFF & ~packet[2]) + 1;
		printf(" Y=-%d", y);
	}
	else
		printf(" Y=%lu", packet[2]);

	printf("\n");
}

int mouse_disable_stream(){
	unsigned long stat;
	int end = 1;
	//Write command to mouse
	sys_inb(KBC_CMD_REG,&stat);

	while(1){
		if((stat & IBF) == 0){
			sys_outb(KBC_CMD_REG, WRITE_MOUSE); //0xD4
			break;
		}
		tickdelay(micros_to_ticks(DELAY_US));
	}

	//Disable stream mode
	while(1){
		sys_inb(KBC_CMD_REG,&stat);
		if((stat & IBF) == 0){
			sys_outb(IN_BUF, DIS_STREAM_MODE); //0xF5
			break;
		}
		tickdelay(micros_to_ticks(DELAY_US));
	}
	sys_inb(OUT_BUF, &stat);
	return 1;
}


int mouse_set_stream() {

	//retorna 1 em caso de sucesso, 0 em caso de falha
	unsigned long stat;
	//Write command to mouse

	while(1){
		sys_inb(KBC_CMD_REG,&stat);
		if((stat & IBF) == 0){
			sys_outb(KBC_CMD_REG, WRITE_MOUSE); //0xD4
			break;
		}
		tickdelay(micros_to_ticks(DELAY_US));
	}
	tickdelay(micros_to_ticks(DELAY_US));

	//Activate stream mode
	while(1){
		sys_inb(KBC_CMD_REG,&stat);
		if((stat & IBF) == 0){
			sys_outb(IN_BUF, STREAM_MODE); //0xEA
			break;
		}
		tickdelay(micros_to_ticks(DELAY_US));
	}
	//tickdelay(micros_to_ticks(DELAY_US));
	sys_inb(OUT_BUF,&stat);

	//Write command to mouse again
	while(1){
		sys_inb(KBC_CMD_REG,&stat);
		if((stat & IBF) == 0){
			sys_outb(KBC_CMD_REG, WRITE_MOUSE); //0xD4
			break;
		}
		tickdelay(micros_to_ticks(DELAY_US));
	}
	tickdelay(micros_to_ticks(DELAY_US));
	//Activate data mode
	while(1){
		sys_inb(KBC_CMD_REG,&stat);
		if((stat & IBF) == 0){
			sys_outb(IN_BUF, DATA_MODE); //0xF4
			break;
		}
		tickdelay(micros_to_ticks(DELAY_US));
	}
	//tickdelay(micros_to_ticks(DELAY_US));
	while(1){
		sys_inb(OUT_BUF, &stat);
		if((stat & OBF) != 0)
			break;
		else
			tickdelay(micros_to_ticks(DELAY_US));
	}

	//sys_inb(OUT_BUF, &stat);
}


int mouse_handler() {
	//retorna 1 em caso de sucesso, 0 em caso de falha, para ser usado em condições no mouse_int

	unsigned long data;
	sys_inb(OUT_BUF, &data);

	if(counter != 2 && counter != 3){
		packet[0] = 0;
		packet[1] = 0;
		packet[2] = 0;

		packet[0] = data;
		if((packet[0] & BYTE_3) == 0){
			packet[0] = 0;
			counter = 1;
			return 0;
		}
		else{
			counter = 2;
			return 0;
		}
	}
	if(counter == 2){
		packet[1] = data;
		counter = 3;
		return 0;
	}
	if(counter == 3){
		packet[2] = data;
		counter = 1;
		mouse_print();
		return 1;
	}
	return 0;

}


int mouse_int(int packets) {
	int ipc_status, r;
	message msg;
	unsigned long irq_set = BIT(MOUSE_IRQ);//BIT(MOUSE_IRQ);
	int a = mouse_subscribe_int();		//senao volta a por no test_packets
	if (a < 0)
		return 1;
	int count = packets;
	counter = 0;
	while (count != 0) {
		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("Driver_receive failed\n");
			continue;
		}
		if (is_ipc_notify(ipc_status)) {
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE:
				if (msg.NOTIFY_ARG & irq_set) {
					int d = mouse_handler();
					count = count - d;
				}
				break;
			default:
				break;
			}
		}
	}

	if(mouse_disable_stream() != 1)
		return -1;

	//ENABLE STREAM MODE AGAIN
	unsigned long stat;
	int end = 1;
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
	return 0;
}

int test_packet(unsigned short cnt) {
	mouse_set_stream();
	mouse_int(cnt);
	if (mouse_unsubscribe_int() != 1)
		return 1;
	return 0;
}


int check_second(int time){
	if((time/60) == 1){
		return 1;
	}
	return 0;
}
int test_async(unsigned short idle_time) {

	int counter = 0;
	int temp = 0;
	int ipc_status, r;
	message msg;
	int irq_set_m = BIT(m_hook_id);
	int irq_set_timer = ~hook_id_timer;
	if (mouse_subscribe_int() == -1)
		return 1;
	if (timer_subscribe_int() == -1)
		return 1;
	mouse_set_stream();
	while (counter < idle_time) {
		//Get a request message.
		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) { // received notification
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE: // hardware interrupt notification

				if (msg.NOTIFY_ARG & irq_set_m) { // subscribed interrupt
					mouse_handler();
					counter=0;
				}
				if (msg.NOTIFY_ARG & irq_set_timer) {
					temp++;
					if(check_second(temp) == 1)
					{
						temp = 0;
						counter++;
					}
				}
				break;
			default:
				break; //no other notifications expected: do nothing
			}

		}
	}
	printf("Time has run out!\n");
	if(mouse_disable_stream() != 1)
		return -1;

	//ENABLE STREAM MODE AGAIN
	unsigned long stat;
	int end = 1;
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
	return 0;

}

int send_command(unsigned long cmd){
	unsigned long stat;
	//Write command to mouse

	while(1){
		sys_inb(KBC_CMD_REG,&stat);
		if((stat & IBF) == 0){
			sys_outb(KBC_CMD_REG, WRITE_MOUSE); //0xD4
			break;
		}
		tickdelay(micros_to_ticks(DELAY_US));
	}
	tickdelay(micros_to_ticks(DELAY_US));

	while(1){
		sys_inb(KBC_CMD_REG,&stat);
		if((stat & IBF) == 0){
			sys_outb(IN_BUF, cmd);
			break;
		}
		tickdelay(micros_to_ticks(DELAY_US));
	}
	//tickdelay(micros_to_ticks(DELAY_US));
	sys_inb(OUT_BUF,&stat);
	if((stat & ACK) == 0)
		return 0;
	else
		return 1;
}

int test_config_print(){
	if((BIT(0) & packet[0]) == 1)
		printf("Left button pressed\n");
	else
		printf("Left button isn't pressed\n");

	if((BIT(2) & packet[0]) == 1)
		printf("Right button pressed\n");
	else
		printf("Right button isn't pressed\n");

	if((BIT(1) & packet[0]) == 1)
		printf("Middle button pressed.\n");
	else
		printf("Middle button isn't pressed\n");

	if((BIT(4) & packet[0]) == 1)
		printf("Scaling is 2:1\n");
	else
		printf("Scaling is 1:1\n");

	if((BIT(5) & packet[0]) == 1)
		printf("Data reporting enabled\n");
	else
		printf("Data reporting disabled\n");

	if((BIT(6) & packet[0]) == 1)
		printf("Remote mode\n");
	else
		printf("Stream mode\n");

	if((packet[1] & 0x01) == 1)
		printf("Resolution 1\n");
	else if ((packet[1] & 0x02) == 1)
		printf("Resolution 2\n");
	else if ((packet[1] & 0x03) == 1)
		printf("Resolution 3\n");

	printf("Sample Rate:%u", packet[2]);
	return 0;
}

int test_config_handler(){

	unsigned long data;
	sys_inb(OUT_BUF, &data);
	if(counter != 2 && counter != 3){
		packet[0] = 0;
		packet[1] = 0;
		packet[2] = 0;

		packet[0] = data;
		counter = 2;
		return 0;
	}

	if(counter == 2){
		packet[1] = data;
		counter = 3;
		return 0;
	}
	if(counter == 3){
		packet[2] = data;
		counter = 1;
		test_config_print();
		return 1;
	}
	return 0;

}

int test_config_int(){
	int ipc_status, r;
	message msg;
	unsigned long irq_set = BIT(MOUSE_IRQ);//BIT(MOUSE_IRQ);
	//mouse_set_stream();
	int count = 0;
	counter = 0;
	while (count == 0) {
		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("Driver_receive failed\n");
			continue;
		}
		if (is_ipc_notify(ipc_status)) {
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE:
				if (msg.NOTIFY_ARG & irq_set) {
					int d = test_config_handler();
					count = count - d;
				}
				break;
			default:
				break;
			}
		}
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
	return 0;
}


int test_config(void ){
	int a = mouse_subscribe_int();
	if (a != 12)
		return 1;
	mouse_disable_stream();

	unsigned long stat;
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
			sys_outb(IN_BUF, MOUSE_CONFIG); //0xE9
			break;
		}
		tickdelay(micros_to_ticks(DELAY_US));
	}
	sys_inb(OUT_BUF, &stat);
	test_config_int();
	return 0;
}



typedef enum {
	INIT, DRAW, COMP
} state_t;

typedef enum {
	RDOWN, RUP, MOVE
} ev_type_t;

typedef struct {
	ev_type_t type;
} event_t;
static state_t st; // initial state; keep state

int check_line(event_t *evt, short length) {
	int ipc_status, r;
	message msg;
	int end = 0;
	unsigned long irq_set = BIT(MOUSE_IRQ);//BIT(MOUSE_IRQ);
	//printf(" inside check_line ");
	//while(end == 0){
	switch (st) {
	case INIT:
		/*if (evt->type == RDOWN){ //se se pressionar o botao direito do rato, fica RDOWN e salta para DRAW
			printf("  inside INIT  ");
			st = DRAW;
			evt->type = MOVE;
			return 2;
		}
		break;*/
	case DRAW:
		if (evt->type == MOVE) { //se se mexer o rato com o botao direito pressionado fica em MOVE

			//printf("   Inside check_line MOVE \n");
			long dx2;
			long dy2;
			if((XSIGN & packet[0]) != 0)
				dx2 = (0xFF - ~packet[1]) + 1; //packet[1] é o delta x
			else
				dx2 = packet[1];
			if((YSIGN & packet[0]) != 0)
				dy2 = (0xFF - ~packet[2]) + 1; //packet[2] é o delta y
			else
				dy2 = packet[2];

			if((dx2 < 0 && dy2 > 0) || (dx2 > 0 && dy2 < 0)){
				dx = 0;
				dy = 0;
				return -1;
			}



			dx += dx2;
			dy += dy2;
			if ((dy * dx) > 0) { //delta x * delta y > 0 significa que o declive é positivo
				//printf("\n im here \n");
				//double a = sqrt(dx1 * dx1 + dy1 * dy1);
				//printf("%d", a);
				//printf("\n");
				if ((sqrt(dx * dx + dy * dy)) < abs(length)){ //se o comprimento do desenho for menor que length está bom, senão está mau
					//double a = sqrt(dx1 * dx1 + dy1 * dy1);
					//printf("%d", a);
					printf("Length achieved\n");
					return 0;
				}
				else
					return 1;
			} else{
				//printf("  dy1 * dx1 <0  ");
				return -1;
			}
		} else if (evt->type == RUP) //se se largar o botão direito volta a INIT
			st = INIT;

		break;
	default:
		break;
	}
	//}
}


int mouse_handler_gesture() {
	//retorna 1 em caso de sucesso, 0 em caso de falha, para ser usado em condições no mouse_int

	unsigned long data;
	sys_inb(OUT_BUF, &data);

	if(counter != 2 && counter != 3){
		packet[0] = 0;
		packet[1] = 0;
		packet[2] = 0;

		packet[0] = data;
		if((packet[0] & BYTE_3) == 0){
			packet[0] = 0;
			counter = 1;
			return 0;
		}
		else{
			counter = 2;
			return 0;
		}
	}
	if(counter == 2){
		packet[1] = data;
		counter = 3;
		return 0;
	}
	if(counter == 3){
		packet[2] = data;
		counter = 1;
		mouse_print();
		return 1;
	}
	return 0;

}

event_t event;

int test_gesture_rdown(length){
	int ipc_status, r;
	message msg;
	unsigned long irq_set = BIT(MOUSE_IRQ);//BIT(MOUSE_IRQ);
	int count = 40;
	while (count != 0){
		if ( driver_receive(ANY, &msg, &ipc_status) != 0 ) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) {
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE:
				if (msg.NOTIFY_ARG & irq_set) {
					int d = mouse_handler_gesture();
					if(d == 1){
						if((RB & packet[0]) == 0){
							event.type = RUP;
							count = 0;
							break;
						}
						st = DRAW;
						event.type = MOVE;
						int a = check_line(&event, length);
						if(a == 0){
							count = 0;
							return 0;

						}
					}
				}
			}
		}
	}
}

int test_gesture_int(short length){
	int ipc_status, r;
	message msg;
	unsigned long irq_set = BIT(MOUSE_IRQ);//BIT(MOUSE_IRQ);
	int count = 40;
	counter = 1;
	while (count != 0){
		if ( driver_receive(ANY, &msg, &ipc_status) != 0 ) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) {
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE:
				if (msg.NOTIFY_ARG & irq_set) {
					int d = mouse_handler_gesture();
					if(d == 1){
						if((RB & packet[0]) != 0){
							event.type = RDOWN;
							if(test_gesture_rdown(length) == 0){
								return 0;
							}
						}
					}
					count -= d;
				}
			}
		}
	}
}


int test_gesture(short length) {
	if (mouse_subscribe_int() != 12)
		return 1;

	mouse_set_stream();

	test_gesture_int(length);


	if(mouse_disable_stream() != 1)
		return -1;

	//ENABLE STREAM MODE AGAIN
	unsigned long stat;
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
	return 0;
	//////////////////////////////////////
}

