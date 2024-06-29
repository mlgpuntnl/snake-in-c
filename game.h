
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#ifndef NEW_GAME_H_INCLUDED
#define NEW_GAME_H_INCLUDED

void initGame(int rows, int columns, GLFWwindow* window);
struct RenderGameState* getGameState();
void updateGameLoop();
void handleKeyPress(int key);
void destroyGame();

#endif
