#include <lcom/lcf.h>

#include "main_menu.h"
#include "single_player.h"
#include "score.h"

extern bool insync;
extern int byteCounter;
extern unsigned int counter;
extern uint8_t scancodes[2];

extern Score score;

MainMenuStM stm = {
    .state = MENU, 
    .event = NOTH, 
    .actions = {stMENU,stSINGLEP,stMULTIP,stSCORES,stEXIT}, 
    .mouse = NULL,
    .xi = 0, 
    .yi = 0
};

SplashScreen spl;
Interface background;
Trex trex;

int mainLoop(){

    LoadBitmaps();

    int ipc_status;
    message msg;
    int r;
    bool detectedCollision = false;

    struct packet p;    
    p.bytes[0] = 0x0;
    p.bytes[1] = 0x0;
    p.bytes[2] = 0x0;
    
    scancodes[0] = 0x00;
    scancodes[1] = 0x00;

    stm.state = SPLASHSCREEN;
        
    while(stm.state != EXIT) {
        /* Get a request message. */
        if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
            printf("driver_receive failed with: %d", r);
            continue;
        }
        if (is_ipc_notify(ipc_status)) { /* received notification */
            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE: /* hardware interrupt notification */         
                    if (msg.m_notify.interrupts & background.irq_mouse) { /* subscribed interrupt */
                        mouse_ih();

                        if (byteCounter % 3 == 0 && insync) {
                            packet_assemble(&p);
                            if(stm.state == MENU)
                                MenuMouseEventHandler(p);
                            else if(stm.state == SCORES)
                                ScoreMouseEventHandler(p);
                            else if(stm.state == MULTIP)
                                MenuMouseEventHandler(p);
                        }      

                    }
                    if(msg.m_notify.interrupts & background.irq_kbd){
                        kbc_ih();
                        if(stm.state == MENU){
                            MenuKbdEventHandler();
                        }
                        else if(stm.state == SINGLEP){
                            PlayKbdEventHandler();
                        }
                        else if(stm.state == SCORES){
                            ScoreKbdEventHandler();
                        }
                        else if(stm.state == MULTIP){
                            MenuKbdEventHandler();
                        }

                    }
                    if (msg.m_notify.interrupts & background.irq_timer) {
                        timer_int_handler();

                        if(stm.state == SPLASHSCREEN){
                            if(stMENU() == OK) stm.state = MENU;
                        }
                        else if(stm.state == MENU){
                            MenuDrawInterface();
                        }
                        else if(stm.state == SINGLEP){
                            if(scancodes[0] != ESC_BREAK_CODE && !detectedCollision){
                                detectedCollision = PlayDrawInterface();
                            }
                            else{
                                detectedCollision = false;
                                stm.state = SPLASHSCREEN;
                                stm.event = NOTH;
                            }
                        }
                        else if(stm.state == SCORES){
                            ScoresDrawInterface();
                        }
                        else if(stm.state == MULTIP){
                            stm.state = MENU;
                            stm.event = NOTH;
                        }
                    }
                    break;
                default:
                    break; /* no other notifications expected: do nothing */ 
            }
        }
        else {
            /* received a standard message, not a notification */
            /* no standard messages expected: do nothing */
        }
    }

    return stm.actions[stm.state]();
}

