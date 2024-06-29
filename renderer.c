#include "renderer.h"
#include <stdio.h>
#include <stdbool.h>
#include <GL/gl.h>
#include "util.h"

#define LINE_WIDTH 1.0f

void drawGrid();
void fillCell(int x, int y);
void drawLine(GLfloat startX, GLfloat startY, GLfloat endX, GLfloat endY);
void calcGridViewport(int rows, int columns, int screenWidth, int screenHeight);
void drawGameState(struct RenderGameState* gameState);

static struct GridViewPort {
    GLfloat x, y, width, height, cellWidth, cellHeight;
} gridViewPort;

struct GameSettings {
    int rows;
    int columns;
    GLfloat lineWidth;
    float windowPadding;
} gameSettings;

void initRenderer(int rows, int columns) {
    gameSettings.rows = rows;
    gameSettings.columns = columns;
    /** @todo Dynamic line width based on screen size? */
    gameSettings.lineWidth = LINE_WIDTH;
    gameSettings.windowPadding = 0.05f;
}

void draw(int screenWidth, int screenHeight, struct RenderGameState* gameState) {
    glClear(GL_COLOR_BUFFER_BIT);
    calcGridViewport(gameSettings.rows, gameSettings.columns, screenWidth, screenHeight);
    drawGrid();
    drawGameState(gameState);
}


void drawLine(GLfloat startX, GLfloat startY, GLfloat endX, GLfloat endY) {
    glVertex2f( startX, startY );
    glVertex2f( endX, endY );
}

void drawGrid() {
    glColor3f(1.0f, 1.0f, 1.0f);
    glLineWidth(LINE_WIDTH);

    glBegin(GL_LINES);

    GLfloat rowChunkSize = 0.0f - (gridViewPort.height / gameSettings.rows);
    GLfloat columnChunkSize = (gridViewPort.width / gameSettings.columns);

    // Draw vertical grid lines
    for (int i = 0; i <= gameSettings.columns; i++) {
        GLfloat xPos = (gridViewPort.x + (columnChunkSize * i));
        drawLine(xPos, gridViewPort.y, xPos, gridViewPort.y - gridViewPort.height);
    }
    // Draw horizontal grid lines
    for (int i = 0; i <= gameSettings.rows; i++) {
        GLfloat yPos = (gridViewPort.y + (rowChunkSize * i));
        drawLine(gridViewPort.x, yPos, gridViewPort.x + gridViewPort.width, yPos);
    }
    
    glEnd();
}

void drawGameState(struct RenderGameState* gameState) {
    for (int i = 0; i < gameState->length; i++) {
        fillCell(gameState->coordinates[i].x, gameState->coordinates[i].y);
    }
}

void fillCell(int x, int y) {
    if (x > gameSettings.columns || y > gameSettings.rows) {
        printf("[ERROR] Cell is out of bounds. got: %dx%d\n", x, y);
        return;
    }
    float startX = gridViewPort.x + gridViewPort.cellWidth * x;
    float startY = gridViewPort.y - gridViewPort.cellHeight * (y + 1);

    glBegin(GL_POLYGON);
        glVertex2f(startX, startY);
        glVertex2f(startX + gridViewPort.cellWidth, startY);
        glVertex2f(startX + gridViewPort.cellWidth, startY + gridViewPort.cellHeight);
        glVertex2f(startX, startY + gridViewPort.cellHeight);        
    glEnd();
}

void calcGridViewport(int rows, int columns, int screenWidth, int screenHeight) {
    float screenSize = 2.0f - gameSettings.windowPadding * 2;
    float windowStartH = -1.0f + gameSettings.windowPadding;
    float windowStartV = 1.0f - gameSettings.windowPadding;
    float widthChunk = screenWidth / columns;
    float columnPercentage = (widthChunk / screenWidth) * 100;
    float heightChunk = screenHeight / rows;
    float rowPercentage = (heightChunk / screenHeight) * 100;

    if (columnPercentage < rowPercentage) {
        float cellHeight = (screenSize / 100) * (widthChunk / screenHeight) * 100;
        float cellWidth = (screenSize / 100) * columnPercentage;
        float usableHeight = cellHeight * rows;
        gridViewPort.x = windowStartH;
        gridViewPort.y = usableHeight / 2;
        gridViewPort.width = screenSize;
        gridViewPort.height = usableHeight;
        gridViewPort.cellWidth = cellWidth;
        gridViewPort.cellHeight = cellHeight;
    } else { // rowPercentage < columnPercentage
        float cellWidth = (screenSize / 100) *  (heightChunk / screenWidth) * 100;
        float cellHeight = (screenSize / 100) * rowPercentage;
        float usableWidth = cellWidth * columns;
        gridViewPort.x = 0.0f - usableWidth / 2;
        gridViewPort.y = windowStartV;
        gridViewPort.width = usableWidth;
        gridViewPort.height = screenSize;
        gridViewPort.cellWidth = cellWidth;
        gridViewPort.cellHeight = cellHeight;
    }
}
