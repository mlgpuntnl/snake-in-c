#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include "util.h"

struct Coordinate createCoordinate(int x, int y) {
    struct Coordinate cord;
    cord.x = x;
    cord.y = y;
    return cord;
}

void sleepSeconds(double seconds) {
    int fullSeconds = 0;
    int nanoSeconds = 0;
    while (seconds > 1.0f) {
        fullSeconds++;
        seconds--;
    }
    nanoSeconds = (int) (seconds * 1000000000);
    struct timespec remaining, request = { fullSeconds, nanoSeconds };
    int error = nanosleep(&request, &remaining);
    if (error != 0 && errno == EINTR) {
        printf("Sleep interupted\n");
    }
}

int randomNumber(int lowest, int highest) {
    srand(time(0));
    return (rand() % ((highest - lowest) + 1)) + lowest;
}
