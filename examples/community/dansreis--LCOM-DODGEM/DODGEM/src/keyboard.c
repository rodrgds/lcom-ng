#include"keyboard.h"
#include "utilities.h"

int hook_id2 = 2;
unsigned long keyboard;

int KBD_subscribe_int(void )
{

	int hook;
	hook = hook_id2;
	if (sys_irqsetpolicy(KBD_IRQ,IRQ_REENABLE | IRQ_EXCLUSIVE,&hook_id2) == OK)
		if (sys_irqenable(&hook_id2) == OK)
			return BIT(hook);

	return -1;
}

int KBD_unsubscribe_int()
{
	if(sys_irqrmpolicy(&hook_id2) == OK)
		if (sys_irqdisable(&hook_id2) == OK)
			return 0;

	return 1;
}

int KBD_handler_C()
{
	sys_inb(OUT_BUF, &keyboard);// vai à porta buscar e coloca-o em &keyboard

	keyboard = keyboard << 24;
	keyboard = keyboard >> 24;

	return keyboard;
}


char getLetra(unsigned long keyboard) // de acordo com os keyboard input retorna a letra char correspondente
{
	switch(keyboard)
	{
	case KEY_A:
		return 'a';
		break;
	case KEY_B:
		return 'b';
		break;
	case KEY_C:
		return 'c';
		break;
	case KEY_D:
		return 'd';
		break;
	case KEY_E:
		return 'e';
		break;
	case KEY_F:
		return 'f';
		break;
	case KEY_G:
		return 'g';
		break;
	case KEY_H:
		return 'h';
		break;
	case KEY_I:
		return 'i';
		break;
	case KEY_J:
		return 'j';
		break;
	case KEY_K:
		return 'k';
		break;
	case KEY_L:
		return 'l';
		break;
	case KEY_M:
		return 'm';
		break;
	case KEY_N:
		return 'n';
		break;
	case KEY_O:
		return 'o';
		break;
	case KEY_P:
		return 'p';
		break;
	case KEY_Q:
		return 'q';
		break;
	case KEY_R:
		return 'r';
		break;
	case KEY_S:
		return 's';
		break;
	case KEY_T:
		return 't';
		break;
	case KEY_U:
		return 'u';
		break;
	case KEY_V:
		return 'v';
		break;
	case KEY_W:
		return 'w';
		break;
	case KEY_X:
		return 'x';
		break;
	case KEY_Y:
		return 'y';
		break;
	case KEY_Z:
		return 'z';
		break;
	case KEY_0:
		return '0';
		break;
	case KEY_1:
		return '1';
		break;
	case KEY_2:
		return '2';
		break;
	case KEY_3:
		return '3';
		break;
	case KEY_4:
		return '4';
		break;
	case KEY_5:
		return '5';
		break;
	case KEY_6:
		return '6';
		break;
	case KEY_7:
		return '7';
		break;
	case KEY_8:
		return '8';
		break;
	case KEY_9:
		return '9';
		break;
//	case RETURN_KEY:
//		return '*';
//		break;
	case BACKSPACE:
		return '*';
		break;
	case KEY_SPACE:
		return ' ';
		break;
	default:
		return '.';
	}
}

int getLetterPos(char letter) // retorna a posicao da letra no bitmap
{
	switch(letter)
		{
		case 'a':
			return 0;
			break;
		case 'b':
			return 40;
			break;
		case 'c':
			return 80;
			break;
		case 'd':
			return 120;
			break;
		case 'e':
			return 160;
			break;
		case 'f':
			return 200;
			break;
		case 'g':
			return 240;
			break;
		case 'h':
			return 280;
			break;
		case 'i':
			return 320;
			break;
		case 'j':
			return 340;
			break;
		case 'k':
			return 370;
			break;
		case 'l':
			return 410;
			break;
		case 'm':
			return 450;
			break;
		case 'n':
			return 490;
			break;
		case 'o':
			return 530;
			break;
		case 'p':
			return 570;
			break;
		case 'q':
			return 610;
			break;
		case 'r':
			return 650;
			break;
		case 's':
			return 690;
			break;
		case 't':
			return 730;
			break;
		case 'u':
			return 770;
			break;
		case 'v':
			return 810;
			break;
		case 'w':
			return 850;
			break;
		case 'x':
			return 900;
			break;
		case 'y':
			return 940;
			break;
		case 'z':
			return 980;
			break;
		case '0':
			return 0;
			break;
		case '1':
			return 40;
			break;
		case '2':
			return 80;
			break;
		case '3':
			return 120;
			break;
		case '4':
			return 160;
			break;
		case '5':
			return 200;
			break;
		case '6':
			return 240;
			break;
		case '7':
			return 280;
			break;
		case '8':
			return 320;
			break;
		case '9':
			return 360;
			break;
		default:
			return -1;
		}

}






