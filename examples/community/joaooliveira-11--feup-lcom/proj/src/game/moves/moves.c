#include "moves.h"
#include <math.h>

extern mouse_t mouse_structure;

extern struct gamecontext context;

void move_FBup() {
    set_sprite_ypos(&context.sprites[SPRITE_BOY_Idx], context.sprites[SPRITE_BOY_Idx].ypos - 5);
}

void move_FBdown() {
    set_sprite_ypos(&context.sprites[SPRITE_BOY_Idx], context.sprites[SPRITE_BOY_Idx].ypos + 5);
}

void move_FBright() {
    set_sprite_xpos(&context.sprites[SPRITE_BOY_Idx], context.sprites[SPRITE_BOY_Idx].xpos + 5);
}

void move_FBleft() {
    set_sprite_xpos(&context.sprites[SPRITE_BOY_Idx], context.sprites[SPRITE_BOY_Idx].xpos - 5);
}

void move_WGup() {
    set_sprite_ypos(&context.sprites[SPRITE_GIRL_Idx], context.sprites[SPRITE_GIRL_Idx].ypos - 5);
}

void move_WGdown() {
    set_sprite_ypos(&context.sprites[SPRITE_GIRL_Idx],  context.sprites[SPRITE_GIRL_Idx].ypos + 5);
}

void move_WGright() {
     set_sprite_xpos(&context.sprites[SPRITE_GIRL_Idx],  context.sprites[SPRITE_GIRL_Idx].xpos + 5);
}

void move_WGleft() {
    set_sprite_xpos(&context.sprites[SPRITE_GIRL_Idx], context.sprites[SPRITE_GIRL_Idx].xpos - 5);
}



int check_FBmoveUP() {
    return check_colisions(context.sprites[SPRITE_BOY_Idx].xpos, context.sprites[SPRITE_BOY_Idx].ypos - 5,0);
}

int check_FBmoveDOWN() {
    return check_colisions(context.sprites[SPRITE_BOY_Idx].xpos, context.sprites[SPRITE_BOY_Idx].ypos + 5,0);
}

int check_FBmoveRIGHT() {
    return check_colisions(context.sprites[SPRITE_BOY_Idx].xpos + 5, context.sprites[SPRITE_BOY_Idx].ypos,0);
}

int check_FBmoveLEFT() {
    return check_colisions(context.sprites[SPRITE_BOY_Idx].xpos - 5, context.sprites[SPRITE_BOY_Idx].ypos,0);
}

int check_WGmoveUP() {
    return check_colisions(context.sprites[SPRITE_GIRL_Idx].xpos, context.sprites[SPRITE_GIRL_Idx].ypos - 5,1);
}

int check_WGmoveDOWN() {
    return check_colisions(context.sprites[SPRITE_GIRL_Idx].xpos, context.sprites[SPRITE_GIRL_Idx].ypos + 5,1);
}

int check_WGmoveRIGHT() {
     return check_colisions(context.sprites[SPRITE_GIRL_Idx].xpos + 5, context.sprites[SPRITE_GIRL_Idx].ypos,1);
}

int check_WGmoveLEFT() {
    return check_colisions(context.sprites[SPRITE_GIRL_Idx].xpos - 5, context.sprites[SPRITE_GIRL_Idx].ypos,1);
}


int check_colisions(uint16_t targetXPOS, uint16_t targetYPOS, int PlayerType) {
    uint16_t targetWidth = (PlayerType == 0) ? context.sprites[SPRITE_BOY_Idx].width : context.sprites[SPRITE_GIRL_Idx].width;
    uint16_t targetHeight = (PlayerType == 0) ? context.sprites[SPRITE_BOY_Idx].height : context.sprites[SPRITE_GIRL_Idx].height;

    if (targetXPOS <= 20 || targetYPOS <= 15 || targetXPOS + targetWidth >= 780 || targetYPOS + targetHeight >= 580) return 1;


    for (int i = 0; i < context.numWalls; i++) {
        uint16_t wallX = context.walls[i].x;
        uint16_t wallY = context.walls[i].y;

        uint16_t wallWidth = 21;
        uint16_t wallHeight = 20;

        if (targetXPOS + targetWidth >= wallX && targetXPOS <= wallX + wallWidth &&
            targetYPOS + targetHeight >= wallY && targetYPOS <= wallY + wallHeight) {
            return 1;
        }
    }

    for (int i = 0; i < context.numTraps; i++) {
        uint16_t trapX = context.traps[i].x;
        uint16_t trapY = context.traps[i].y;
        uint8_t trapType = context.traps[i].type;

        uint16_t trapWidth = 40;
        uint16_t trapHeight = 10;

        if (targetXPOS + targetWidth >= trapX && targetXPOS <= trapX + trapWidth &&
            targetYPOS + targetHeight >= trapY && targetYPOS <= trapY + trapHeight && (PlayerType == trapType)) {
                context.gamestate = GAMEOVER_MENU;
        }
    }

    for (int i = 0; i < 3; i++) {
        if (targetXPOS + targetWidth >= context.levers[i].x && targetXPOS <= context.levers[i].x + context.sprites[SPRITE_LEVERleft_Idx].width &&
            targetYPOS + targetHeight >= context.levers[i].y && targetYPOS <= context.levers[i].y + context.sprites[SPRITE_LEVERleft_Idx].height) {
            return 1;
        }
        if (targetXPOS + targetWidth >= context.levers_match[i].x && targetXPOS <= context.levers_match[i].x + context.sprites[SPRITE_LEVERleft_Idx].width &&
            targetYPOS + targetHeight >= context.levers_match[i].y && targetYPOS <= context.levers_match[i].y + context.sprites[SPRITE_LEVERleft_Idx].height) {
            return 1;
        }
    }

    for(int i = 0; i < 3; i++){
        if(!(context.barriers[i].is_open)){
            if (targetXPOS + targetWidth >= context.barriers[i].x && targetXPOS <= context.barriers[i].x + context.sprites[SPRITE_BARRIER_Idx].width &&
                targetYPOS + targetHeight >= context.barriers[i].y && targetYPOS <= context.barriers[i].y + context.sprites[SPRITE_BARRIER_Idx].height) {
                return 1;
            }
        }
    }
    
    return 0;
}

