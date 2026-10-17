#include <minix/drivers.h>
#include <stdio.h>
#include <stdlib.h>

#include "scene.h"

int main(int argc, char **argv) {
    sef_startup();

    scene_init();

    return 0;
}
