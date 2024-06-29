#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "util.h"

struct Coordinate createCoordinate(int x, int y) {
    struct Coordinate cord;
    cord.x = x;
    cord.y = y;
    return cord;
}

int randomNumber(int lowest, int highest) {
    srand(time(0));
    return (rand() % ((highest - lowest) + 1)) + lowest;
}