void LoadBitmaps(){
    
    trex.under = false;
    trex.jump = false;
    trex.corrector = 8;
    trex.hy = V(29,60);

    srand(time(0));

    stm.xi = rand() % (H(1,1) - 50) + 10;
    stm.yi = rand() % (V(1,1) - 50) + 10;

    background.trackvelocity = 5;
    background.btrack = 0;
    
    trex.trex1 = loadBitmap("/home/lcom/labs/proj/res/trex1.bmp");
    trex.trex2 = loadBitmap("/home/lcom/labs/proj/res/trex2.bmp");
    trex.trex3 = loadBitmap("/home/lcom/labs/proj/res/trex3.bmp");
    trex.trex4 = loadBitmap("/home/lcom/labs/proj/res/trex4.bmp");
    trex.trex5 = loadBitmap("/home/lcom/labs/proj/res/trex5.bmp");

    background.title = loadBitmap("/home/lcom/labs/proj/res/trex_run_title.bmp");
    background.track1 = loadBitmap("/home/lcom/labs/proj/res/track1.bmp");
    background.track2 = loadBitmap("/home/lcom/labs/proj/res/track2.bmp");
    background.playBtn = loadBitmap("/home/lcom/labs/proj/res/playbtn.bmp");
    background.playBtn_Hoover = loadBitmap("/home/lcom/labs/proj/res/playbtn_hoover.bmp");
    background.multiBtn = loadBitmap("/home/lcom/labs/proj/res/multibtn.bmp");
    background.multiBtn_Hoover = loadBitmap("/home/lcom/labs/proj/res/multibtn_hoover.bmp");    
    background.scoresBtn = loadBitmap("/home/lcom/labs/proj/res/scorebtn.bmp");
    background.scoresBtn_Hoover = loadBitmap("/home/lcom/labs/proj/res/scorebtn_hoover.bmp");
    background.exitBtn = loadBitmap("/home/lcom/labs/proj/res/exitbtn.bmp");
    background.exitBtn_Hoover = loadBitmap("/home/lcom/labs/proj/res/exitbtn_hoover.bmp");
    background.cloud1 = loadBitmap("/home/lcom/labs/proj/res/cloud1.bmp");
    background.cloud2 = loadBitmap("/home/lcom/labs/proj/res/cloud2.bmp");
    background.bigtree1 = loadBitmap("/home/lcom/labs/proj/res/bigtree1.bmp");
    background.bigtree2 = loadBitmap("/home/lcom/labs/proj/res/bigtree2.bmp");
    background.bigtree3 = loadBitmap("/home/lcom/labs/proj/res/bigtree3.bmp");
    background.bigtree4 = loadBitmap("/home/lcom/labs/proj/res/bigtree4.bmp");
    background.bigtree5 = loadBitmap("/home/lcom/labs/proj/res/bigtree5.bmp");
    background.bigtree6 = loadBitmap("/home/lcom/labs/proj/res/bigtree6.bmp");
    background.smalltree1 = loadBitmap("/home/lcom/labs/proj/res/smalltree1.bmp");
    background.smalltree2 = loadBitmap("/home/lcom/labs/proj/res/smalltree2.bmp");
    background.smalltree3 = loadBitmap("/home/lcom/labs/proj/res/smalltree3.bmp");
    background.smalltree4 = loadBitmap("/home/lcom/labs/proj/res/smalltree4.bmp");
    background.smalltree5 = loadBitmap("/home/lcom/labs/proj/res/smalltree5.bmp");
    background.multitree1 = loadBitmap("/home/lcom/labs/proj/res/multiple_trees1.bmp");
    background.multitree2 = loadBitmap("/home/lcom/labs/proj/res/multiple_trees2.bmp");
    background.multitree3 = loadBitmap("/home/lcom/labs/proj/res/multiple_trees3.bmp");
    background.multitree4 = loadBitmap("/home/lcom/labs/proj/res/multiple_trees4.bmp");
    background.bird1 = loadBitmap("/home/lcom/labs/proj/res/bird1.bmp");
    background.bird2 = loadBitmap("/home/lcom/labs/proj/res/bird2.bmp");
    background.gameover = loadBitmap("/home/lcom/labs/proj/res/gameover.bmp");
    background.n0 = loadBitmap("/home/lcom/labs/proj/res/0.bmp");
    background.n1 = loadBitmap("/home/lcom/labs/proj/res/1.bmp");
    background.n2 = loadBitmap("/home/lcom/labs/proj/res/2.bmp");
    background.n3 = loadBitmap("/home/lcom/labs/proj/res/3.bmp");
    background.n4 = loadBitmap("/home/lcom/labs/proj/res/4.bmp");
    background.n5 = loadBitmap("/home/lcom/labs/proj/res/5.bmp");
    background.n6 = loadBitmap("/home/lcom/labs/proj/res/6.bmp");
    background.n7 = loadBitmap("/home/lcom/labs/proj/res/7.bmp");
    background.n8 = loadBitmap("/home/lcom/labs/proj/res/8.bmp");
    background.n9 = loadBitmap("/home/lcom/labs/proj/res/9.bmp");
    background.bar = loadBitmap("/home/lcom/labs/proj/res/bar.bmp");
    background.time = loadBitmap("/home/lcom/labs/proj/res/time.bmp");
    background.sun = loadBitmap("/home/lcom/labs/proj/res/sun.bmp");
    background.moon = loadBitmap("/home/lcom/labs/proj/res/moon.bmp");

    stm.mouse = loadBitmap("/home/lcom/labs/proj/res/mouse.bmp");

    spl.x1 = -100;
    spl.xtrex = -100;
    spl.x2 = H(1,1) + 100;
    spl.xtitle =  H(1,1) + 100;
    spl.xtrack =  H(1,1) + 100;
    spl.mover = 2;
    
    if(readScores() != OK) printf("SCORES NOT OK!\n");
}

