#include "entity.h"


void (createLogs(struct Entity * ent)){
  int nLogs = 6;
  for (int i=0;i<nLogs;i++){
    Log tempLog;
    initLog(&tempLog,i);
    ent->logs[i]=tempLog;
  }
  ent->logNr=nLogs;
}

void (createTurtle(struct Entity * ent)){
  int nturtles = 2;
  for (int i=0;i<nturtles;i++){
    Turtle tempTurtle;
    initTurtle(&tempTurtle,i);
    ent->turtles[i]=tempTurtle;
  }
  ent->turtleNr=nturtles;
}

void createVehicle(struct Entity * ent){
  int nVehicles = 6;
  for (int i=0;i<nVehicles;i++){
    Vehicle tempVehicle;
    initVehicle(&tempVehicle,i);
    ent->vehicles[i]=tempVehicle;
  }
  ent->vehicleNr=nVehicles;
}


void initEntity(struct Entity * ent, Type type){
  switch (type)
  {
  case FROG:
    {
    Frog tempFrog;
    initFrog(&tempFrog);
    ent->frog=tempFrog;
    ent->type=FROG;
    break;
    }
  case LOG:
    createLogs(ent);
    ent->type=LOG;

    break;
  case TURTLE:
    createTurtle(ent);
    ent->type=TURTLE;
    ent->UnderWaterTurtles=false;

    break;
  case VEHICLE:
    createVehicle(ent);
    ent->type=VEHICLE;
    break;  
  default:
    break;
  }
}



void drawFrog(struct Entity ent, Direction direction){
    int x=ent.frog.x;
    int y=ent.frog.y;
    int img_width=ent.frog.img[ent.frog.animationIndex].width;
    int img_height=ent.frog.img[ent.frog.animationIndex].height;
    uint32_t* map= (uint32_t*) ent.frog.img[ent.frog.animationIndex].bytes;
    switch (direction)
    { 
    case UP:
      draw(x,y,img_width,img_height,map);
      break;
    case DOWN:
      map += ent.frog.img[0].height * ent.frog.img[0].width;
      drawDown(x,y,img_width,img_height,map);
      break;
    case RIGHT:
      drawRight(x,y,img_width,img_height,map);
      break;
    case LEFT:
      drawLeft(x,y,img_width,img_height,map);
      break; 
    default:
      break;
    }
    
    
}

int (drawEntity(struct Entity ent)){
  int x,y,img_width,img_height;
  uint32_t *map=NULL;
  switch (ent.type)
  {
  case LOG:
    for (int i=0;i<ent.logNr;i++){
      x=ent.logs[i].x;
      y=ent.logs[i].y;
      img_width=ent.logs[i].img.width;
      img_height=ent.logs[i].img.height;
      map= (uint32_t*) ent.logs[i].img.bytes;
      draw(x,y,img_width,img_height,map);
    }
    break;
  case TURTLE:
    for (int i=0;i<ent.turtleNr;i++){
      x=ent.turtles[i].x;
      y=ent.turtles[i].y;
      if (ent.turtles[i].underwater){
        if (ent.turtles[i].dayTurtle){
          img_width=ent.turtles[i].img2.width;
          img_height=ent.turtles[i].img2.height;
          map= (uint32_t*) ent.turtles[i].img2.bytes;
        }
        else{
          img_width=ent.turtles[i].img3.width;
          img_height=ent.turtles[i].img3.height;
          map= (uint32_t*) ent.turtles[i].img3.bytes;
        }
        
      }
      else{
        img_width=ent.turtles[i].img1.width;
        img_height=ent.turtles[i].img1.height;
        map= (uint32_t*) ent.turtles[i].img1.bytes;
      }
      draw(x,y,img_width,img_height,map);
    }
    break;
  case VEHICLE:
    for (int i=0;i<ent.vehicleNr;i++){
      x=ent.vehicles[i].x;
      y=ent.vehicles[i].y;
      if (!ent.vehicles[i].car){
        img_width=ent.vehicles[i].img2.width;
      img_height=ent.vehicles[i].img2.height;
      map= (uint32_t*) ent.vehicles[i].img2.bytes;
      }
      else{
        img_width=ent.vehicles[i].img.width;
        img_height=ent.vehicles[i].img.height;
        map= (uint32_t*) ent.vehicles[i].img.bytes;
      }
      draw(x,y,img_width,img_height,map);
    }
    break;     
  default:
    break;
  }
  return 0;
}



void (switchTurtlesDay(struct Entity * ent,bool day)){
  if (!day){
    for (int i=0;i<ent->turtleNr;i++){
      ent->turtles[i].dayTurtle=false;
    }
  }
  else{
    for (int i=0;i<ent->turtleNr;i++){
      ent->turtles[i].dayTurtle=true;
    }
  }
}

