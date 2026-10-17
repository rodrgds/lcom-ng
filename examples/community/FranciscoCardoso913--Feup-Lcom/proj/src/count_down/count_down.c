#include "count_down.h"
int seconds_left=0,first,second,third;
int x=150,y=0;

int(start_counter)(int time){
    if(time<0) return 1;
    seconds_left=time;
    first= (int)time/100;
    second=(int) (time-first*100)/10;
    third=(int)(time-first*100-second*10);
    
    return 0;
}
int(draw_timer)(){
    if(draw_number((x+30),y, first)){ 
        printf("Error while drawing the timer first number");
        return 1;
    }
    if(draw_number((x+60),y, second)){ 
        printf("Error while drawing the second first number");
        return 1;
    }
    if(draw_number((x+90),y, third)){ 
        printf("Error while drawing the timer third number");
        return 1;
    }
    return 0;
}

int(decrement_counter)(){
    
    return start_counter(--seconds_left);
}

void(set_timer_pos)(int x_, int y_){
    x=x_;
    y=y_;
}

int(clear_timer)(){
    for(int i=0; i<54;i++){
        for(int j=0; j<54;j++){
            if(replace_with_background(x+j,y+i)) return 1;
        }
    }
    return 0;
}

