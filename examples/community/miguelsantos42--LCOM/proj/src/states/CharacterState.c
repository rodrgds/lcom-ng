#include "src/include/states/CharacterState.h"
#include "src/include/controllers/video/graphics.h"
#include "src/include/Macros.h"
#include "src/include/controllers/video/projXPM.h"
#include <stdlib.h>
#include <math.h>

static xpm_image_t loaddedWall;
static xpm_image_t loaddedHeroDown;
static xpm_image_t loaddedHeroLeft;
static xpm_image_t loaddedHeroRight;
static xpm_image_t loaddedHeroFinal;
static xpm_image_t loaddedLife;
static xpm_image_t loaddedReload;
static xpm_image_t loaddedChest;
static xpm_image_t loaddedMonsterI;
static xpm_image_t loaddedBulletUp;
static xpm_image_t loaddedBulletDown;
static xpm_image_t loaddedBulletLeft;
static xpm_image_t loaddedBulletRight;
static xpm_image_t loaddedMouse;

static xpm_image_t loaddedNumber0;
static xpm_image_t loaddedNumber1;
static xpm_image_t loaddedNumber2;
static xpm_image_t loaddedNumber3;
static xpm_image_t loaddedNumber4;
static xpm_image_t loaddedNumber5;
static xpm_image_t loaddedNumber6;
static xpm_image_t loaddedNumber7;
static xpm_image_t loaddedNumber8;
static xpm_image_t loaddedNumber9;

static xpm_image_t loaddedWordBullet;
static xpm_image_t loaddedWordLife;
static xpm_image_t loaddedWordScore;

static uint8_t* mappedWall = NULL;
static uint8_t* mappedHeroDown = NULL;
static uint8_t* mappedHeroLeft = NULL;
static uint8_t* mappedHeroRight = NULL;
static uint8_t* mappedHeroFinal = NULL;
static uint8_t* mappedLife = NULL;
static uint8_t* mappedReload = NULL;
static uint8_t* mappedChest = NULL;
static uint8_t* mappedMonsterI = NULL;
static uint8_t* mappedBulletUp = NULL;
static uint8_t* mappedBulletDown = NULL;
static uint8_t* mappedBulletLeft = NULL;
static uint8_t* mappedBulletRight = NULL;
static uint8_t* mappedMouse = NULL;

static uint8_t* mappedNumber0 = NULL;
static uint8_t* mappedNumber1 = NULL;
static uint8_t* mappedNumber2 = NULL;
static uint8_t* mappedNumber3 = NULL;
static uint8_t* mappedNumber4 = NULL;
static uint8_t* mappedNumber5 = NULL;
static uint8_t* mappedNumber6 = NULL;
static uint8_t* mappedNumber7 = NULL;
static uint8_t* mappedNumber8 = NULL;
static uint8_t* mappedNumber9 = NULL;

static uint8_t* mappedWordBullets = NULL;
static uint8_t* mappedWordLife = NULL;
static uint8_t* mappedWordScore = NULL;

uint8_t bulletCount = 5;
uint8_t lifeCount = 3;
uint8_t score = 0;

bool game_over = false;

CharacterState* characters[10000];
uint16_t characterNum = 0;
CharacterState* playableCharacterState = NULL;
MouseState* mouseState = NULL;

struct CharacterStateData{
    int16_t tLX;
    int16_t tLY;
    int16_t bRX;
    int16_t bRY;
    uint16_t internalCounter;
    uint16_t characterID;
    bool movable;
    enum ScreenDirection direction;
    enum XPMType xpm;
};
typedef struct CharacterStateData CharacterStateData;

static void (destroyCharacterStateData)(CharacterStateData *data){
    if(data) free(data);
}

static void (destroyCharacterState)(CharacterState *state){
    if(!state) return;
    destroyCharacterStateData(state->data);
    free(state);
}

static int16_t (getX)(const CharacterState *state){
    CharacterStateData *data;
    getData(state,data,-1);
    return data->tLX;
}
static int16_t (getY)(const CharacterState *state){
    CharacterStateData *data;
    getData(state,data,-1);
    return data->tLY;
}

static enum ScreenDirection (getDirection)(const CharacterState *state){
    CharacterStateData *data;
    getData(state,data,-1);
    return data->direction;
}

static enum XPMType (getXPMType)(const CharacterState *state) {
    CharacterStateData *data;
    getData(state, data, -1);
    return data->xpm;
}

static void (loadDrawXPM)(xpm_map_t xpm, xpm_image_t* image, uint8_t** map, int16_t x, int16_t y) {
    if(*map == NULL) load_xpm(xpm, image, map);
    draw_xpm(*image, *map, x, y);
}

