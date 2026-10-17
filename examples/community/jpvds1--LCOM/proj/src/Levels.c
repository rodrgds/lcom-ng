#include "Levels.h"

int shootfrequency;
int movementmode;
int gamemode;

int call_3()
{
    gamemode = 0;
    shootfrequency = 3;
    movementmode = 2;

    ll_CleanUp(LaserFirst);
    LaserFirst = NULL;
    LaserAdded = NULL;  

    el_addNode(el_getNewNode(240, 300, 240, 440, 300, 400));
    el_addNode(el_getNewNode(440, 300, 440, 640, 300, 400));
    el_addNode(el_getNewNode(640, 300, 640, 840, 300, 400));
    el_addNode(el_getNewNode(840, 300, 840, 1040, 300, 400));

    el_addNode(el_getNewNode(240, 400, 240, 440, 400, 500));
    el_addNode(el_getNewNode(440, 400, 440, 640, 400, 500));
    el_addNode(el_getNewNode(640, 400, 640, 840, 400, 500));
    el_addNode(el_getNewNode(840, 400, 840, 1040, 400, 500));

    el_addNode(el_getNewNode(240, 500, 240, 440, 500, 600));
    el_addNode(el_getNewNode(440, 500, 440, 640, 500, 600));
    el_addNode(el_getNewNode(640, 500, 640, 840, 500, 600));
    el_addNode(el_getNewNode(840, 500, 840, 1040, 500, 600));

    if(interrupt_loop()) return 1;
    return 0;
}

int call_2()
{
    gamemode = 0;
    shootfrequency = 3;
    movementmode = 1;

    ll_CleanUp(LaserFirst);
    LaserFirst = NULL;
    LaserAdded = NULL;  

    el_addNode(el_getNewNode(100, 200, 100, 200, 200, 200));
    el_addNode(el_getNewNode(200, 200, 200, 300, 200, 200));
    el_addNode(el_getNewNode(300, 200, 300, 400, 200, 200));
    el_addNode(el_getNewNode(400, 200, 400, 500, 200, 200));
    el_addNode(el_getNewNode(500, 200, 500, 600, 200, 200));
    el_addNode(el_getNewNode(600, 200, 600, 700, 200, 200));
    el_addNode(el_getNewNode(700, 200, 700, 800, 200, 200));
    el_addNode(el_getNewNode(800, 200, 800, 900, 200, 200));
    el_addNode(el_getNewNode(900, 200, 900, 1000, 200, 200));
    el_addNode(el_getNewNode(1000, 200, 1000, 1100, 200, 200));

    el_addNode(el_getNewNode(150, 300, 150, 250, 300, 300));
    el_addNode(el_getNewNode(250, 300, 250, 350, 300, 300));
    el_addNode(el_getNewNode(350, 300, 350, 450, 300, 300));
    el_addNode(el_getNewNode(450, 300, 450, 550, 300, 300));
    el_addNode(el_getNewNode(550, 300, 550, 650, 300, 300));
    el_addNode(el_getNewNode(650, 300, 650, 750, 300, 300));
    el_addNode(el_getNewNode(750, 300, 750, 850, 300, 300));
    el_addNode(el_getNewNode(850, 300, 850, 950, 300, 300));
    el_addNode(el_getNewNode(950, 300, 950, 1050, 300, 300));

    el_addNode(el_getNewNode(100, 400, 100, 200, 400, 400));
    el_addNode(el_getNewNode(200, 400, 200, 300, 400, 400));
    el_addNode(el_getNewNode(300, 400, 300, 400, 400, 400));
    el_addNode(el_getNewNode(400, 400, 400, 500, 400, 400));
    el_addNode(el_getNewNode(500, 400, 500, 600, 400, 400));
    el_addNode(el_getNewNode(600, 400, 600, 700, 400, 400));
    el_addNode(el_getNewNode(700, 400, 700, 800, 400, 400));
    el_addNode(el_getNewNode(800, 400, 800, 900, 400, 400));
    el_addNode(el_getNewNode(900, 400, 900, 1000, 400, 400));
    el_addNode(el_getNewNode(1000, 400, 1000, 1100, 400, 400));

    if(interrupt_loop()) return 1;
    call_3();
    return 0;
}

int call_1()
{
    gamemode = 0;
    shootfrequency = 2;
    movementmode = 1;
    player_defaults();

    el_addNode(el_getNewNode(200, 200, 200, 600, 200, 200));
    el_addNode(el_getNewNode(300, 200, 300, 700, 200, 200));
    el_addNode(el_getNewNode(400, 200, 400, 800, 200, 200));
    el_addNode(el_getNewNode(500, 200, 500, 900, 200, 200));
    el_addNode(el_getNewNode(600, 200, 600, 1000, 200, 200));
    el_addNode(el_getNewNode(200, 400, 200, 600, 400, 400));
    el_addNode(el_getNewNode(300, 400, 300, 700, 400, 400));
    el_addNode(el_getNewNode(400, 400, 400, 800, 400, 400));
    el_addNode(el_getNewNode(500, 400, 500, 900, 400, 400));
    el_addNode(el_getNewNode(600, 400, 600, 1000, 400, 400));

    //1280
    if(interrupt_loop()) return 1;
    call_2();
    return 0;
}

void spawn_endless()
{
    el_addNode(el_getNewNode(100, 200, 100, 250, 200, 300));
    el_addNode(el_getNewNode(250, 200, 250, 400, 200, 300));
    el_addNode(el_getNewNode(400, 200, 400, 550, 200, 300));
    el_addNode(el_getNewNode(550, 200, 550, 700, 200, 300));
    el_addNode(el_getNewNode(700, 200, 700, 850, 200, 300));
    el_addNode(el_getNewNode(850, 200, 850, 1000, 200, 300));
    el_addNode(el_getNewNode(1000, 200, 1000, 1150, 200, 300));

    el_addNode(el_getNewNode(100, 500, 100, 250, 500, 600));
    el_addNode(el_getNewNode(250, 500, 250, 400, 500, 600));
    el_addNode(el_getNewNode(400, 500, 400, 550, 500, 600));
    el_addNode(el_getNewNode(550, 500, 550, 700, 500, 600));
    el_addNode(el_getNewNode(700, 500, 700, 850, 500, 600));
    el_addNode(el_getNewNode(850, 500, 850, 1000, 500, 600));
    el_addNode(el_getNewNode(1000, 500, 1000, 1150, 500, 600));
}

int call_endless()
{
    gamemode = 1;
    shootfrequency = 4;
    movementmode = 2;
    player_defaults();

    ll_CleanUp(LaserFirst);
    LaserFirst = NULL;
    LaserAdded = NULL;  

    spawn_endless();

    if(interrupt_loop()) return 1;
    return 0;
}

int call_level(int lvl)
{
    switch (lvl)
    {
    case 0:
        call_1();
        return dead_menu_interrupt_loop();
        break;
    case 1:
        call_endless(); 
        return dead_menu_interrupt_loop();
        break;
    case 2:
        instructions_loop();
        return menu_interrupt_loop();
        break;
    case 3:
        return 1;
    case 4:
        if(menu_interrupt_loop()) return 1;
        return dead_menu_interrupt_loop();
    default:
        printf("not a level");
        return 1;
        break;
    }
}

/*
endless
lvl 1
lbl 2
lvl 3
quit
*/
