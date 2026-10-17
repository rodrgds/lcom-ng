#include "src/include/controllers/kbc/KBCInput.h"
#include "../i8042.h"
#include <stdlib.h>
#include "src/include/Macros.h"

struct KBCData{
    bool aux;
    void *rawInput;
};

typedef struct KBCData KBCData;

static void (destroyData)(KBCData *data){
    if(!data) return;
    if(data->aux){
        MouseInput *rawInput = data->rawInput;
        if(rawInput) rawInput->destroy(rawInput);
    }else{
        KeyboardInput *rawInput = data->rawInput;
        if(rawInput) rawInput->destroy(rawInput);
    }
    free(data);
}

static void (destroy)(KBCInput *input){
    if(!input) return;
    KBCData *data = input->data;
    if(data) destroyData(data);
    free(input);
}

static bool (isAux)(const KBCInput *input){
    KBCData *data;
    getData(input,data,false);
    return data->aux;
}

static const KeyboardInput *(getKeyboardInput)(const struct KBCInput *input){
    KBCData *data;
    getData(input,data,NULL);
    if(data->aux) return NULL;
    KeyboardInput *keyboardInput = data->rawInput;
    return keyboardInput;
}

static const MouseInput *(getMouseInput)(const struct KBCInput *input){
    KBCData *data;
    getData(input,data,NULL);
    if(!data->aux) return NULL;
    MouseInput *mouseInput = data->rawInput;
    return mouseInput;
}

static KBCData *(dataConstructor)(const void *rawInput, const bool aux){
    if(!rawInput) return NULL;
    KBCData *data = malloc(sizeof(KBCData));
    if(!data) return NULL;
    data->aux = aux;
    if(aux) data->rawInput = mouseCopyConstructor(rawInput);
    else data->rawInput = keyboardCopyConstructor(rawInput);
    if(!data->rawInput){
        free(data);
        return NULL;
    }
    return data;
}

static KBCInput *(linkInput)(const void *rawInput, const bool aux){
    if(!rawInput) return NULL;
    KBCData *data = dataConstructor(rawInput, aux);
    if(!data) return NULL;
    KBCInput *processedInput = malloc(sizeof(KBCInput));
    if(!processedInput) {
        destroyData(data);
        return NULL;
    }
    processedInput->data = data;
    processedInput->isAux = isAux;
    processedInput->getKeyboardInput = getKeyboardInput;
    processedInput->getMouseInput = getMouseInput;
    processedInput->destroy = destroy;
    return processedInput;
}

KBCInput *(kBCKeyboardConstructor)(const KeyboardInput *input){
    return linkInput(input, false);
}

KBCInput *(kBCMouseConstructor)(const MouseInput *input){
    return linkInput(input, true);
}

KBCInput *(kBCCopyConstructor)(const KBCInput *originalInput){
    KBCData *orginalData;
    getData(originalInput,orginalData,NULL);
    if(!orginalData->rawInput) return NULL;
    void *copyRawInput = NULL;
    if(orginalData->aux){
        MouseInput *originalRawInput = orginalData->rawInput;
        copyRawInput = mouseCopyConstructor(originalRawInput);
    }else{
        KeyboardInput *originalRawInput = orginalData->rawInput;
        copyRawInput = keyboardCopyConstructor(originalRawInput);
    }
    if(!copyRawInput) return NULL;
    return linkInput(copyRawInput, orginalData->aux);
}
