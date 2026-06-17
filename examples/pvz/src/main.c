#include "pvz_platform.h"

#include "loop.h"

int main(void) {
    if (lcom_init() != LCOM_OK) {
        return 1;
    }
    int result = game_loop();
    lcom_exit();
    return result;
}