void (switchTurtlesUnderwater(struct Entity * ent)){
  if (ent->UnderWaterTurtles){
    for (int i=0;i<ent->turtleNr;i++){
      ent->turtles[i].underwater=false;
    }
    ent->UnderWaterTurtles=false;
  }
  else{
    for (int i=0;i<ent->turtleNr;i++){
      ent->turtles[i].underwater=true;
    }
    ent->UnderWaterTurtles=true;
  }
}

void (moveLogs(struct Entity * ent)){
  for (int i=0;i<ent->logNr;i++){
    if (ent->logs[i].x+ ent->logs[i].speed >xRes){
      ent->logs[i].x-=(xRes-ent->logs[i].speed);
    }
    else{
      ent->logs[i].x+=ent->logs[i].speed;
    }
    
    
  }
}

void (moveTurtles(struct Entity * ent)){
  for (int i=0;i<ent->turtleNr;i++){
    if (ent->turtles[i].x- ent->turtles[i].speed <0){
      ent->turtles[i].x+=(xRes-ent->turtles[i].speed);
    }
    else{
      ent->turtles[i].x-=ent->turtles[i].speed;
    }
    
    
  }
}


void (moveVehicles(struct Entity * ent)){
  for (int i=0;i<ent->vehicleNr;i++){
    if (!ent->vehicles[i].car){
      if (ent->vehicles[i].x- ent->vehicles[i].speed <0){
        ent->vehicles[i].x+=(xRes-ent->vehicles[i].speed);
      }
      else{
        ent->vehicles[i].x-=ent->vehicles[i].speed;
      }
    }
    else{
      if (ent->vehicles[i].x+ent->vehicles[i].speed>xRes){
        ent->vehicles[i].x-=(xRes-ent->vehicles[i].speed);
      }
      else{
        ent->vehicles[i].x+=ent->vehicles[i].speed;
      }
    }
    
  }
}

bool (moveFrog(struct Entity * ent,Direction dir)){
  switch (dir)
  {
  case UP:
    if (ent->frog.y-25>=0){
      ent->frog.y-=25;
      return true;
    }
    break;
  case LEFT:
    if (ent->frog.x-25>=0){
      ent->frog.x-=25;
      return true;
    }
    break;
  case RIGHT:
    if (ent->frog.x+25<=xRes-ent->frog.img[0].width){
      ent->frog.x+=25;
      return true;
    }
    break;
  case DOWN:
    if (ent->frog.y+25<=yRes-ent->frog.img[0].height && ent->frog.y+ent->frog.img[0].height+25<=844){
      ent->frog.y+=25;
      return true;
    }   
    break;      
  default:
    break;
  }
  return false;
}


bool frogOnLog(struct Entity entFrog, struct Entity entLogs,int * logIndex){
  int logGaps = 10;
  for (int i=0;i<entLogs.logNr;i++){
    if (entLogs.logs[i].x+entLogs.logs[i].img.width>xRes){
      int overflow = (entLogs.logs[i].x+entLogs.logs[i].img.width) - xRes;
      if (entLogs.logs[i].y-logGaps<=entFrog.frog.y + (entFrog.frog.img[0].height/2) && entFrog.frog.y + (entFrog.frog.img[0].height/2)<=entLogs.logs[i].y+entLogs.logs[i].img.height+logGaps){
        if ((entLogs.logs[i].x<=entFrog.frog.x + (entFrog.frog.img[0].width/2) && entFrog.frog.x + (entFrog.frog.img[0].width/2)<=entLogs.logs[i].x+entLogs.logs[i].img.width) || (0<=entFrog.frog.x + (entFrog.frog.img[0].width/2) && entFrog.frog.x + (entFrog.frog.img[0].width/2)<=overflow)){
          *logIndex=i;
          return true;
        }
      }
    }
    else{
      if (entLogs.logs[i].x<=entFrog.frog.x + (entFrog.frog.img[0].width/2) && entFrog.frog.x + (entFrog.frog.img[0].width/2)<=entLogs.logs[i].x+entLogs.logs[i].img.width){
      if (entLogs.logs[i].y-logGaps<=entFrog.frog.y + (entFrog.frog.img[0].height/2) && entFrog.frog.y + (entFrog.frog.img[0].height/2)<=entLogs.logs[i].y+entLogs.logs[i].img.height+logGaps){
        *logIndex=i;
        return true;
      }
    }
    }
    
  }
  return false;
}

