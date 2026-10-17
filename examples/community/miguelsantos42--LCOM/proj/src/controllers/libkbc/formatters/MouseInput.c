#include <stdlib.h>
#include "src/include/controllers/kbc/MouseInput.h"
#include "../i8042.h"
#include "src/include/Macros.h"

struct MouseData{
    int16_t x;
    int16_t y;
    bool left;
    bool middle;
    bool right;
};

typedef struct MouseData MouseData;

static void (destroyData)(MouseData *data){
    if(data) free(data);
}

static void (destroy)(MouseInput *input){
    if(!input) return;
    MouseData *data = input->data;
    if(data) destroyData(data);
    free(input);
}

static bool (leftPressed)(const MouseInput *input){
    MouseData *data;
    getData(input,data,false);
    return data->left;
}

static bool (middlePressed)(const  MouseInput *input){
    MouseData *data;
    getData(input,data,false);
    return data->middle;
}

static bool (rightPressed)(const  MouseInput *input){
    MouseData *data;
    getData(input,data,false);
    return data->right;
}

static int16_t (getY)(const  MouseInput *input){
    MouseData *data;
    getData(input,data,0);
    return data->y;
}

static int16_t (getX)(const  MouseInput *input){
    MouseData *data;
    getData(input,data,0);
    return data->x;
}

static MouseData *(dataConstructor)(int16_t x, int16_t y, bool left, bool middle, bool right){
    MouseData *data = malloc(sizeof(MouseData));
    if(!data) return NULL;
    data->left = left;
    data->middle = middle;
    data->right = right;
    data->x = x;
    data->y = y;
    return data;
}

static MouseInput *(linkInput)(int16_t x, int16_t y, bool left, bool middle, bool right){
    MouseData *data = dataConstructor(x,y,left,middle,right);
    if(!data) return NULL;
    MouseInput *input = malloc(sizeof(MouseInput));
    if(!input){
        destroyData(data);
        return NULL;
    }
    input->data = data;
    input->getX = getX;
    input->getY = getY;
    input->leftPressed = leftPressed;
    input->middlePressed = middlePressed;
    input->rightPressed = rightPressed;
    input->destroy = destroy;
    return input;
}

MouseInput *(mouseConstructor)(const struct packet *mousePacket){
    if(!mousePacket) return NULL;
    if(!(mousePacket->bytes[0]&MOUSE_CTRL_BYTE)) return NULL;
    return linkInput(mousePacket->delta_x,mousePacket->delta_y,mousePacket->lb,mousePacket->mb,mousePacket->rb);
}

MouseInput *(mouseCopyConstructor)(const MouseInput *originalInput){
    MouseData *originalData;
    getData(originalInput,originalData,NULL);
    return linkInput(originalData->x, originalData->y, originalData->left, originalData->middle, originalData->right);
}
