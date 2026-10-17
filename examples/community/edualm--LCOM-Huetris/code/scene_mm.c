#include "board.h"
#include "scene.h"

void sceneMainMenu_keyboardHandler() {
    unsigned long kb_read = keyboard_read();
    
    switch (kb_read) {
        case kKeyboardKey1:
            scene_initGame(1);
            
            break;
            
        case kKeyboardKey2:
            scene_initGame(2);
            
            break;
            
        case kKeyboardKeyEsc:
            scene_terminate();
            
            break;
	
	default:
	    
	    break;
    }
}