static void (drawNumber)(uint8_t number, int16_t x, int16_t y) {
    switch (number) {
    case 0:
        loadDrawXPM(XPMMapDigit0, &loaddedNumber0, &mappedNumber0, x, y);
        break;
    case 1:
        loadDrawXPM(XPMMapDigit1, &loaddedNumber1, &mappedNumber1, x, y);
        break;
    case 2:
        loadDrawXPM(XPMMapDigit2, &loaddedNumber2, &mappedNumber2, x, y);
        break;
    case 3:
        loadDrawXPM(XPMMapDigit3, &loaddedNumber3, &mappedNumber3, x, y);
        break;
    case 4:
        loadDrawXPM(XPMMapDigit4, &loaddedNumber4, &mappedNumber4, x, y);
        break;
    case 5:
        loadDrawXPM(XPMMapDigit5, &loaddedNumber5, &mappedNumber5, x, y);
        break;
    case 6:
        loadDrawXPM(XPMMapDigit6, &loaddedNumber6, &mappedNumber6, x, y);
        break;
    case 7:
        loadDrawXPM(XPMMapDigit7, &loaddedNumber7, &mappedNumber7, x, y);
        break;
    case 8:
        loadDrawXPM(XPMMapDigit8, &loaddedNumber8, &mappedNumber8, x, y);
        break;
    case 9:
        loadDrawXPM(XPMMapDigit9, &loaddedNumber9, &mappedNumber9, x, y);
        break;
    default:
        break;
    }
}

static void (drawValue)(uint8_t value, int16_t x, int16_t y) {
    if(value >= 0) {
        drawNumber((value / 10) % 10, x, y);
        drawNumber(value % 10, x + 16, y);
    } else {
        drawNumber(value, x, y);
    }
}

void (draw_information)() {
    loadDrawXPM(XPMMapWordBULLETS, &loaddedWordBullet, &mappedWordBullets, 20, 50);
    drawValue(bulletCount, 120, 50);
    loadDrawXPM(XPMMapWordLIFES, &loaddedWordLife, &mappedWordLife, 20, 80);
    drawValue(lifeCount, 120, 80);
    loadDrawXPM(XPMMapWordSCORE, &loaddedWordScore, &mappedWordScore, 20, 110);
    drawValue(score, 120, 110);
}

static void (drawWall)(const CharacterState *state){
    CharacterStateData *data;
    getVoidData(state,data);
    loadDrawXPM(XPMMapWall, &loaddedWall, &mappedWall, data->tLX, data->tLY);
}

static void (drawHero)(const CharacterState *state){
    CharacterStateData *data;
    getVoidData(state,data);
    switch(data->direction){
        case ScreenUp:
            loadDrawXPM(XPMMapHeroFinal, &loaddedHeroFinal, &mappedHeroFinal, data->tLX, data->tLY);
            break;
        case ScreenDown:
            loadDrawXPM(XPMMapHeroDown, &loaddedHeroDown, &mappedHeroDown, data->tLX, data->tLY);
            break;
        case ScreenLeft:
            loadDrawXPM(XPMMapHeroLeft, &loaddedHeroLeft, &mappedHeroLeft, data->tLX, data->tLY);
            break;
        case ScreenRight:
            loadDrawXPM(XPMMapHeroRight, &loaddedHeroRight, &mappedHeroRight, data->tLX, data->tLY);
        default:
            break;
    }  
}

static void (drawLife)(const CharacterState *state){
    CharacterStateData *data;
    getVoidData(state,data);
    loadDrawXPM(XPMMapLife, &loaddedLife, &mappedLife, data->tLX, data->tLY);
}

static void (drawReload)(const CharacterState *state){
    CharacterStateData *data;
    getVoidData(state,data);
    loadDrawXPM(XPMMapReload, &loaddedReload, &mappedReload, data->tLX, data->tLY);
}

static void (drawChest)(const CharacterState *state){
    CharacterStateData *data;
    getVoidData(state,data);
    loadDrawXPM(XPMMapChest, &loaddedChest, &mappedChest, data->tLX, data->tLY);
}

static void (drawMonsterI)(const CharacterState *state){
    CharacterStateData *data;
    getVoidData(state,data);
    loadDrawXPM(XPMMapMonsterI, &loaddedMonsterI, &mappedMonsterI, data->tLX, data->tLY);
}

