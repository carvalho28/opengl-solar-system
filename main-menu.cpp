// clang-format off
// #include <GLFW/glfw3.h>
// #include <glad/glad.h>
#include "main.h"
#include <string.h> 
#include <GL/glut.h>
#include <stdlib.h>
#include <iostream>
#include <filesystem>
#include <unistd.h>
// clang-format on
#include "main-menu.h"

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
    glScalef(3.0, 3.0, 3.0);
    for (size_t i = 0; i < strlen(str); i++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, str[i]);
    }
    glScalef(1 / 1.5, 1 / 1.5, 1 / 1.5);
}

void renderString(const char* str) {
    for (size_t i = 0; i < strlen(str); i++) {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, str[i]);
    }
}

// render string using glutStrokeCharacter with scale
void renderStrokeString(const char* str, int x, int y, float sx, float sy,
                        float sz) {
    glPushMatrix();
    // move to position
    glTranslatef(x, y, 0);
    glScalef(sx, sy, sz);
    // increase line width and render
    glLineWidth(3);
    for (size_t i = 0; i < strlen(str); i++) {
        glutStrokeCharacter(GLUT_STROKE_ROMAN, str[i]);
    }
    glPopMatrix();
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
    glColor3f(1, 0, 0);
    int widthStart =
        glutStrokeLength(GLUT_STROKE_ROMAN, (const unsigned char*)"Start Game");
    renderStrokeString("Start Game", WIDTH / 2 - widthStart / 10,
                       HEIGHT / 2 - 10, 0.2, 0.2, 0.2);

    // Draw the "Exit" button centered in the window below the "Start Game"
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
        glutStrokeLength(GLUT_STROKE_ROMAN, (const unsigned char*)"Exit");
    renderStrokeString("Exit", WIDTH / 2 - widthExit / 10, HEIGHT / 2 - 210,
                       0.2, 0.2, 0.2);

    // set above the start game button, a heading saying "Solar System"
    glColor3f(1, 1, 1);
    int width = glutStrokeLength(GLUT_STROKE_ROMAN,
                                 (const unsigned char*)"Solar System");
    renderStrokeString("Solar System", WIDTH / 2 - width / 4, HEIGHT / 2 + 150,
                       0.5, 0.5, 0.5);

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
    int widthLoading =
        glutStrokeLength(GLUT_STROKE_ROMAN, (const unsigned char*)"Loading...");
    renderStrokeString("Loading...", WIDTH / 2 - widthLoading / 10,
                       HEIGHT / 2 - 150, 0.2, 0.2, 0.2);

    // Swap buffers
    glutSwapBuffers();
}

void renderStarWarsText() {
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

    // star wars color
    glColor3f(1, 1, 0);

    // read text from file
    FILE* fp = fopen(
        "/Users/carvalho28/uni/cg/cg-avaliacoes/sistema-solar/resources/info/"
        "intro.txt",
        "r");
    char str[100];
    int i = -135;

    // present the text with scrolling effect
    while (fgets(str, 100, fp) != NULL) {
        int width =
            glutStrokeLength(GLUT_STROKE_ROMAN, (const unsigned char*)str);
        renderStrokeString(str, WIDTH / 2 - width / 10, HEIGHT / 3 + i, 0.2,
                           0.2, 0.2);
        i += 40;
    }

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
            // loadingScreen();
            //  read star wars text using
            renderStarWarsText();
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