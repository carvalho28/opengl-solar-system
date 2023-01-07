// clang-format off
// #include <GLFW/glfw3.h>
// #include <glad/glad.h>
#include "main.h"
#include <string.h> 
#include <GL/glut.h>
// clang-format on

// Window dimensions
const int WIDTH = 800;
const int HEIGHT = 600;

void drawText(char* string, int x, int y) {
    glRasterPos2i(x, y);
    int len = strlen(string);
    for (int i = 0; i < len; i++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string[i]);
    }
}

// draw text centered big in the middle of the screen
void drawCenteredText(char* str, int x, int y, void* font) {
    int len = glutBitmapLength(font, (unsigned char*)str);
    glRasterPos2i(x - len / 2, y);
    for (int i = 0; i < len; i++) {
        glutBitmapCharacter(font, str[i]);
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

    // draw the text
    glColor3f(0, 0, 0);
    drawText("Start Game!", WIDTH / 2 - 80, HEIGHT / 2 - 10);

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
    drawText("Exit", WIDTH / 2 - 20, HEIGHT / 2 - 190);

    // set above the start game button, a heading saying "Solar System"
    glColor3f(1, 1, 1);
    drawText("Solar System", WIDTH / 2 - 100, HEIGHT / 2 + 100);

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

    // set above the start game button, a heading saying "Solar System"
    glColor3f(1, 1, 1);
    drawText("Loading...", WIDTH / 2 - 100, HEIGHT / 2 + 100);

    // Swap buffers
    glutSwapBuffers();
}

// check if the mouse was clicked inside the button
void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        // Check if the mouse was clicked inside the "Start Game" button
        if (x >= WIDTH / 2 - 100 && x <= WIDTH / 2 + 100 &&
            y >= HEIGHT / 2 - 50 && y <= HEIGHT / 2 + 50) {
            //  show loading screen
            loadingScreen();
            // close the main menu window
            glutDestroyWindow(glutGetWindow());
            // start the solar system
            solarSystemRender();
        }

        // Check if the mouse was clicked inside the "Exit" button
        if (x >= WIDTH / 2 - 100 && x <= WIDTH / 2 + 100 &&
            y >= HEIGHT / 2 - 250 && y <= HEIGHT / 2 - 150) {
            // Close the window
            glutDestroyWindow(glutGetWindow());
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
    glutCreateWindow("Main Menu");
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutMainLoop();
    return 0;
}