static void (drawBullet)(const CharacterState *state){
    CharacterStateData *data;
    getVoidData(state,data);
    switch(data->direction){
        case ScreenUp:
            loadDrawXPM(XPMMapBulletUP, &loaddedBulletUp, &mappedBulletUp, data->tLX, data->tLY);
            break;
        case ScreenDown:
            loadDrawXPM(XPMMapBulletDOWN, &loaddedBulletDown, &mappedBulletDown, data->tLX, data->tLY);
            break;
        case ScreenLeft:
            loadDrawXPM(XPMMapBulletLEFT, &loaddedBulletLeft, &mappedBulletLeft, data->tLX, data->tLY);
            break;
        case ScreenRight:
            loadDrawXPM(XPMMapBulletRIGHT, &loaddedBulletRight, &mappedBulletRight, data->tLX, data->tLY);
            break;
        default:
            break;
    }  
}

static void (deleteCharacter)(uint16_t index) {
    if(index >= characterNum) return;
    characters[index]->destroy(characters[index]);
    characters[index] = NULL;

    for(uint16_t i = index; i < characterNum - 1; i++){
        characters[i] = characters[i + 1];
        CharacterStateData* data;
        getVoidData(characters[i], data);
        data->characterID = i;
    }
    characters[characterNum - 1] = NULL;
    characterNum--;
}

static bool (collideBullet)(CharacterState *bullet, uint16_t charIndex){
    CharacterStateData *charData;
    getData(characters[charIndex], charData, false);
    CharacterStateData *bulletData;
    getData(bullet, bulletData, false);
    enum XPMType charXPM = charData->xpm;

    if(bulletData->xpm == XPMBullet){
        if(charXPM == XPMMonsterI || charXPM == XPMBullet) {

            if(charXPM == XPMMonsterI) {
                score += 1;
            }

            deleteCharacter(charIndex);
            deleteCharacter(bulletData->characterID);
        }
        else if(charXPM == XPMWall) deleteCharacter(bulletData->characterID);
        else return false;
    }else if(bulletData->xpm == XPMMonsterI){
        if(charXPM == XPMBullet){
            deleteCharacter(charIndex);
            deleteCharacter(bulletData->characterID);
        }
        else return false;
    }
    else return false;
    return true;
}

static bool (collideCollectable)(CharacterState *hero, uint16_t charIndex){
    CharacterStateData *heroData;
    getData(hero, heroData, false);
    CharacterStateData *charData;
    getData(characters[charIndex], charData, false);

    switch (charData->xpm){
        case XPMReload:
            if(heroData->xpm != XPMHero) return true;
            bulletCount += 5;
            break;
        case XPMLife:
            if(heroData->xpm != XPMHero) return true;
            lifeCount = 3;
            break;
        case XPMMonsterI:
            if(heroData->xpm != XPMHero) return true;
            if(--lifeCount <= 0) {
                game_over = true;
            }
        case XPMChest:
            if(heroData->xpm != XPMHero) return true;
            if(score >= 2) {
                game_over = true;
            } else {
                return false;
            }
            break;
        default:
            return false;
    }
    deleteCharacter(charIndex);
    return true;
}

static bool (checkCharacters)(CharacterState *state, const enum ScreenDirection direction) {
    if(state == NULL) return false;
    CharacterStateData *movingData;
    getData(state, movingData, false);
    int16_t new_tLX = movingData->tLX;
    int16_t new_tLY = movingData->tLY;
    int16_t new_bRX = movingData->bRX;
    int16_t new_bRY = movingData->bRY;
    switch(direction) {
        case ScreenUp:
            new_tLY -= 16;
            new_bRY -= 16;
            break;
        case ScreenDown:
            new_tLY += 16;
            new_bRY += 16;
            break;
        case ScreenLeft:
            new_tLX -= 16;
            new_bRX -= 16;
            break;
        case ScreenRight:
            new_tLX += 16;
            new_bRX += 16;
            break;
        default:
            break;
    }
    for(uint16_t i = 0; i < characterNum; i++){
        CharacterStateData *otherData;
        getData(characters[i], otherData, false);
        if(characters[i] == state) continue;

        if (!(new_bRX <= otherData->tLX || new_tLX >= otherData->bRX ||
              new_bRY <= otherData->tLY || new_tLY >= otherData->bRY)) {
            if(collideBullet(state, i)) return true;
            if(collideCollectable(state, i)) return false;
            return true; 
        }
    }
    if (playableCharacterState && playableCharacterState != state) {
        CharacterStateData *playerData;
        getData(playableCharacterState, playerData, false);
        if (!(new_bRX <= playerData->tLX || new_tLX >= playerData->bRX ||
              new_bRY <= playerData->tLY || new_tLY >= playerData->bRY)) {
            
            return true;
        }
    }
    return false;
}

