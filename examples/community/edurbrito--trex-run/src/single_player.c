#include <lcom/lcf.h>

#include "main_menu.h"
#include "single_player.h"
#include "score.h"

extern unsigned int counter;
extern uint8_t scancodes[2];
extern bool two_byte_code;

extern Interface background;
extern Trex trex;
extern MainMenuStM stm;
Score score;

int stSINGLEP(){

    drawBitmap(background.playBtn_Hoover,H(2,6),V(7,10), ALIGN_CENTER,REAL_BUFFER);
    drawBitmap(stm.mouse,stm.xi,stm.yi, ALIGN_LEFT,REAL_BUFFER); 
    tickdelay(micros_to_ticks(150000));
    
    drawBitmap(background.playBtn,H(2,6),V(7,10), ALIGN_CENTER,REAL_BUFFER);
    drawBitmap(stm.mouse,stm.xi,stm.yi, ALIGN_LEFT,REAL_BUFFER); 
    tickdelay(micros_to_ticks(1000000));

    vg_draw_rectangle2(0,0,H(1,1),V(1,1),0xffffff,(void *) get_double_buffer());
    
    change_buffers();

    stm.state = SINGLEP;

    srand(12345);
    int random = rand();
        
    scancodes[0] = 0x00;
    scancodes[1] = 0x00;

    background.collisionCount = 0; 
    background.collisionLimit = 11;
    background.obstacle = 0;
    background.bcloudHeight = V(random % 2 + 3,12);
    background.trackvelocity = 9;
    background.cloudsvelocity = 1;
    background.trackHeight = V(5,12);
    score.score = 0;
    
    // Calculating Sun or Moon Position
    uint32_t sec = 0, min = 0, hour = 0;
    get_time(&sec,&min,&hour);
    if(hour >=6 && hour <= 18){
        background.sunX = H(hour,24) + min*(H(1,24))/60;
        background.day = true;
    }
    else{
        background.sunX = ((hour+12)%24)*H(1,24) + min*(H(1,24))/60;
        background.day = false;

    }
    counter = 0;
        
    return OK;
}

void PlayKbdEventHandler(){

    // Detecting TREX movements with Keyboard Buttons
    if(scancodes[0] == TWO_BYTES_CODE && scancodes[1] == DOWN_BREAK_CODE  && trex.hy == V(29,60)) 
        trex.under = true;
    else 
        trex.under = false;

    if(scancodes[0] != TWO_BYTES_CODE && scancodes[0] == SPACE_BREAK_CODE && trex.hy == V(29,60)){
        trex.jump = true;
        trex.corrector = 10;
    }
    
    else if (trex.hy == V(29,60)) 
        trex.jump = false;

}