void reset_barrier(){
    for (int i = 0; i < 3; i++) {
        if(context.barriers[i].is_open){
            context.barriers[i].is_open = 0;
            context.levers[i].is_pressed = 0;
            context.levers_match[i].is_pressed = 0;
            context.startLeversCountdown = 0;
            context.levers_countdown = 0;
        }
    }
}

int check_nearBY(lever_t lever, int PlayerType) {
    uint16_t targetWidth = (PlayerType == 0) ? context.sprites[SPRITE_BOY_Idx].width : context.sprites[SPRITE_GIRL_Idx].width;
    uint16_t targetHeight = (PlayerType == 0) ? context.sprites[SPRITE_BOY_Idx].height : context.sprites[SPRITE_GIRL_Idx].height;

    int indx = (PlayerType == 0) ? SPRITE_BOY_Idx : SPRITE_GIRL_Idx;

    int playerCenterX = context.sprites[indx].xpos + targetWidth / 2;
    int playerCenterY = context.sprites[indx].ypos + targetHeight / 2;

    int leverCenterX = lever.x + context.sprites[SPRITE_LEVERleft_Idx].width / 2;
    int leverCenterY = lever.y + context.sprites[SPRITE_LEVERleft_Idx].height / 2;


    int distance = sqrt(pow(playerCenterX - leverCenterX, 2) + pow(playerCenterY - leverCenterY, 2));

    if (distance <= 40) {
        return 0; 
    }

    return 1;
}

void mouse_MENU(){
    if(mouse_structure.xpos >= context.sprites[SPRITE_PLAYbtn_Idx].xpos && mouse_structure.xpos <= context.sprites[SPRITE_PLAYbtn_Idx].xpos + context.sprites[SPRITE_PLAYbtn_Idx].width
     && mouse_structure.ypos >= context.sprites[SPRITE_PLAYbtn_Idx].ypos && mouse_structure.ypos <= context.sprites[SPRITE_PLAYbtn_Idx].ypos + context.sprites[SPRITE_PLAYbtn_Idx].height
     ){
        context.sprites[SPRITE_PLAYbtn_Idx].is_pressed = 1;
    }

    else if(mouse_structure.xpos >= context.sprites[SPRITE_INSTbtn_Idx].xpos && mouse_structure.xpos <= context.sprites[SPRITE_INSTbtn_Idx].xpos + context.sprites[SPRITE_INSTbtn_Idx].width
     && mouse_structure.ypos >= context.sprites[SPRITE_INSTbtn_Idx].ypos && mouse_structure.ypos <= context.sprites[SPRITE_INSTbtn_Idx].ypos + context.sprites[SPRITE_INSTbtn_Idx].height
     ){
        context.sprites[SPRITE_INSTbtn_Idx].is_pressed = 1;
    }

    else if(mouse_structure.xpos >= context.sprites[SPRITE_QUITbtn_Idx].xpos && mouse_structure.xpos <= context.sprites[SPRITE_QUITbtn_Idx].xpos + context.sprites[SPRITE_QUITbtn_Idx].width
     && mouse_structure.ypos >= context.sprites[SPRITE_QUITbtn_Idx].ypos && mouse_structure.ypos <= context.sprites[SPRITE_QUITbtn_Idx].ypos + context.sprites[SPRITE_QUITbtn_Idx].height
     ){
        context.sprites[SPRITE_QUITbtn_Idx].is_pressed = 1;
    }
}

void mouse_INSTRUCTIONS(){
    if(mouse_structure.xpos >= context.sprites[SPRITE_BACKbtn_Idx].xpos && mouse_structure.xpos <= context.sprites[SPRITE_BACKbtn_Idx].xpos + context.sprites[SPRITE_BACKbtn_Idx].width
     && mouse_structure.ypos >= context.sprites[SPRITE_BACKbtn_Idx].ypos && mouse_structure.ypos <= context.sprites[SPRITE_BACKbtn_Idx].ypos + context.sprites[SPRITE_BACKbtn_Idx].height
     ){
        context.sprites[SPRITE_BACKbtn_Idx].is_pressed = 1;
    }
}