static void (applyDelta)(CharacterStateData *data, int16_t deltaX, int16_t deltaY){
    data->tLX += deltaX;
    data->tLY += deltaY;
    data->bRX += deltaX;
    data->bRY += deltaY;
}

static void (centerHero)(){
    if(!playableCharacterState) return;
    const int16_t targetX = 1024/2 - 8;
    const int16_t targetY = 768/2 - 8;
    CharacterStateData *playerData;
    getVoidData(playableCharacterState, playerData);
    int16_t deltaX = targetX - playerData->tLX;
    int16_t deltaY = targetY - playerData->tLY;
    applyDelta(playerData, deltaX, deltaY);
    for(uint16_t i = 0; i < characterNum; i++){
        CharacterStateData *data;
        getVoidData(characters[i], data);
        applyDelta(data, deltaX, deltaY);
    }
}

static enum ScreenDirection getRandomDirection() {
    return (enum ScreenDirection)(rand() % 5);
}

static void (moveStatic)(CharacterState *state, const enum ScreenDirection direction){}

static void (moveMovable)(CharacterState *state, const enum ScreenDirection direction){
    CharacterStateData *data;
    getVoidData(state,data);
    if(!data->movable) return;
    data->movable = data->xpm == XPMHero;
    enum ScreenDirection actualDirection = direction;

    if(data->xpm == XPMMonsterI) {
        if(++data->internalCounter % 15 != 0) return;
        data->internalCounter = 0;
        actualDirection = getRandomDirection();
    }

    data->direction = actualDirection;
    CHECK_VOID(checkCharacters(state,actualDirection));
    switch (actualDirection)
    {
        case ScreenUp:
            if(data->tLY>=(int16_t)0x8010) applyDelta(data, 0, -16);
            else applyDelta(data, 0, (int16_t)0x8000 - data->tLY);
            break;
        case ScreenDown:
            if(data->bRY <= (int16_t)0x7FEF) applyDelta(data, 0, 16);
            else applyDelta(data, 0, (int16_t)0x7FFF - data->bRY);
            break;
        case ScreenLeft:
            if(data->tLX>=(int16_t)0x8010) applyDelta(data, -16,0);
            else applyDelta(data, (int16_t)0x8000 - data->tLX, 0);
            break;
        case ScreenRight:
            if(data->bRX<=(int16_t)0x7FEF) applyDelta(data, 16,0);
            else applyDelta(data, (int16_t)0x7FFF - data->bRX, 0);
            break;
        default:
            break;
    }
    if(data->xpm == XPMHero) centerHero();
}

static void (moveBullet)(CharacterState *state, const enum ScreenDirection direction){
    CharacterStateData *data;
    getVoidData(state, data);
    if(++data->internalCounter % 5 != 0) return;
    data->internalCounter = 0;
    moveMovable(state, data->direction);
}

void (shootBullet)(enum ScreenDirection direction) {
    if(bulletCount == 0) return;
    bulletCount--;

    CharacterStateData *playableCharacterData;
    getVoidData(playableCharacterState, playableCharacterData);
    
    int16_t bulletX = playableCharacterData->tLX;
    int16_t bulletY = playableCharacterData->tLY;

    CharacterState *bullet = characterStateConstructor(bulletX, bulletY, XPMBullet);
    CharacterStateData *bulletData;
    
    getVoidData(bullet, bulletData);
    bulletData->direction = direction;
    CHECK_VOID(checkCharacters(bullet, direction));

    switch (direction) {
        case ScreenUp:
            bulletY -= 16;
            break;
        case ScreenDown:
            bulletY += 16;
            break;
        case ScreenLeft:
            bulletX -= 16;
            break;  
        case ScreenRight:
            bulletX += 16;
            break;
        default:
            return;
    }

    bulletData->tLX = bulletX;
    bulletData->tLY = bulletY;
    bulletData->bRX = bulletX + 16;
    bulletData->bRY = bulletY + 16;

    characters[characterNum++] = bullet;
}

static void (linkXPM)(CharacterState *state, CharacterStateData *data, enum XPMType xpm){
    data->xpm = xpm;
    state->data = data;
    switch(xpm){
        case XPMWall:
            state->draw = drawWall;
            state->move = moveStatic;
            break;
        case XPMHero:
            state->draw = drawHero;
            state->move = moveMovable;
            break;        
        case XPMLife:
            state->draw = drawLife;
            state->move = moveStatic;
            break;
        case XPMReload:
            state->draw = drawReload;
            state->move = moveStatic;
            break;
        case XPMChest:
            state->draw = drawChest;
            state->move = moveStatic;
            break;
        case XPMMonsterI:
            state->draw = drawMonsterI;
            state->move = moveMovable;
            break;
        case XPMBullet:
            state->draw = drawBullet;
            state->move = moveBullet;
            break;
        default:
            break;
    }
}