void MenuMouseEventHandler(struct packet p){

    // Parsing and calculating Mouse Position
    if(stm.xi + p.delta_x >= 0 && stm.xi + p.delta_x + stm.mouse->bitmapInfoHeader.width <= H(1,1)) 
        stm.xi += p.delta_x;
    if(stm.yi - p.delta_y >= 0 && stm.yi - p.delta_y + stm.mouse->bitmapInfoHeader.height <= V(1,1)) 
        stm.yi -= p.delta_y;
    
    // Detecting EXIT Event with Mouse Position
    if(stm.xi < H(4,6) + background.exitBtn->bitmapInfoHeader.width/2 && stm.xi > H(4,6) - background.exitBtn->bitmapInfoHeader.width/2 && stm.yi < V(8,10) + background.exitBtn->bitmapInfoHeader.height && stm.yi > V(8,10)){   
        if(p.lb){
            stm.event = EXBTN;
        }
    }
    
    // Detecting PLAY Event with Mouse Position
    if(stm.xi < H(2,6) + background.playBtn->bitmapInfoHeader.width/2 && stm.xi > H(2,6) - background.playBtn->bitmapInfoHeader.width/2 && stm.yi < V(7,10) + background.playBtn->bitmapInfoHeader.height && stm.yi > V(7,10)){   
        if(p.lb){
            stm.event = SPBTN;
        }
    }

    // Detecting MULTIPLAYER Event with Mouse Position
    if(stm.xi < H(4,6) + background.multiBtn->bitmapInfoHeader.width/2 && stm.xi > H(4,6) - background.multiBtn->bitmapInfoHeader.width/2 && stm.yi < V(7,10) + background.multiBtn->bitmapInfoHeader.height && stm.yi > V(7,10)){   
        if(p.lb){
            stm.event = MPBTN;
        }
    }

    // Detecting SCORES Event with Mouse Position
    if(stm.xi < H(2,6) + background.scoresBtn->bitmapInfoHeader.width/2 && stm.xi > H(2,6) - background.scoresBtn->bitmapInfoHeader.width/2 && stm.yi < V(8,10) + background.scoresBtn->bitmapInfoHeader.height && stm.yi > V(8,10)){   
        if(p.lb){
            stm.event = SCBTN;
        }
    }

    // Detecting TREX movements with Mouse Buttons
    if(p.rb && trex.hy == V(29,60)) 
        trex.under = true;
    else 
        trex.under = false;

    if(p.mb && trex.hy == V(29,60)){
        trex.jump = true;
        trex.corrector = 8;
    }
    else if (trex.hy == V(29,60)) 
        trex.jump = false;
}

