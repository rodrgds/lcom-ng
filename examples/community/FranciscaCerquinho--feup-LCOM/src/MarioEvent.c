#include "MarioEvent.h"


MarioGame *StartMarioGame(){

	MarioGame *newMarioG=(MarioGame *)malloc(sizeof(MarioGame));

	newMarioG->irq_set_timer=timer_subscribe_int();
	newMarioG->irq_set_mouse=mouse_subscribe_int();
	newMarioG->irq_set_kbd=keyboard_subscribe_int();

	newMarioG->BreakCode=0;


	return newMarioG;
}

int UpdateMarioGame(char *map, IRQ *irqs)
{
	Bitmap* mario1, *newBackground1, *mario2, *pipe, *GameOver;

	int ipc_status;
	int r;
	unsigned long st;
	int frames=0;
	message msg;
	int mode=0;
	unsigned char packet[3];

	mario *new, *new2;
	Background *newb;
	Pipe *newPip;

	Mouse* auxMouse =newMouse();


	int speed1 = 0, speed2=0, gravity=0;
	int speed_background=2, changeMario1=0, changeMario2=0;
	int aux=1;
	map = vg_init(0x117);

	GameOver=loadBitmap("/home/lcom/lcom1617-t1g04/proj/images/GameOver.bmp");
	mario1= loadMario("/home/lcom/lcom1617-t1g04/proj/images/mario.bmp");
	mario2= loadMario("/home/lcom/lcom1617-t1g04/proj/images/mario2.bmp");
	pipe = loadPipe();


	newPip = newPipe(100, 601);
	newBackground1 = loadBackground();
	new = newMario(100, 500);
	new2 = newMario(100, 500);


	while(1){
		/* Get a request message. */
		if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) { /* received notification */
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE: /* hardware interrupt notification */
				if (msg.NOTIFY_ARG & irqs->irq_set_timer) { /* subscribed interrupt */

					if(ColisionPipe(new,newPip)==1)
					{
						if(keyboard_unsubscribe_int()==1)
							return 1;
						if(timer_unsubscribe_int()==1)
							return 1;
						drawBitmap(GameOver,0,0,ALIGN_LEFT,map);
						sleep(1);
						vg_exit();
						return frames/60/9;
					}
					frames++;
					changeMario1=changeMario1+1;
					changeMario2=changeMario2+1;
					newb = newBackground(0-speed2, 0);
					drawBackground(newb,map,newBackground1);
					newPip= newPipe(1024-speed2,599);
					drawPipe(newPip, map, pipe);

					if(speed2==1024)
					{
						speed2=0;
					}

					speed2+=speed_background;

					if(mode == 0xb9){
						speed1+=speed_background+2;
						if(speed1==200)
							mode=0;
					}

					if(speed1 < 200){
						if(changeMario1<15){
							new = newMario(100, 601 - speed1 );
							drawMario(new,map,mario1);
						}
						else if(changeMario1>=15){
							new2 = newMario(100, 601 - speed1 );
							drawMario(new2,map,mario2);
						}
						if(changeMario1>30){
							changeMario1 = 0;
						}
					}
					else
					{

						gravity+=speed_background+2;
						if(changeMario2<15)
						{
							new = newMario(100, 401 + gravity);
							drawMario(new,map,mario1);
						}
						else if(changeMario2>=15){
							new2 = newMario(100, 401 + gravity);
							drawMario(new2,map,mario2);
						}
						if(changeMario2>30)
						{
							changeMario2=0;
						}
						if(gravity==200)
						{
							speed1=0;
							gravity=0;

						}

					}
				}
				if (msg.NOTIFY_ARG & irqs->irq_set_kbd) { /* subscribed interrupt */

					mode=keyboard_int_handler_c();

				}
				break;
			default:
				break; /* no other notifications expected: do nothing */
			}
		} else { /* received a standard message, not a notification */
			/* no standard messages expected: do nothing */
		}
	}


	return frames/60/9;
}






void DestroyGame(MarioGame *newMarioG)
{
	free(newMarioG);
}


