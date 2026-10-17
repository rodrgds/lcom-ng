#ifndef __KEYSTATES_H
#define __KEYSTATES_H

struct keyStates
{
	char _escape;
	char _key_W;
	char _key_S;
	char _key_A;
	char _key_D;
};
//==============================

//define used keys identifier
//(should be equal to make code)
#define KEY_ESCAPE 0x001 // 0x1 make | brake 0x81
#define KEY_W 0x0011
#define KEY_A 0x001E
#define KEY_S 0x001F
#define KEY_D 0x0020

//funcs below
/**
 * @brief check if key has just been pressed
 * @param key to check
 * @return 0 if not pressed, 1 if pressed, negative value if the func does not check the given key
 */
char ks_keyPressed(unsigned long key);

/**
 * @brief check if key has just been released
 * @param key to check
 * @return 0 if not released, 1 if released, negative value if the func does not check the given key
 */
char ks_keyReleased(unsigned long key);

/**
 * @brief update key states and previous key states
 * @param last read code by keyboard
 */
void ks_updateKeyStates(unsigned long code);

void ks_setKeyStates2Zero();

#endif //__KEYSTATES_H