void MenuKbdEventHandler(){
    
    // Detecting EXIT Event with Keyboard Buttons
    if(scancodes[0] == ESC_BREAK_CODE){
        stm.event = EXBTN;
    }
    
    // Detecting PLAY Event with Keyboard Buttons
    else if(scancodes[0] == P_BREAK_CODE){
        stm.event = SPBTN;
    }
    
    // Detecting PLAY Event with Keyboard Buttons
    else if(scancodes[0] == M_BREAK_CODE){
        stm.event = MPBTN;
    }

    // Detecting SCORE Event with Keyboard Buttons
    else if(scancodes[0] == S_BREAK_CODE){
        stm.event = SCBTN;
    }

    // Detecting TREX movements with Keyboard Buttons
    if(scancodes[0] == TWO_BYTES_CODE && scancodes[1] == DOWN_BREAK_CODE  && trex.hy == V(29,60)) 
        trex.under = true;
    else 
        trex.under = false;

    if(scancodes[0] != TWO_BYTES_CODE && scancodes[0] == SPACE_BREAK_CODE && trex.hy == V(29,60)){
        trex.jump = true;
        trex.corrector = 8;
    }
    
    else if (trex.hy == V(29,60)) 
        trex.jump = false;        
        
}

void MenuDrawInterface(){

    Bitmap * trexs[]  = {trex.trex2,trex.trex3};
    Bitmap * trexsU[] = {trex.trex4,trex.trex5};
    Bitmap * tracks[] = {background.track1,background.track2};
        
    // Generates a new track on the screen
    if((-1)*(counter*background.trackvelocity + H(1,1))%(H(2,1)) >= 1020*2){
        background.btrack = rand() % 2;
    }
    
    vg_draw_rectangle2(0,0,H(1,1),V(1,1),0xffffff,(void *) get_double_buffer());
    drawBitmap(background.title,H(4,7),V(3,10), ALIGN_CENTER,SECOND_BUFFER);
    
    drawBitmap(background.track1,(-1)*(counter*background.trackvelocity) % (H(2,1)),V(5,12), ALIGN_RIGHT,SECOND_BUFFER);
    drawBitmap(tracks[background.btrack],(-1)*(counter*background.trackvelocity + H(1,1))%(H(2,1)),V(5,12), ALIGN_RIGHT,SECOND_BUFFER);
    
    // Drawing Trex on the screen - jumping or lowering
    if(!trex.under && !trex.jump)
        drawBitmap(trexs[(counter/8)%2],H(2,7),V(29,60), ALIGN_CENTER,SECOND_BUFFER);
    else if(trex.under)
        drawBitmap(trexsU[(counter/8)%2],H(2,7),V(29,60), ALIGN_CENTER,SECOND_BUFFER);
    else if(trex.jump){
        trex.hy -= trex.corrector;
        drawBitmap(trex.trex1,H(2,7),trex.hy,ALIGN_CENTER,SECOND_BUFFER);
        if(trex.hy >= V(29,60)){
            trex.hy = V(29,60);
            trex.jump = false;
            trex.corrector = 8;
        }
        else if(trex.hy <= V(29,60) - 120) 
            trex.corrector = -8;
    }
    
    drawBitmap(background.playBtn,H(2,6),V(7,10), ALIGN_CENTER,SECOND_BUFFER);
    drawBitmap(background.multiBtn,H(4,6),V(7,10), ALIGN_CENTER,SECOND_BUFFER);
    drawBitmap(background.scoresBtn,H(2,6),V(8,10), ALIGN_CENTER,SECOND_BUFFER);
    drawBitmap(background.exitBtn,H(4,6),V(8,10), ALIGN_CENTER,SECOND_BUFFER);
    drawBitmap(stm.mouse,stm.xi,stm.yi, ALIGN_LEFT,SECOND_BUFFER); 
    
    change_buffers();

    // Calculating new state based on current event of the stm
    if(stm.event == SPBTN){
        stSINGLEP();
        stm.event = NOTH;
    }
    else if(stm.event == SCBTN){
        stSCORES();
        stm.event = NOTH;
    }
    else if(stm.event == MPBTN){
        stMULTIP();
        stm.event = NOTH;
    }
    else if(stm.event == EXBTN)
    {
        stm.state = EXIT;
        stm.event = NOTH;
    }
    
}

