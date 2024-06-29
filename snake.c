#include <stdio.h>
#include <GL/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include <unistd.h>
#include "renderer.h"
#include "game.h"
#include "util.h"

#define PREFER_X11 0

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480
#define WINDOW_TITLE "Snake (in C)"

#define GAME_ROWS 15
#define GAME_COLUMNS 20
#define GAME_FPS 3.0

void errorCallback(int error, const char* description);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void framebufferSizeCallback(GLFWwindow* window, int width, int height);

int main() {
    glfwSetErrorCallback(errorCallback);
    glfwInitHint(GLFW_WAYLAND_LIBDECOR, GLFW_WAYLAND_DISABLE_LIBDECOR);
    if (PREFER_X11 == 1 && glfwPlatformSupported(GLFW_PLATFORM_X11)) {
        glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_X11);
    }
    if (!glfwInit()) {
        printf("[ERROR] Failed to initialize GLFW library");
        return -1;
    }

    int majorv, minorv, revision;
    glfwGetVersion(&majorv, &minorv, &revision);
    printf("Running GLFW version: Major: %d, Minor: %d, Revision %d\n", majorv, minorv, revision);

    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, NULL, NULL);
    if (!window) {
        glfwTerminate();
        return 1;
    }

    glfwSetKeyCallback(window, keyCallback);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    initGame(GAME_ROWS, GAME_COLUMNS, window);
    initRenderer(GAME_ROWS, GAME_COLUMNS);

    const double fpsLimit = 1.0 / GAME_FPS;
    double lastFrameTime = 0;

    while (!glfwWindowShouldClose(window)) {
        double now = glfwGetTime();
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);

        updateGameLoop();
        draw(width, height, getGameState());

        glfwSwapBuffers(window);
        glfwPollEvents();
        
        double sleeptime = fpsLimit - (now - lastFrameTime);
        lastFrameTime = now;
        sleepSeconds(sleeptime);
    }

    destroyGame();
    glfwDestroyWindow(window);
    glfwTerminate();
    printf("Goodbye!\n");
    return 0;
}


void errorCallback(int error, const char* description) {
    fprintf(stderr, "Error: %s\n", description);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        switch (key) {
        case GLFW_KEY_ESCAPE:
            glfwSetWindowShouldClose(window, GLFW_TRUE);
            break;

        default:
            handleKeyPress(key);
            break;
        }
    }
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    printf("resize to: Width %d Height: %d\n", width, height);
}

