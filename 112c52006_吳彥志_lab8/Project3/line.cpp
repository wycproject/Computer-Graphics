#include <stdio.h> 
#include <stdlib.h>
#include "C:\OpenglLib\freeglut\include\GL\freeglut.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm> 


int boardSize = 21; // Default size of the chessboard
std::vector<std::vector<bool>> boardState;

void boardSizeMenu(int option) {
    boardSize = option;
    boardState.clear(); // Clear the existing state
    boardState.resize(boardSize, std::vector<bool>(boardSize, false)); // Reinitialize with the new size
    glutPostRedisplay();
}


void display() {
    int i, j;
    int cellWidth = glutGet(GLUT_WINDOW_WIDTH) / boardSize;
    int cellHeight = glutGet(GLUT_WINDOW_HEIGHT) / boardSize;

    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 1.0, 1.0); // White color for grid lines

    // Draw filled squares
    for (i = 0; i < boardSize; ++i) {
        for (j = 0; j < boardSize; ++j) {
            if (boardState[i][j]) {
                glColor3f(0.0, 1.0, 0.0); // Green for filled squares
                glBegin(GL_QUADS);
                glVertex2i(i * cellWidth, j * cellHeight);
                glVertex2i((i + 1) * cellWidth, j * cellHeight);
                glVertex2i((i + 1) * cellWidth, (j + 1) * cellHeight);
                glVertex2i(i * cellWidth, (j + 1) * cellHeight);
                glEnd();
            }
        }
    }

    // Draw grid lines
    glColor3f(1.0, 1.0, 1.0); // Reset color to white for grid lines
    glBegin(GL_LINES);
    for (i = 0; i <= boardSize; ++i) {
        int x = i * cellWidth;
        int y = i * cellHeight;
        glVertex2i(x, 0); glVertex2i(x, glutGet(GLUT_WINDOW_HEIGHT));
        glVertex2i(0, y); glVertex2i(glutGet(GLUT_WINDOW_WIDTH), y);
    }
    glEnd();

    glFlush();
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, w, 0, h); // Use pixel coordinates
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void mouseClick(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        int cellWidth = glutGet(GLUT_WINDOW_WIDTH) / boardSize;
        int cellHeight = glutGet(GLUT_WINDOW_HEIGHT) / boardSize;

        int i = x / cellWidth;
        int j = (glutGet(GLUT_WINDOW_HEIGHT) - y) / cellHeight; // Invert y to align with the top-left corner

        int n = 10;
        if (boardSize == 21) { n = 10; }
        if (boardSize == 31) { n = 15; }
        if (boardSize == 41) { n = 20; }

        if (i >= 0 && i < boardSize && j >= 0 && j < boardSize) {
            boardState[i][j] = !boardState[i][j]; // Toggle the square's state
            std::cout << "Square toggled at: (" << i-n << ", " << j-n << ")" << std::endl;
            glutPostRedisplay();
        }
    }
}

void createMenu() {
    glutCreateMenu(boardSizeMenu);
    glutAddMenuEntry("X,Y:10", 21);
    glutAddMenuEntry("X,Y:15", 31);
    glutAddMenuEntry("X,Y:20", 41);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(525, 525);
    glutCreateWindow("OpenGL Chessboard");

    // Initialize board state
    boardState.resize(boardSize, std::vector<bool>(boardSize, false));

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    createMenu();
    glutMouseFunc(mouseClick);

    glClearColor(0.0, 0.0, 0.0, 1.0); // Set background color to black
    glutMainLoop();
    return 0;
}