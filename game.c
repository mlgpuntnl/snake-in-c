#include <stdio.h>
#include <stdlib.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "util.h"

#define MIN_ROWS 6
#define MIN_COLUMNS 6
#define START_SIZE 3

#define CLOSE_GAME_ON_GAME_OVER 1

enum Direction {
    UP = 0,
    DOWN = 1,
    LEFT = 2,
    RIGHT = 3,
};

struct RenderGameState {
    int length;
    struct Coordinate *coordinates;
} gameState;

typedef struct PlayerPart {
    int x;
    int y;
} PlayerPart;

struct Target {
    int x;
    int y;
};

static struct GameInfo {
    PlayerPart *playerParts;
    int playerLength;
    struct Target target;
    int currentDirection;
} gameInfo;

static int gameRows;
static int gameColumns;

GLFWwindow* gameWindow;

void setGameDimensions(int rows, int columns);
void updatePlayer();
void createPlayerPart(int x, int y);
void createInitialPlayer();
void createNewTarget();
PlayerPart getNewDirection(PlayerPart lastDirection);
int isPlayerSpace(int x, int y);
void initGameState();

void initGame(int rows, int columns, GLFWwindow* window) {
    gameInfo.currentDirection = UP;
    gameWindow = window;
    setGameDimensions(rows, columns);
    createInitialPlayer();
    createNewTarget();
    initGameState();
}

void initGameState() {
    gameState.length = 0;
    gameState.coordinates = malloc(sizeof(struct Coordinate) * (gameRows * gameColumns));
}

void updateGameLoop() {
    updatePlayer();
}

struct RenderGameState* getGameState() {
    gameState.length = gameInfo.playerLength + 1;
    for (int i = 0; i < gameInfo.playerLength; i++) {
        struct Coordinate playerCord = { gameInfo.playerParts[i].x, gameInfo.playerParts[i].y };
        gameState.coordinates[i] = playerCord;
    }
    struct Coordinate targetCord = { gameInfo.target.x, gameInfo.target.y };
    gameState.coordinates[gameInfo.playerLength] = targetCord;
    return &gameState;
}

void destroyGame() {
    free(gameInfo.playerParts);
    free(gameState.coordinates);
}

void setGameDimensions(int rows, int columns) {
    if (rows < MIN_ROWS || columns < MIN_COLUMNS) {
        printf(
            "[ERROR] grid dimensions to small. Need at least: %dx%d, but got: %dx%d\n",
            MIN_COLUMNS, MIN_ROWS, columns, rows
        );
    }
    gameRows = rows;
    gameColumns = columns;
}

void updatePlayer() {
    PlayerPart targetDirection = getNewDirection(
        gameInfo.playerParts[gameInfo.playerLength - 1]
    );

    if (
        targetDirection.x < 0 ||
        targetDirection.y < 0 ||
        targetDirection.x >= gameColumns ||
        targetDirection.y >= gameRows ||
        isPlayerSpace(targetDirection.x, targetDirection.y) == 1
    ) {
        printf("LOSER!\n");
        if (CLOSE_GAME_ON_GAME_OVER == 1) {
            glfwSetWindowShouldClose(gameWindow, GLFW_TRUE);
        }
        return;
    }

    if (targetDirection.x == gameInfo.target.x && targetDirection.y == gameInfo.target.y) {
        createPlayerPart(targetDirection.x, targetDirection.y);
        createNewTarget();
        return;
    }

    for (int i = 0; i < gameInfo.playerLength; i++) {
        if (i == gameInfo.playerLength - 1) {
            gameInfo.playerParts[i] = targetDirection;
        } else {
            gameInfo.playerParts[i] = gameInfo.playerParts[i + 1];
        }
    }
    
}

PlayerPart getNewDirection(PlayerPart lastDirection) {
    PlayerPart targetDirection = lastDirection;
    switch (gameInfo.currentDirection)
    {
    case UP:
        targetDirection.y -= 1;
        break;

    case DOWN:
        targetDirection.y += 1;
        break;

    case LEFT:
        targetDirection.x -= 1;
        break;

    case RIGHT:
        targetDirection.x += 1;
        break;
    }
    return targetDirection;
}

void handleKeyPress(int key) {
    switch (key) {
    case GLFW_KEY_UP:
    case GLFW_KEY_W:
        if (gameInfo.currentDirection != DOWN) {
            gameInfo.currentDirection = UP;
        }
        break;
    
    case GLFW_KEY_DOWN:
    case GLFW_KEY_S:
        if (gameInfo.currentDirection != UP) {
            gameInfo.currentDirection = DOWN;
        }
        break;
    
    case GLFW_KEY_LEFT:
    case GLFW_KEY_A:
        if (gameInfo.currentDirection != RIGHT) {
            gameInfo.currentDirection = LEFT;
        }
        break;
    
    case GLFW_KEY_RIGHT:
    case GLFW_KEY_D:
        if (gameInfo.currentDirection != LEFT) {
            gameInfo.currentDirection = RIGHT;
        }
        break;
    
    default:
        break;
    }
}

void createInitialPlayer() {
    gameInfo.playerParts = malloc(sizeof(PlayerPart) * (gameRows * gameColumns));
    gameInfo.playerLength = 0;
    int playerX = gameColumns / 2;
    int playerY = gameRows / 2;
    for (int i = START_SIZE; i > 0; i--) {
        createPlayerPart(playerX, playerY + (i - 1));
    }
}

void createPlayerPart(int x, int y) {
    PlayerPart part = {x, y};
    gameInfo.playerParts[gameInfo.playerLength] = part;
    gameInfo.playerLength++;
}

void createNewTarget() {
    int emptySpacesCount = 0;
    int emptySpaces[gameRows * gameColumns][2];
    for (int x = 0; x < gameColumns; x++) {
        for (int y = 0; y < gameRows; y++) {
            if (isPlayerSpace(x, y) == 0) {
                emptySpaces[emptySpacesCount][0] = x;
                emptySpaces[emptySpacesCount][1] = y;
                emptySpacesCount++;
            }
        }
    }
    int randomIndex = randomNumber(0, emptySpacesCount);
    struct Target newTarget;
    newTarget.x = emptySpaces[randomIndex][0];
    newTarget.y = emptySpaces[randomIndex][1];
    gameInfo.target = newTarget;
}

/** @todo find a better way to find the player location */
int isPlayerSpace(int x, int y) {
    for (int i = 0; i < gameInfo.playerLength; i++) {
        if (gameInfo.playerParts[i].x == x && gameInfo.playerParts[i].y == y) {
            return 1;
        }
    }
    return 0;
}
