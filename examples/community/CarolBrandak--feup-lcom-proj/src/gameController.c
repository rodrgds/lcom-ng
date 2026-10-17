#include "gameController.h"
extern uint8_t data;
struct Game gameController;
bool ready=true;
bool pressed=false;
bool quit=false;
bool pauseOption1=true;
bool frogIdle=true;
int logIndex;
int turtleIndex;
Direction direction=UP;
extern bool codeDone;
extern int counter;
extern uint8_t hour;
extern uint8_t minute;
extern bool codeDoneMouse;
struct packet pack;


void initGame(){
  Mouse mouse;
  initMouse(&mouse);
  gameController.mouse=mouse;
  gameController.gameState=MENU;
  gameController.mouse=mouse;
  gameController.day=true;
  gameController.time[0]=DELAY_HOUR;
  loadMap();
}

void playGame(){
  struct Entity entFrog;
  struct Entity entLog;
  struct Entity entTurtle;
  struct Entity entVehicle;
  initEntity(&entVehicle,VEHICLE);
  initEntity(&entFrog,FROG);
  initEntity(&entLog,LOG);
  initEntity(&entTurtle,TURTLE);
  gameController.entities[0]=entFrog;
  gameController.entities[1]=entLog;
  gameController.entities[2]=entTurtle;
  gameController.entities[3]=entVehicle;
  gameController.level=1;
  gameController.lives=5;
  direction=UP;
}

void checkPauseOption(){
  if (pauseOption1){
    gameController.gameState=ALIVE;
  }
  else{

    gameController.gameState=MENU;
    resetMouse(&gameController.mouse);
    pauseOption1=true;
  }
}


void updateMouse(struct packet pp){
  if (pp.delta_x>0){
    if (gameController.mouse.x +pp.delta_x> xRes-gameController.mouse.img1.width){
      gameController.mouse.x = xRes-gameController.mouse.img1.width;
    }
    else{
      gameController.mouse.x += pp.delta_x;
    }
  }
  if (pp.delta_x<0){
    if (gameController.mouse.x +pp.delta_x < 0){
      gameController.mouse.x = 0;
    }
    else{
      gameController.mouse.x += pp.delta_x;
    }
  }
  if (pp.delta_y>0){
    if (gameController.mouse.y - pp.delta_y< 0){
      gameController.mouse.y = 0;
    }
    else{
      gameController.mouse.y -= pp.delta_y;
    }
  }
  if (pp.delta_y<0){
    if (gameController.mouse.y - pp.delta_y + gameController.mouse.img1.height > yRes){
      gameController.mouse.y = yRes - gameController.mouse.img1.height;
    }
    else{
      gameController.mouse.y -= pp.delta_y;
    }
  }

  
}

