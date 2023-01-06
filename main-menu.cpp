#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define GLM_ENABLE_EXPERIMENTAL
// #include <glm/gtx/intersect.hpp>

// // #include <learnopengl/filesystem.h>
// #include <learnopengl/camera.h>
// #include <learnopengl/model.h>
// #include <learnopengl/shader_m.h>

// #include <iostream>

// #define STB_IMAGE_IMPLEMENTATION
// #include <ft2build.h>
// #include <stb_image.h>

// #include <filesystem>
// #include FT_FREETYPE_H

// make a menu
#include <GL/freeglut.h>

bool animationRunning = false;

void startAnimation() { animationRunning = true; }

void drawButton() {
    // Draw a rectangle using the current color and the specified vertices
    glBegin(GL_QUADS);
    glVertex2f(-0.5, -0.5);
    glVertex2f(-0.5, 0.5);
    glVertex2f(0.5, 0.5);
    glVertex2f(0.5, -0.5);
    glEnd();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Set the color of the button
    glColor3f(1.0, 0.0, 0.0);

    // Draw the button at the center of the screen
    glPushMatrix();
    glTranslatef(0.0, 0.0, 0.0);
    drawButton();
    glPopMatrix();

    glFlush();
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    // Check if the left mouse button was clicked
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        // Check if the mouse click was inside the button
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        int width, height;
        glfwGetWindowSize(window, &width, &height);
        float x = (float)xpos / width - 0.5;
        float y = (float)ypos / height - 0.5;
        if (abs(x) < 0.5 && abs(y) < 0.5) {
            startAnimation();
        }
    }
}

int render() {
    // Initialize GLFW and create a window
    glfwInit();
    GLFWwindow* window = glfwCreateWindow(400, 400, "Menu", NULL, NULL);
    glfwMakeContextCurrent(window);

    // Set the mouse button callback function
    glfwSetMouseButtonCallback(window, mouseButtonCallback);

    // Set up the OpenGL context
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
}