void (allowMovement)(CharacterState* state){
    CharacterStateData *data;
    getVoidData(state,data);
    data->movable = true;
}

CharacterState *(characterStateConstructor)(int16_t x, int16_t y, enum XPMType xpm){
    CharacterStateData *data = malloc(sizeof(CharacterStateData));
    if(!data) return NULL;
    CharacterState *state = malloc(sizeof(CharacterState));
    if(!state){
        destroyCharacterStateData(data);
        return NULL;
    }
    data->direction = ScreenRight;
    data->tLX = x;
    data->tLY = y;
    data->bRX = x + 16;
    data->bRY = y + 16;
    data->characterID = characterNum;
    data->movable = true;
    data->internalCounter = 0;
    state->getX = getX;
    state->getY = getY;
    state->getDirection = getDirection;
    state->getXPMType = getXPMType;
    state->destroy = destroyCharacterState;
    linkXPM(state, data, xpm);
    return state;
}


struct MouseStateData{
    uint16_t x;
    uint16_t y;
};

typedef struct MouseStateData MouseStateData;

static void (setPosition)(struct MouseState* state, uint16_t x, uint16_t y){
    MouseStateData* data;
    getVoidData(state,data);
    data->x = x;
    data->y = y;
}
static void (drawMouse)(struct MouseState* state){
    MouseStateData* data;
    getVoidData(state,data);
    loadDrawXPM(XPMMapMouse, &loaddedMouse, &mappedMouse, data->x, data->y);
}
static void (deleteMouse)(struct MouseState* state){
    MouseStateData* data;
    getVoidData(state,data);
    free(data);
    free(state);
}

static uint16_t (getMouseX)(struct MouseState* state) {
    MouseStateData* data;
    getData(state, data, 0);  
    return data->x;
}

static uint16_t (getMouseY)(struct MouseState* state) {
    MouseStateData* data;
    getData(state, data, 0); 
    return data->y;
}

MouseState *(mouseStateConstructor)(){
    MouseStateData *data = malloc(sizeof(MouseStateData));
    if(!data) return NULL;
    MouseState *state = malloc(sizeof(MouseState));
    if(!state){
        free(data);
        return NULL;
    }
    data->x = (1024/2)-8;
    data->y = (768/2)-8;
    state->data = data;
    state->setPosition = setPosition;
    state->draw = drawMouse;
    state->delete = deleteMouse;
    
    state->getMouseX = getMouseX;
    state->getMouseY = getMouseY;

    return state;
}

int (draw_map)(char* map){
  FILE *fptr;

  fptr = fopen(map, "r");

  char _str[110];

  printf("Starting to read file...\n");

  
  uint16_t y = 0;

  if(fptr != NULL) {
    while(fgets(_str, 110, fptr)) {
      uint16_t x = 0;
      for(size_t i = 0; i < 100; i++){
        // # H M L R C
        switch(_str[i]) {
          case '#':
            characters[characterNum++] = characterStateConstructor(x,y,XPMWall);
            break;
          case 'H':
            playableCharacterState = characterStateConstructor(x, y, XPMHero);
            break;
          case 'L':
            characters[characterNum++] = characterStateConstructor(x, y, XPMLife);
            break;
          case 'R':
            characters[characterNum++] = characterStateConstructor(x, y, XPMReload);
            break;
          case 'C':
            characters[characterNum++] = characterStateConstructor(x, y, XPMChest);
            break;
          case 'M':
            characters[characterNum++] = characterStateConstructor(x, y, XPMMonsterI);
            break;
          default:
            break;
        }
        x += 16;
      }
      y += 16;
      memset(_str, 0, sizeof _str);
    }
    fclose(fptr);
  } else {
    printf("Ficheiro não encontrado! %s\n", map);
  }
  if(!playableCharacterState) return 1;
  centerHero();
  mouseState = mouseStateConstructor();

  printf("Ending reading file\n");
  return 0;
}

void (destroyMap)(){
    playableCharacterState->destroy(playableCharacterState);
    for(uint16_t i = 0; i < characterNum; i++) {
        characters[i]->destroy(characters[i]);
        characters[i] = NULL;
    }

    characterNum = 0;
    playableCharacterState = NULL;
    
}

int (reloadMap)(char* path) {
    bulletCount = 5;
    lifeCount = 3;
    score = 0;
    game_over = false;

    destroyMap();

    return draw_map(path);
}