int stEXIT(){
    drawBitmap(background.exitBtn_Hoover,H(4,6),V(8,10), ALIGN_CENTER,REAL_BUFFER);
    drawBitmap(stm.mouse,stm.xi,stm.yi, ALIGN_LEFT,REAL_BUFFER); 
    tickdelay(micros_to_ticks(150000));
    
    drawBitmap(background.exitBtn,H(4,6),V(8,10), ALIGN_CENTER,REAL_BUFFER);
    drawBitmap(stm.mouse,stm.xi,stm.yi, ALIGN_LEFT,REAL_BUFFER); 
    tickdelay(micros_to_ticks(1000000));
    
    vg_draw_rectangle2(0,0,H(1,1),V(1,1),0xffffff,(void *) get_double_buffer());
    
    change_buffers();
    
    deleteBitmap(trex.trex1);
    deleteBitmap(trex.trex2);
    deleteBitmap(trex.trex3);
    deleteBitmap(trex.trex4);
    deleteBitmap(trex.trex5);

    deleteBitmap(background.title);
    deleteBitmap(background.track1);
    deleteBitmap(background.track2);
    deleteBitmap(background.playBtn);
    deleteBitmap(background.playBtn_Hoover);
    deleteBitmap(background.multiBtn);
    deleteBitmap(background.multiBtn_Hoover);
    deleteBitmap(background.scoresBtn);
    deleteBitmap(background.scoresBtn_Hoover);
    deleteBitmap(background.exitBtn);
    deleteBitmap(background.exitBtn_Hoover);
    deleteBitmap(background.cloud1);
    deleteBitmap(background.cloud2);
    deleteBitmap(background.bigtree1);
    deleteBitmap(background.bigtree2);
    deleteBitmap(background.bigtree3);
    deleteBitmap(background.bigtree4);
    deleteBitmap(background.bigtree5);
    deleteBitmap(background.bigtree6);
    deleteBitmap(background.smalltree1);
    deleteBitmap(background.smalltree2);
    deleteBitmap(background.smalltree3);
    deleteBitmap(background.smalltree4);
    deleteBitmap(background.smalltree5);
    deleteBitmap(background.multitree1);
    deleteBitmap(background.multitree2);
    deleteBitmap(background.multitree3);
    deleteBitmap(background.multitree4);
    deleteBitmap(background.bird1);
    deleteBitmap(background.bird2);
    deleteBitmap(background.gameover);
    deleteBitmap(background.n0);
    deleteBitmap(background.n1);
    deleteBitmap(background.n2);
    deleteBitmap(background.n3);
    deleteBitmap(background.n4);
    deleteBitmap(background.n5);
    deleteBitmap(background.n6);
    deleteBitmap(background.n7);
    deleteBitmap(background.n8);
    deleteBitmap(background.n9);
    deleteBitmap(background.bar);
    deleteBitmap(background.time);
    deleteBitmap(background.sun);
    deleteBitmap(background.moon);

    deleteBitmap(stm.mouse);

    if(writeScores() != OK) printf("SCORES SAVING NOT OK!\n");
    
    return OK;
}