bool frogOnTurtle(struct Entity entFrog, struct Entity entTurtles,int * turtleIndex){
  int turtleGaps = 0;
  for (int i=0;i<entTurtles.turtleNr;i++){
    if (entTurtles.turtles[i].x+entTurtles.turtles[i].img1.width>xRes){
      int overflow = (entTurtles.turtles[i].x+entTurtles.turtles[i].img1.width) - xRes;
      if (entTurtles.turtles[i].y-turtleGaps<=entFrog.frog.y + (entFrog.frog.img[0].height/2) && entFrog.frog.y + (entFrog.frog.img[0].height/2)<=entTurtles.turtles[i].y+entTurtles.turtles[i].img1.height+turtleGaps){
        if ((entTurtles.turtles[i].x<=entFrog.frog.x + (entFrog.frog.img[0].width/2) && entFrog.frog.x + (entFrog.frog.img[0].width/2)<=entTurtles.turtles[i].x+entTurtles.turtles[i].img1.width) || (0<=entFrog.frog.x + (entFrog.frog.img[0].width/2) && entFrog.frog.x + (entFrog.frog.img[0].width/2)<=overflow)){
          *turtleIndex=i;
          return true;
        }
      }
    }
    else{
      if (entTurtles.turtles[i].x<=entFrog.frog.x + (entFrog.frog.img[0].width/2) && entFrog.frog.x + (entFrog.frog.img[0].width/2)<=entTurtles.turtles[i].x+entTurtles.turtles[i].img1.width){
      if (entTurtles.turtles[i].y-turtleGaps<=entFrog.frog.y + (entFrog.frog.img[0].height/2) && entFrog.frog.y + (entFrog.frog.img[0].height/2)<=entTurtles.turtles[i].y+entTurtles.turtles[i].img1.height+turtleGaps){
        *turtleIndex=i;
        return true;
      }
    }
    }
    
  }
  return false;
}


bool frogOnVehicle(struct Entity entFrog, struct Entity entVehicle){
  int vehicleGaps = 10;
  for (int i=0;i<entVehicle.vehicleNr;i++){
    if (entVehicle.vehicles[i].car){
      if (entVehicle.vehicles[i].x+entVehicle.vehicles[i].img.width>xRes){
      int overflow = (entVehicle.vehicles[i].x+entVehicle.vehicles[i].img.width) - xRes;
      if (entVehicle.vehicles[i].y-vehicleGaps<=entFrog.frog.y + (entFrog.frog.img[0].height/2) && entFrog.frog.y + (entFrog.frog.img[0].height/2)<=entVehicle.vehicles[i].y+entVehicle.vehicles[i].img.height+vehicleGaps){
        if ((entVehicle.vehicles[i].x<=entFrog.frog.x + (entFrog.frog.img[0].width/2) && entFrog.frog.x + (entFrog.frog.img[0].width/2)<=entVehicle.vehicles[i].x+entVehicle.vehicles[i].img.width) || (0<=entFrog.frog.x + (entFrog.frog.img[0].width/2) && entFrog.frog.x + (entFrog.frog.img[0].width/2)<=overflow)){
          return true;
          }
        }
      }
      else{
      if (entVehicle.vehicles[i].x<=entFrog.frog.x + (entFrog.frog.img[0].width/2) && entFrog.frog.x + (entFrog.frog.img[0].width/2)<=entVehicle.vehicles[i].x+entVehicle.vehicles[i].img.width){
      if (entVehicle.vehicles[i].y-vehicleGaps<=entFrog.frog.y + (entFrog.frog.img[0].height/2) && entFrog.frog.y + (entFrog.frog.img[0].height/2)<=entVehicle.vehicles[i].y+entVehicle.vehicles[i].img.height+vehicleGaps){
        return true;
        }
      }
    }
    }
    else{
      if (entVehicle.vehicles[i].x+entVehicle.vehicles[i].img2.width>xRes){
      int overflow = (entVehicle.vehicles[i].x+entVehicle.vehicles[i].img2.width) - xRes;
      if (entVehicle.vehicles[i].y-vehicleGaps<=entFrog.frog.y + (entFrog.frog.img[0].height/2) && entFrog.frog.y + (entFrog.frog.img[0].height/2)<=entVehicle.vehicles[i].y+entVehicle.vehicles[i].img2.height+vehicleGaps){
        if ((entVehicle.vehicles[i].x<=entFrog.frog.x + (entFrog.frog.img[0].width/2) && entFrog.frog.x + (entFrog.frog.img[0].width/2)<=entVehicle.vehicles[i].x+entVehicle.vehicles[i].img2.width) || (0<=entFrog.frog.x + (entFrog.frog.img[0].width/2) && entFrog.frog.x + (entFrog.frog.img[0].width/2)<=overflow)){
          return true;
          }
        }
      }
      else{
      if (entVehicle.vehicles[i].x<=entFrog.frog.x + (entFrog.frog.img[0].width/2) && entFrog.frog.x + (entFrog.frog.img[0].width/2)<=entVehicle.vehicles[i].x+entVehicle.vehicles[i].img2.width){
      if (entVehicle.vehicles[i].y-vehicleGaps<=entFrog.frog.y + (entFrog.frog.img[0].height/2) && entFrog.frog.y + (entFrog.frog.img[0].height/2)<=entVehicle.vehicles[i].y+entVehicle.vehicles[i].img2.height+vehicleGaps){
        return true;
        }
      }
    }
    }
    
  }
  return false;
}