void mouse_PLAYING(){
    for(int i = 0; i < 3; i++){
        if(
            !(context.levers[i].is_pressed) &&
            mouse_structure.xpos >= context.levers[i].x && mouse_structure.xpos <= context.levers[i].x + context.sprites[SPRITE_LEVERleft_Idx].width &&
            mouse_structure.ypos >= context.levers[i].y && mouse_structure.ypos <= context.levers[i].y + context.sprites[SPRITE_LEVERleft_Idx].height
        ){
            if(check_nearBY(context.levers[i], 0) == 0 || check_nearBY(context.levers[i], 1) == 0){
                context.levers[i].is_pressed = 1; 
                context.levers_match[i].is_pressed = 0; 
                context.barriers[i].is_open = 1;
                context.startLeversCountdown = 1;
            }
        }
        if(
            !(context.levers_match[i].is_pressed) &&
            mouse_structure.xpos >= context.levers_match[i].x && mouse_structure.xpos <= context.levers_match[i].x + context.sprites[SPRITE_LEVERleft_Idx].width &&
            mouse_structure.ypos >= context.levers_match[i].y && mouse_structure.ypos <= context.levers_match[i].y + context.sprites[SPRITE_LEVERleft_Idx].height
        )
        {
            if(check_nearBY(context.levers_match[i], 0) == 0 || check_nearBY(context.levers_match[i], 1) == 0){
                context.levers[i].is_pressed = 0;
                context.levers_match[i].is_pressed = 1; 
                context.barriers[i].is_open = 1; 
                context.startLeversCountdown = 1;
            }
        }    
    }
}

void mouse_GAMEOVER(){
    if(mouse_structure.xpos >= context.sprites[SPRITE_MAINMENUbtn_Idx].xpos && mouse_structure.xpos <= context.sprites[SPRITE_MAINMENUbtn_Idx].xpos + context.sprites[SPRITE_MAINMENUbtn_Idx].width
     && mouse_structure.ypos >= context.sprites[SPRITE_MAINMENUbtn_Idx].ypos && mouse_structure.ypos <= context.sprites[SPRITE_MAINMENUbtn_Idx].ypos + context.sprites[SPRITE_MAINMENUbtn_Idx].height
     ){
        context.sprites[SPRITE_MAINMENUbtn_Idx].is_pressed = 1;
    }
}

void mouse_GAMEWIN(){
    if(mouse_structure.xpos >= context.sprites[SPRITE_MAINMENUbtn_Idx].xpos && mouse_structure.xpos <= context.sprites[SPRITE_MAINMENUbtn_Idx].xpos + context.sprites[SPRITE_MAINMENUbtn_Idx].width
     && mouse_structure.ypos >= context.sprites[SPRITE_MAINMENUbtn_Idx].ypos && mouse_structure.ypos <= context.sprites[SPRITE_MAINMENUbtn_Idx].ypos + context.sprites[SPRITE_MAINMENUbtn_Idx].height
     ){
        context.sprites[SPRITE_MAINMENUbtn_Idx].is_pressed = 1;
    }
}
void reset_mouseClicks(){
    context.sprites[SPRITE_PLAYbtn_Idx].is_pressed = 0;
    context.sprites[SPRITE_QUITbtn_Idx].is_pressed = 0;
    context.sprites[SPRITE_INSTbtn_Idx].is_pressed = 0;
    context.sprites[SPRITE_BACKbtn_Idx].is_pressed = 0;
    context.sprites[SPRITE_MAINMENUbtn_Idx].is_pressed = 0;
}

void check_mouse_clicks() {
    if(mouse_structure.lb){ 
        funcs1[context.gamestate](); 
    }
    else {
        reset_mouseClicks();
    }
}

void check_gameWin() {
    if ((context.sprites[SPRITE_BOY_Idx].xpos + context.sprites[SPRITE_BOY_Idx].width >= 663 && context.sprites[SPRITE_BOY_Idx].xpos +  context.sprites[SPRITE_BOY_Idx].width <= 707  &&
            context.sprites[SPRITE_BOY_Idx].ypos + context.sprites[SPRITE_BOY_Idx].height  >= 27 && context.sprites[SPRITE_BOY_Idx].ypos + context.sprites[SPRITE_BOY_Idx].height <= 80) && 
        (context.sprites[SPRITE_GIRL_Idx].xpos + context.sprites[SPRITE_GIRL_Idx].width >= 715 && context.sprites[SPRITE_GIRL_Idx].xpos +  context.sprites[SPRITE_GIRL_Idx].width <= 760  &&
            context.sprites[SPRITE_GIRL_Idx].ypos + context.sprites[SPRITE_GIRL_Idx].height  >= 27 && context.sprites[SPRITE_GIRL_Idx].ypos + context.sprites[SPRITE_GIRL_Idx].height <= 80)){
                context.gamestate = GAMEWIN_MENU;
            } 
}
