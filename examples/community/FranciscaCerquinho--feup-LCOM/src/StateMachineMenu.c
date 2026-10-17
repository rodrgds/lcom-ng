#include "StateMachineMenu.h"
#include "Bitmap.h"


StateMenu *newSateMenu()
{
	StateMenu *new;
	new=(StateMenu *)malloc(sizeof(StateMenu));


	new->menu=loadBitmap("/home/lcom/lcom1617-t1g04/proj/images/menu.bmp");

	new->Play->image=loadBitmap("/home/lcom/lcom1617-t1g04/proj/images/startButton.bmp");
	new->ClickedStart->image=loadBitmap("/home/lcom/lcom1617-t1g04/proj/images/ClickedStart.bmp");


	new->Exit->image=loadBitmap("/home/lcom/lcom1617-t1g04/proj/images/ExitButton.bmp");
	new->ClickedExit->image=loadBitmap("/home/lcom/lcom1617-t1g04/proj/images/ClickedExit.bmp");

	new->Start=0;
	new->ExitGame=0;

	return new;
}

int Menu(char *map, IRQ *irqs)
{
	Bitmap *menu, *buttonStart, *buttonExit, *clickedStart, *clickedExit, *cursor;
	int ipc_status;
	int r;
	unsigned long st;
	int counter=0;
	message msg;
	int mode=0, mode2=0;
	unsigned char packet[3];

	Mouse *auxMouse =newMouse();

	int speed1 = 0, speed2=0, gravity=0;
	int y=2, x=0, x2=0;
	int aux=1;

	menu=loadBitmap("home/lcom/lcom1617-t1g04/proj/images/menu.bmp");
	buttonStart= loadBitmap("/home/lcom/lcom1617-t1g04/proj/images/startButton.bmp");
	buttonExit= loadBitmap("/home/lcom/lcom1617-t1g04/proj/images/exitButton.bmp");
	clickedStart=loadBitmap("/home/lcom/lcom1617-t1g04/proj/images/ClickedStart.bmp");
	clickedExit=loadBitmap("/home/lcom/lcom1617-t1g04/proj/images/ClickedExit.bmp");


	mousePermissions();

	drawBitmap(menu,512,0,ALIGN_CENTER,map);

	drawBitmap(buttonStart, 512,384,ALIGN_CENTER,map);
	drawBitmap(buttonExit, 512, 384+76+2,ALIGN_CENTER,map);

	while(1){ /* You may want to use a different condition */
		/* Get a request message. */
		if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) { /* received notification */
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE: /* hardware interrupt notification */
				if (msg.NOTIFY_ARG & irqs->irq_set_mouse) { /* subscribed interrupt */
					if(mouse_int_handler(auxMouse->packet) == 1){
						LeftButton(auxMouse);

						if(auxMouse->LeftPressed)
						{

							drawBitmap(clickedStart, 512, 384,ALIGN_CENTER ,map);
							sleep(1);
							free(clickedStart);
							free(clickedExit);
							free(buttonExit);
							free(buttonStart);
							free(menu);
							free(auxMouse);
							return 2;
						}
					}
				}
				if (msg.NOTIFY_ARG & irqs->irq_set_kbd) { /* subscribed interrupt */

					mode=keyboard_int_handler_c();
					if(mode == 0x81){
						drawBitmap(clickedExit, 512, 384+76+2,ALIGN_CENTER,map);
						sleep(1);
						free(clickedStart);
						free(clickedExit);
						free(buttonExit);
						free(buttonStart);
						free(menu);
						free(auxMouse);
						keyboard_unsubscribe_int();
						mouse_unsubscribe_int();
						return 1;
					}

					if(mode == 0x9f)
					{
						drawBitmap(clickedStart, 512, 384,ALIGN_CENTER ,map);
						sleep(1);
						free(clickedStart);
						free(clickedExit);
						free(buttonExit);
						free(buttonStart);
						free(menu);
						free(auxMouse);
						return 2;
					}
				}
				break;
			default:
				break; /* no other notifications expected: do nothing */
			}
		} else { /* received a standard message, not a notification */
			/* no standard messages expected: do nothing */
		}
	}
}