void DeviceHandler(Device device){
  switch (device)
  {
  case TIMER:
    switch (gameController.gameState)
    {
    case ALIVE:
      checkEvent();
      if (gameController.gameState==MENU){
        break;
      }
      if (gameController.gameState==PAUSE){
        break;
      }

      entMove();
      entDraw();
      if (!frogIdle){
        frogAnimation(&gameController.entities[0].frog,&frogIdle);
      }
      break;
    case MENU:
      checkMouseEvent(pack);
      if (counter % MOUSE_FPS ==0){
        drawMenu();

      }
      drawMouse(gameController.mouse);
      break;  
    case PAUSE:
      checkMouseEvent(pack);
      if (counter % MOUSE_FPS ==0){
        drawPauseOption(pauseOption1);
      }
      drawMouse(gameController.mouse);
      
      break;
    case HELP:
      drawHelp();  
    default:
      break;
    }
    break;
  case KEYBOARD:
    switch (gameController.gameState)
    {
    case ALIVE:
      if (data==0x81){
        gameController.gameState=PAUSE;
        resetMouse(&gameController.mouse);
        
        break;
      }
      if (codeDone){
        if ((data >> 7)==1){
          ready=true;
        }
        else{
          if (data==UP_ARROW && ready){
            if (moveFrog(&gameController.entities[0],UP)){
              direction=UP;
              frogIdle=false;
              ready=false;
            }      
          }
          if (data==LEFT_ARROW && ready){
            if (moveFrog(&gameController.entities[0],LEFT)){
              direction=LEFT;
              frogIdle=false;
              ready=false;
            }       
          }
          if (data==RIGHT_ARROW && ready){
            if (moveFrog(&gameController.entities[0],RIGHT)){
              direction=RIGHT;
              frogIdle=false;
              ready=false;
            }      
          }
          if(data==DOWN_ARROW && ready){  
            if (moveFrog(&gameController.entities[0],DOWN)){
              direction=DOWN;
              frogIdle=false;
              ready=false;
            }                
          }
        }
      }
      break;
    case MENU:
      if (data==0x81){
        quit=true;
        break;
      }
    case HELP:
      if (data==0x81){
        gameController.gameState=MENU;
        resetMouse(&gameController.mouse);
        break;
      }  
    case PAUSE:
      if (data==0x81){
        gameController.gameState=ALIVE;
        
        break;
      }
      if (codeDone){
        if ((data >> 7)==1){
          ready=true;
        }
        else{
          if (data==UP_ARROW && ready){
            pauseOption1=true;
                    
          }
          if(data==DOWN_ARROW && ready){  
            pauseOption1=false;
                             
          }
          if(data==ENTER && ready){  
            checkPauseOption();
            break;           
          }
        }
      }
      break;  
    default:
      break;
    }
    break;
  case RTC:
    switch (gameController.gameState)
    {
    case ALIVE:
      if (hour>=18 || hour <= 7){
        gameController.day=false;
      }
      else{
        gameController.day=true;
      }
      gameController.time[0]=hour;
      gameController.time[1]=minute;
      break;
    case MENU:
      if (hour>=18 || hour <= 7){
        gameController.day=false;
      }
      else{
        gameController.day=true;
      }
      gameController.time[0]=hour;
      gameController.time[1]=minute;
      break;  
    
    default:
      break;
    }
    break;
  case MOUSE:
   
    switch (gameController.gameState)
      {
      case MENU:
        if (codeDoneMouse){
          pack=getPacket();
          updateMouse(pack);
        }
        break;
      case PAUSE:
        if (codeDoneMouse){
          pack=getPacket();
          updateMouse(pack);
          
        }
        break;   
    default:
      break;
    }   
  default:
    break;
  }
}


void entMove(){
  moveLogs(&gameController.entities[1]);
  moveTurtles(&gameController.entities[2]);
  moveVehicles(&gameController.entities[3]);
  if(frogOnLog(gameController.entities[0],gameController.entities[1],&logIndex) && canFrogMove(gameController.entities[0].frog,0,gameController.entities[1].logs[0].speed)){
    gameController.entities[0].frog.x+=gameController.entities[1].logs[logIndex].speed;
  }
  if (frogOnTurtle(gameController.entities[0],gameController.entities[2],&turtleIndex) && canFrogMove(gameController.entities[0].frog,1,gameController.entities[2].turtles[0].speed)){
    gameController.entities[0].frog.x-=gameController.entities[2].turtles[turtleIndex].speed;
  }
}


void entDraw(){
  drawBackground(gameController.day);
  drawBar(gameController.level,gameController.lives,gameController.time);
  drawEntity(gameController.entities[1]);
  drawEntity(gameController.entities[2]);
  drawEntity(gameController.entities[3]);
  drawFrog(gameController.entities[0],direction);
  vg_copy();
}

void updateEntitiesSpeed(){
  for (int i=0;i<gameController.entities[1].logNr;i++){
    gameController.entities[1].logs[i].speed+=1;
  }
  for (int i=0;i<gameController.entities[2].turtleNr;i++){
    gameController.entities[2].turtles[i].speed+=1;
  }
  for (int i=0;i<gameController.entities[3].vehicleNr;i++){
    if (gameController.entities[3].vehicles[i].car){
      gameController.entities[3].vehicles[i].speed+=1;
    }
    else{
      gameController.entities[3].vehicles[i].speed+=1;
    }
    
  }
}


