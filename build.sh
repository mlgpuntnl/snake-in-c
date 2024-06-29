#!/bin/sh

gcc -Wall -g -o build/snake \
    /usr/lib/libEGL_mesa.so.0.0.0 \
    /usr/lib/libGLX_mesa.so.0.0.0 \
    /usr/lib/libglfw.so.3 \
    util.c \
    game.c \
    renderer.c \
    snake.c