int PlayDrawInterface(){

    bool changeV = false;
    bool detectedCollision = false;
    int obstacleX = (-1)*(counter*background.trackvelocity + H(1,1))%(H(2,1));

    Bitmap * trexs[]      = {trex.trex2,trex.trex3};
    Bitmap * trexsU[]     = {trex.trex4,trex.trex5};
    Bitmap * tracks[]     = {background.track1,background.track2};
    Bitmap * smalltrees[] = {background.smalltree1, background.smalltree2, background.smalltree3, background.smalltree5, background.multitree1,background.multitree2,background.multitree3,background.multitree4};
    Bitmap * clouds[]     = {background.cloud1,background.cloud2}; 
    Bitmap * birds[]      = {background.bird1,background.bird2};
    
    // Generating Obstacle Position
    if(obstacleX >= 1020*2){
        int random = rand();
        background.btrack = random % 2;
        background.bsmalltree = random % 8;
        background.obstacle = random % 2;
        changeV = true;
    }

    // Generating Cloud
    if((-1)*(counter*background.cloudsvelocity + H(1,1))%(H(2,1)) >= 1020*2){
        int random = rand();
        background.bcloud = random % 2;   
        background.bcloudHeight = V(random % 2 + 3,12);
    }
    
    vg_draw_rectangle2(0,0,H(1,1),V(1,1),0xffffff,(void *) get_double_buffer());
    vg_draw_rectangle2(0,0,H(1,1),V(1,1),0xf81f,(void *) get_rd_buffer());

    // Updating Sun Position
    if(counter%(60*60) == 0) background.sunX += (H(1,24))/30;

    if(background.day)
        drawBitmap(background.sun,background.sunX,V(2,12),ALIGN_CENTER,SECOND_BUFFER);
    else
        drawBitmap(background.moon,background.sunX,V(2,12),ALIGN_CENTER,SECOND_BUFFER);

    drawBitmap(background.track1,(-1)*(counter*background.trackvelocity)%(H(2,1)),background.trackHeight, ALIGN_RIGHT,SECOND_BUFFER);
    drawBitmap(tracks[background.btrack],obstacleX,background.trackHeight, ALIGN_RIGHT,SECOND_BUFFER);    
    drawBitmap(clouds[background.bcloud],(-1)*(counter*background.cloudsvelocity + H(1,1))%(H(2,1)), background.bcloudHeight,ALIGN_RIGHT,SECOND_BUFFER);
    
    // Drawing Trex on the screen - jumping or lowering
    if(!trex.under && !trex.jump){
        drawBitmap(trexs[(counter/8)%2],H(2,7),V(29,60), ALIGN_CENTER,SECOND_BUFFER);
        drawBitmap(trexs[(counter/8)%2],H(2,7),V(29,60), ALIGN_CENTER,THIRD_BUFFER); 
    }

    else if(trex.under){
        drawBitmap(trexsU[(counter/8)%2],H(2,7),V(29,60), ALIGN_CENTER,SECOND_BUFFER);
        drawBitmap(trexsU[(counter/8)%2],H(2,7),V(29,60), ALIGN_CENTER,THIRD_BUFFER);   
    }

    else if(trex.jump){
        trex.hy -= trex.corrector;
        drawBitmap(trex.trex1,H(2,7),trex.hy,ALIGN_CENTER,SECOND_BUFFER);
        drawBitmap(trex.trex1,H(2,7),trex.hy,ALIGN_CENTER,THIRD_BUFFER);
     
        if(trex.hy >= V(29,60)){
            trex.hy = V(29,60);
            trex.jump = false;
            trex.corrector = 18;
        }
        else if(trex.hy <= V(29,60) - 160) trex.corrector = -14;
    }
    
    // Changing Velocity according to the score
    if(changeV && ((score.score) % 10) == 0){
        background.trackvelocity += 1;
        background.collisionLimit -= 1;
    }
    
    vg_draw_rectangle2(0,background.trackHeight + 145,H(1,1),V(1,1),0xffffff,(void *) get_double_buffer());

    int collision = 0;

    // Drawing Obstacle on the screen
    if(background.obstacle == 0){
        background.bsmalltree = background.bsmalltree%4 + 4;
        if(background.bsmalltree > 3){
            collision = drawBitmap_collision(smalltrees[background.bsmalltree],obstacleX,V(28,60), ALIGN_LEFT);
            drawBitmap(smalltrees[background.bsmalltree],obstacleX,V(28,60), ALIGN_RIGHT,SECOND_BUFFER);
        }
        else{
            collision = drawBitmap_collision(smalltrees[background.bsmalltree],obstacleX,V(30,60), ALIGN_LEFT);
            drawBitmap(smalltrees[background.bsmalltree],obstacleX,V(30,60), ALIGN_RIGHT,SECOND_BUFFER);
        }
    }
    else{
        collision = drawBitmap_collision(birds[(counter/7)%2],obstacleX,V(6,14) + 70 - (background.btrack * 60), ALIGN_LEFT);
        drawBitmap(birds[(counter/7)%2],obstacleX,V(6,14) + 70 - (background.btrack * 60), ALIGN_LEFT,SECOND_BUFFER);
    }
    
    // Drawing the score on the Screen
    score.score = counter/5;    
    drawDigits(score.score,H(1,2) + 60,background.trackHeight+200,8);

    change_buffers();

    // Detecting the collision - Comment the following line for debug purposes
    if(collision == 1) detectedCollision = true;
    
    // Handling Collision Detection - Game Over when Collision Limit Reached
    if(detectedCollision) {
        background.collisionCount++;
        if(background.collisionCount >= background.collisionLimit){
            drawBitmap(background.gameover,H(1,2),background.trackHeight+300, ALIGN_CENTER,REAL_BUFFER);
            background.collisionCount = 0;
            get_time(&score.sec,&score.min,&score.hour);
            get_date(&score.day,&score.month,&score.year);
            
            updateScores();
            sleep(2);

            return 1;
        }
    }
    else{
        background.collisionCount -= 2;
        if(background.collisionCount < 0)
            background.collisionCount = 0;
    }

    return 0;
}