int stMENU(){ 

    // Generating the SplashScreen Animations
    if(spl.xtrack - spl.mover >= H(3,6)){
        
        Bitmap * trexs[] = {trex.trex2,trex.trex3};
        
        vg_draw_rectangle2(0,0,H(1,1),V(1,1),0xffffff,(void *) get_double_buffer());

        if(spl.x1 + spl.mover <= H(2,6)){
            drawBitmap(background.playBtn,spl.x1+spl.mover,V(7,10), ALIGN_CENTER,SECOND_BUFFER);
            drawBitmap(background.scoresBtn,spl.x1+spl.mover,V(8,10), ALIGN_CENTER,SECOND_BUFFER);
        }
        else{
            drawBitmap(background.playBtn,H(2,6),V(7,10), ALIGN_CENTER,SECOND_BUFFER);
            drawBitmap(background.scoresBtn,H(2,6),V(8,10), ALIGN_CENTER,SECOND_BUFFER);      
        }

        if(spl.x2 - spl.mover >= H(4,6)){
            drawBitmap(background.multiBtn,spl.x2-spl.mover,V(7,10), ALIGN_CENTER,SECOND_BUFFER);
            drawBitmap(background.exitBtn,spl.x2-spl.mover,V(8,10), ALIGN_CENTER,SECOND_BUFFER);
        }
        else{
            drawBitmap(background.multiBtn,H(4,6),V(7,10), ALIGN_CENTER,SECOND_BUFFER);
            drawBitmap(background.exitBtn,H(4,6),V(8,10), ALIGN_CENTER,SECOND_BUFFER);
        }

        if(spl.mover > 40 && spl.xtitle - spl.mover >= H(4,7)){
            drawBitmap(background.title,spl.xtitle - spl.mover,V(3,10), ALIGN_CENTER,SECOND_BUFFER);
        }
        else if(spl.mover > 40){
            drawBitmap(background.title,H(4,7),V(3,10), ALIGN_CENTER,SECOND_BUFFER);
        }

        if(spl.mover > 30 && spl.xtrack - spl.mover >= H(3,6)){
            drawBitmap(background.track1,spl.xtrack - spl.mover,V(5,12), ALIGN_CENTER,SECOND_BUFFER);
        } 
        else if(spl.mover > 30){
            drawBitmap(background.track1,H(3,6),V(5,12), ALIGN_CENTER,SECOND_BUFFER);
        }   

        if(spl.mover > 50 && spl.xtrex + spl.mover <= H(2,7)){
            drawBitmap(trexs[(counter/8)% 2],spl.xtrex + spl.mover,V(29,60), ALIGN_CENTER,SECOND_BUFFER);
        }
        else if(spl.mover > 50){
            drawBitmap(trexs[(counter/8)% 2],H(2,7),V(29,60), ALIGN_CENTER,SECOND_BUFFER);
        }
        
        change_buffers();
        
        spl.mover += 5;
        
        return NOT_OK;
    }

    spl.x1 = -100;
    spl.xtrex = -100;
    spl.x2 = H(1,1) + 100;
    spl.xtitle =  H(1,1) + 100;
    spl.xtrack =  H(1,1) + 100;
    spl.mover = 2;

    trex.under = false;
    trex.jump = false;
    trex.corrector = 8;
    trex.hy = V(29,60);

    srand(time(0));

    stm.xi = rand() % (H(1,1) - 50) + 10;
    stm.yi = rand() % (V(1,1) - 50) + 10;

    background.trackvelocity = 5;    

    return OK; 
}

int stMULTIP(){ 
    
    //Multiplayer was not implemented! Lack of Time, beyond other reasons.
    drawBitmap(background.multiBtn_Hoover,H(4,6),V(7,10), ALIGN_CENTER,REAL_BUFFER);
    drawBitmap(stm.mouse,stm.xi,stm.yi, ALIGN_LEFT,REAL_BUFFER); 
    tickdelay(micros_to_ticks(150000));
    
    drawBitmap(background.multiBtn,H(4,6),V(7,10), ALIGN_CENTER,REAL_BUFFER);
    drawBitmap(stm.mouse,stm.xi,stm.yi, ALIGN_LEFT,REAL_BUFFER); 
    tickdelay(micros_to_ticks(1000000));

    return OK;
}

int H(int n, int d){ return n*getHres()/d;}
int V(int n, int d){ return n*getVres()/d;}