void nextLevel(){
  gameController.level+=1;
  updateEntitiesSpeed();
}

void checkEvent(){
  if (gameController.entities[0].frog.y<90-(gameController.entities[0].frog.img[0].height)){
    nextLevel();
    resetFrog(&gameController.entities[0].frog);
  }
  
  if (100<gameController.entities[0].frog.y+ (gameController.entities[0].frog.img[0].height/2) && gameController.entities[0].frog.y + (gameController.entities[0].frog.img[0].height/2)<400){
    if (!frogOnLog(gameController.entities[0],gameController.entities[1],&logIndex)){
      if (!frogOnTurtle(gameController.entities[0],gameController.entities[2],&turtleIndex) || gameController.entities[2].UnderWaterTurtles){
        gameController.lives--;
        if (gameController.lives==0){
          gameController.gameState=MENU;

           
        }
        else{
          resetFrog(&gameController.entities[0].frog);
        }
        
      }
    }
  }
  if (464<gameController.entities[0].frog.y+ (gameController.entities[0].frog.img[0].height/2) && gameController.entities[0].frog.y + (gameController.entities[0].frog.img[0].height/2)<764){
    if (frogOnVehicle(gameController.entities[0],gameController.entities[3])){
      gameController.lives--;
      if (gameController.lives==0){
        gameController.gameState=MENU;
        
      }
      else{
        resetFrog(&gameController.entities[0].frog);
      }
    }
  }
  if (counter%TURTLE_DIVE==0){
    switchTurtlesUnderwater(&gameController.entities[2]);
  }
  switchTurtlesDay(&gameController.entities[2],gameController.day);
  
  
}

void checkMouseEvent(struct packet pp){
  if (gameController.gameState==MENU){
    if (345<=gameController.mouse.x&&gameController.mouse.x<=800 && 325<=gameController.mouse.y&&gameController.mouse.y<=430){
      gameController.mouse.hover=true;
      if (pp.lb && !pressed){
        pressed=true;
      }
      else if (!pp.lb && pressed){
        pressed=false;
        gameController.gameState=ALIVE;
        playGame();
      }
    }
    else if (345<=gameController.mouse.x&&gameController.mouse.x<=800 && 465<=gameController.mouse.y&&gameController.mouse.y<=570){
      gameController.mouse.hover=true;
      if (pp.lb && !pressed){
        pressed=true;
      }
      else if (!pp.lb && pressed){
        pressed=false;
        gameController.gameState=HELP;
      }
    }
    
    else if (345<=gameController.mouse.x&&gameController.mouse.x<=800 && 600<=gameController.mouse.y&&gameController.mouse.y<=705){
      gameController.mouse.hover=true;
      if (pp.lb && !pressed){
        pressed=true;
      }
      else if (!pp.lb && pressed){
        pressed=false;
        quit=true;
      }
    }
    
    else{
      gameController.mouse.hover=false;
      pressed=false;
    }
  }
  if (gameController.gameState==PAUSE){
    if (290<=gameController.mouse.x&&gameController.mouse.x<=845 && 230<=gameController.mouse.y&&   gameController.mouse.y<=350){
      gameController.mouse.hover=true;
      pauseOption1=true;
      if (pp.lb && !pressed){
        pressed=true;
      }
      else if (!pp.lb && pressed){
        pressed=false;
        gameController.gameState=ALIVE;
    
      }
    }
    else if (290<=gameController.mouse.x&&gameController.mouse.x<=845 && 465<=gameController.mouse.y&&gameController.mouse.y<=585){
      gameController.mouse.hover=true;
      pauseOption1=false;
      if (pp.lb && !pressed){
        pressed=true;
      }
      else if (!pp.lb && pressed){
        pressed=false;
        gameController.gameState=MENU;
        pauseOption1=true;
        resetMouse(&gameController.mouse);
       
      }
    }
    else{
      gameController.mouse.hover=false;

      pressed=false;
    }
  }


}

