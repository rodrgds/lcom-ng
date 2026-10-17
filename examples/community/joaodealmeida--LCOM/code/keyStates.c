#include "keyStates.h"
#include "Utilities.h"

//current and previous state of the keys of interest
//0 should indicate that is not being pressed and 1 that is being pressed
//char previous_ESCAPE = 0;
//char current_ESCAPE = 0;
struct keyStates zero_ks = {0,0,0,0,0};
struct keyStates previous_ks = {0,0,0,0,0};
struct keyStates current_ks = {0,0,0,0,0};

//funcs implementation below
/*to implemtent*/ //char ks_keyPressed(unsigned long key)

char ks_keyReleased(unsigned long key)
{
	switch(key)
	{
	case KEY_ESCAPE: return (current_ks._escape==1&&previous_ks._escape==0);
	case KEY_W: return (current_ks._key_W==1&&previous_ks._key_W==0);
	case KEY_S: return (current_ks._key_S==1&&previous_ks._key_S==0);
	case KEY_A: return (current_ks._key_A==1&&previous_ks._key_A==0);
	case KEY_D: return (current_ks._key_D==1&&previous_ks._key_D==0);
	default:
		return -1;
	}
}

char ks_keyPressed(unsigned long key)
{
	switch(key)
	{
	case KEY_ESCAPE: return (current_ks._escape==0&&previous_ks._escape==1);
	case KEY_W: return (current_ks._key_W==0&&previous_ks._key_W==1);
	case KEY_S: return (current_ks._key_S==0&&previous_ks._key_S==1);
	case KEY_A: return (current_ks._key_A==0&&previous_ks._key_A==1);
	case KEY_D: return (current_ks._key_D==0&&previous_ks._key_D==1);
	default:
		return -1;
	}
}

void ks_updateKeyStates(unsigned long code)
{
	//copy current state to previous
	//in this lab5 we only use one key so it is simpler
	//previous_ESCAPE=current_ESCAPE;
	previous_ks=current_ks;

	//update key accordingly to received code
	//note:careful! not considering larger codes!
	//assuming 1 byte code
	char beingPressed=((code&0x80)==0);//if it is a makecode assign value 1

	code&=0x7f;//take out breakbit bit
	switch(code)//find key to update
	{
	case KEY_ESCAPE:current_ks._escape=beingPressed;	break;
	case KEY_W:current_ks._key_W=beingPressed;	break;
	case KEY_S:current_ks._key_S=beingPressed;	break;
	case KEY_A:current_ks._key_A=beingPressed;	break;
	case KEY_D:current_ks._key_D=beingPressed;	break;
	default:
		break;
	}

}

void ks_setKeyStates2Zero()
{
	previous_ks=zero_ks;
	current_ks=zero_ks;
}
