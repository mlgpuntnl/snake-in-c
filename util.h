#ifndef UTIL_H_INCLUDED
#define UTIL_H_INCLUDED

struct Coordinate {
    int x;
    int y;
};

struct Coordinate createCoordinate(int x, int y);

void sleepSeconds(double seconds);

int randomNumber(int lowest, int highest);

#endif