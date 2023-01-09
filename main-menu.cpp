// clang-format off
// #include <GLFW/glfw3.h>
// #include <glad/glad.h>
#include "main.h"
#include <string.h> 
#include <GL/glut.h>
#include <stdlib.h>
#include <iostream>
#include <filesystem>
// clang-format on
#include "main-menu.h"

float sx = 1.0;
float sy = 1.0;

// Window dimensions
const int WIDTH = 800;
const int HEIGHT = 600;
int window;

void closeWindow() { glutDestroyWindow(window); }

// void close program
void closeProgram() {
    glutDestroyWindow(window);
    exit(0);
}

// render string increased font
void renderTitle(const char* str) {
    for (size_t i = 0; i < strlen(str); i++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, str[i]);
    }
}

void renderString(const char* str) {
    for (size_t i = 0; i < strlen(str); i++) {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, str[i]);
    }
}

void display() {
    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT);

    // Set the projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, WIDTH, 0, HEIGHT);

    // Set the modelview matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Draw the "Start Game" button centered in the window
    glColor3f(1, 1, 1);
    glBegin(GL_QUADS);
    glVertex2f(WIDTH / 2 - 100, HEIGHT / 2 - 50);
    glVertex2f(WIDTH / 2 + 100, HEIGHT / 2 - 50);
    glVertex2f(WIDTH / 2 + 100, HEIGHT / 2 + 50);
    glVertex2f(WIDTH / 2 - 100, HEIGHT / 2 + 50);
    glEnd();

    // glEnable(GL_LIGHTING);
    // draw the text
    glColor3f(1, 0, 0);
    int widthStart = glutBitmapLength(GLUT_BITMAP_9_BY_15,
                                      (const unsigned char*)"Start Game");
    glRasterPos2f(WIDTH / 2 - widthStart / 2, HEIGHT / 2 - 10);
    renderString("Start Game");
    glFlush();

    // Draw the "Exit" button centered in the window below the "Start Game"
    // button
    glColor3f(1, 1, 1);
    glBegin(GL_QUADS);
    glVertex2f(WIDTH / 2 - 100, HEIGHT / 2 - 150);
    glVertex2f(WIDTH / 2 + 100, HEIGHT / 2 - 150);
    glVertex2f(WIDTH / 2 + 100, HEIGHT / 2 - 250);
    glVertex2f(WIDTH / 2 - 100, HEIGHT / 2 - 250);
    glEnd();

    // draw the text
    glColor3f(0, 0, 0);
    int widthExit =
        glutBitmapLength(GLUT_BITMAP_9_BY_15, (const unsigned char*)"Exit");
    glRasterPos2f(WIDTH / 2 - widthExit / 2, HEIGHT / 2 - 210);
    renderString("Exit");
    glFlush();

    // set above the start game button, a heading saying "Solar System"
    glColor3f(1, 1, 1);
    int widthSolarSystem = glutBitmapLength(
        GLUT_BITMAP_9_BY_15, (const unsigned char*)"Solar System");
    glRasterPos2f(WIDTH / 2 - widthSolarSystem / 2, HEIGHT / 2 + 100);
    renderTitle("Solar System");
    glFlush();

    // Swap buffers
    glutSwapBuffers();
}

// draw loading screen
void loadingScreen() {
    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT);

    // Set the projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, WIDTH, 0, HEIGHT);

    // Set the modelview matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // scale the size of the text
    glScalef(1, 2, 1);

    // set above the start game button, a heading saying "Solar System"
    glColor3f(1, 1, 1);
    int widthLoading = glutBitmapLength(GLUT_BITMAP_9_BY_15,
                                        (const unsigned char*)"Loading...");
    glRasterPos2f(WIDTH / 2 - widthLoading / 2, HEIGHT / 2 + 100);
    renderString("Loading...");
    glFlush();

    // Swap buffers
    glutSwapBuffers();
}

// check if the mouse was clicked inside the button
void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        printf("Mouse clicked at (%d, %d)\n", x, y);
        // Check if the mouse was clicked inside the "Start Game" button
        if (x >= WIDTH / 2 - 100 && x <= WIDTH / 2 + 100 &&
            y >= HEIGHT / 2 - 50 && y <= HEIGHT / 2 + 50) {
            printf("Start Game button clicked\n");
            //  show loading screen
            loadingScreen();
            // start the solar system and close the main menu
            glutDestroyWindow(window);
            solarSystemRender();
        }

        if (WIDTH / 2 - 100 <= x && x <= WIDTH / 2 + 100 &&
            HEIGHT / 2 - 150 <= y && y <= HEIGHT / 2 + 250) {
            printf("Exit button clicked\n");
            closeProgram();
        }
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(WIDTH, HEIGHT);
    // begin centering the window
    int screenWidth = glutGet(GLUT_SCREEN_WIDTH);
    int screenHeight = glutGet(GLUT_SCREEN_HEIGHT);
    int x = (screenWidth - WIDTH) / 2;
    int y = (screenHeight - HEIGHT) / 2;
    glutInitWindowPosition(x, y);
    window = glutCreateWindow("Main Menu");

    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutWMCloseFunc(closeWindow);
    // close on escape
    glutKeyboardFunc([](unsigned char key, int x, int y) {
        if (key == 27) {
            closeProgram();
        }
    });
    glutMainLoop();
    return 0;
}