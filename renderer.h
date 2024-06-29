#include "util.h"

#ifndef RENDERER_H_INCLUDED
#define RENDERER_H_INCLUDED

struct RenderGameState {
    int length;
    struct Coordinate *coordinates;
};

void initRenderer(int rows, int columns);

void draw(int screenWidth, int screenHeight, struct RenderGameState* gameState);

#endif