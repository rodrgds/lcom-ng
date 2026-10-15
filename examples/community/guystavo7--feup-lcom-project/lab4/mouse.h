#ifndef MOUSE
#define MOUSE

#include <lcom/lcf.h>
#include <stdbool.h>

enum selectedByte {
    BYTE_1,
    BYTE_2,
    BYTE_3,
    READY
};

static uint8_t byte1 = 0, byte2 = 0, byte3 = 0;
static enum selectedByte currentByte = BYTE_1;

int mouseSubscribe(int* hook_id);
int mouseUnsubscribe(int* hook_id);

int getStatusAndErrors(uint8_t* status, uint8_t* errors);
bool canRead();
bool canWrite();

int enableDataReport();
int disableDataReport();

void cleanBytes();

uint8_t getByte1();
uint8_t getByte2();
uint8_t getByte3();

bool pressedRightButton();
bool pressedLeftButton();
bool pressedMiddleButton();

int16_t getDeltaX();
int16_t getDeltaY();
bool isOverflowingX();
bool isOverflowingY();
bool isReady();

#endif
