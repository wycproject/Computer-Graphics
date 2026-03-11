#include <stdio.h> 
#include <stdlib.h>
#include "C:\OpenglLib\freeglut\include\GL\freeglut.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm> 
#include <utility>
#include <cmath>
#include <queue>
std::queue<std::pair<int, int>> fillQueue; // Queue to hold points to be filled
int refreshMills = 30; // refresh interval in milliseconds

int boardSize = 21; // Default size of the chessboard
std::vector<std::vector<int>> boardState(boardSize, std::vector<int>(boardSize, 0)); // 0 = none, 1 = red, 2 = green

std::vector<std::pair<int, int>> clickHistory; // Stores the last n clicks

std::vector<std::pair<int, int>> clickHistorymod; // Stores the last n clicks 調整過後(因為會有偏差)


void boardSizeMenu(int option) {
    boardSize = option;
    boardState.clear(); // Clear the existing state
    boardState.resize(boardSize, std::vector<int>(boardSize, 0)); // Reinitialize with the new size
    
    clickHistory.clear();
    glutPostRedisplay();
}

void setSquare(int i, int j, int color) {
    if (i >= 0 && i < boardSize && j >= 0 && j < boardSize) {
        boardState[i][j] = color;
    }
}

bool isPointInTriangle(int px, int py, std::pair<int, int> v1, std::pair<int, int> v2, std::pair<int, int> v3) {
    int d1, d2, d3;
    bool has_neg, has_pos;

    auto sign = [](int x1, int y1, int x2, int y2, int x3, int y3) {
        return (x1 - x3) * (y2 - y3) - (x2 - x3) * (y1 - y3);
    };

    d1 = sign(px, py, v1.first, v1.second, v2.first, v2.second);
    d2 = sign(px, py, v2.first, v2.second, v3.first, v3.second);
    d3 = sign(px, py, v3.first, v3.second, v1.first, v1.second);

    has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
    has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);

    return !(has_neg && has_pos);
}

void enqueueFillTriangle(std::vector<std::pair<int, int>> points) {
    int min_x = std::min({ points[0].first, points[1].first, points[2].first });
    int max_x = std::max({ points[0].first, points[1].first, points[2].first });
    int min_y = std::min({ points[0].second, points[1].second, points[2].second });
    int max_y = std::max({ points[0].second, points[1].second, points[2].second });

    for (int x = min_x; x <= max_x; x++) {
        for (int y = min_y; y <= max_y; y++) {
            if (isPointInTriangle(x, y, points[0], points[1], points[2])) {
                fillQueue.push({ x, y });
            }
        }
    }
}

void determineLineType(int x0, int y0, int x1, int y1) {
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);

    if (x1 > x0) {
        if (y1 > y0) {//四之第一象限
            if (dx > dy) {//八之第一象限
                std::cout << "第一象限" << std::endl;
            }
            else {//八之第二象限
                std::cout << "第二象限" << std::endl;
            }
        }
        else {//y1<y0 四之第四象限
            if (dx > dy) {//八之第八象限
                std::cout << "第八象限" << std::endl;
            }
            else {//八之第七象限
                std::cout << "第七象限" << std::endl;
            }
        }
    }
    else {
        if (y1 > y0) {//四之第二象限
            if (dx > dy) {//八之第四象限
                std::cout << "第四象限" << std::endl;
            }
            else {//八之第三象限
                std::cout << "第三象限" << std::endl;
            }
        }
        else {//y1<y0 四之第三象限
            if (dx > dy) {//八之第五象限
                std::cout << "第五象限" << std::endl;
            }
            else {//八之第六象限
                std::cout << "第六象限" << std::endl;
            }
        }        
    }
}

void line(int x0, int y0, int x1, int y1) {
    determineLineType(x0, y0, x1, y1); // Determine and print line type

    int a = 0, b = 0, c = 0, d = 0;//用來偵測xy是否都有同時改變有的話來畫藍色
    int originx0 = x0, originy0 = y0;
    const int dx = abs(x1 - x0);
    const int dy = -abs(y1 - y0);
    const int sx = x0 < x1 ? 1 : -1;
    const int sy = y0 < y1 ? 1 : -1;
    int err = dx + dy, e2; // error value e_xy

    while (true) {  // The loop will run until it breaks when x0 == x1 and y0 == y1
        setSquare(x0, y0, 2); // Set the current square to green
        if (a != b && c != d) {
            setSquare(x0, y0, 3);
            
        }
        a = 0; b = 0; c = 0; d = 0;
        if (x0 == x1 && y0 == y1) break;
        e2 = 2 * err;
        if (e2 >= dy) { 
            err += dy;
            a = x0;
            x0 += sx;
            b = x0;
        }
        if (e2 <= dx) { 
            err += dx;
            c = y0;
            y0 += sy;
            d = y0;
        }
        
    }
    setSquare(originx0, originy0, 1);//把起始點畫成紅色
    setSquare(x1, y1, 1);
}

void Timer(int value) {
    if (!fillQueue.empty()) {
        auto p = fillQueue.front();
        fillQueue.pop();
        setSquare(p.first, p.second, 2); // Draw one square at a time
    }
    glutPostRedisplay();      // Post re-paint request to activate display()
    glutTimerFunc(refreshMills, Timer, 0); // next Timer call milliseconds later
}

void display() {
    int i, j;
    glClear(GL_COLOR_BUFFER_BIT);
    int cellWidth = glutGet(GLUT_WINDOW_WIDTH) / boardSize;
    int cellHeight = glutGet(GLUT_WINDOW_HEIGHT) / boardSize;

    // Draw filled squares
    for (int i = 0; i < boardSize; ++i) {
        for (int j = 0; j < boardSize; ++j) {
            if (boardState[i][j] == 1) {
                glColor3f(1.0, 0.0, 0.0); // Red
                glBegin(GL_QUADS);
                glVertex2i(i * cellWidth, j * cellHeight);
                glVertex2i((i + 1) * cellWidth, j * cellHeight);
                glVertex2i((i + 1) * cellWidth, (j + 1) * cellHeight);
                glVertex2i(i * cellWidth, (j + 1) * cellHeight);
                glEnd();
            }
        }
    }

    for (i = 0; i < boardSize; ++i) {
        for (j = 0; j < boardSize; ++j) {
            switch (boardState[i][j]) {
            case 1:
                glColor3f(1.0, 0.0, 0.0); // Red
                break;
            case 2:
                glColor3f(0.0, 1.0, 0.0); // Green
                break;
            case 3:
                glColor3f(0.0, 0.0, 1.0); // Blue
                break;
            default:
                continue; // No need to draw squares with no color
            }
            glBegin(GL_QUADS);
            glVertex2i(i * cellWidth, j * cellHeight);
            glVertex2i((i + 1) * cellWidth, j * cellHeight);
            glVertex2i((i + 1) * cellWidth, (j + 1) * cellHeight);
            glVertex2i(i * cellWidth, (j + 1) * cellHeight);
            glEnd();
        }
    }

    

    // Draw grid lines
    glColor3f(1.0, 1.0, 1.0); // White
    glBegin(GL_LINES);
    for (int i = 0; i <= boardSize; ++i) {
        int x = i * cellWidth;
        int y = i * cellHeight;
        glVertex2i(x, 0);
        glVertex2i(x, glutGet(GLUT_WINDOW_HEIGHT));
        glVertex2i(0, y);
        glVertex2i(glutGet(GLUT_WINDOW_WIDTH), y);
    }
    glEnd();
    glFlush();
}


void mouseClick(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        int cellWidth = glutGet(GLUT_WINDOW_WIDTH) / boardSize;
        int cellHeight = glutGet(GLUT_WINDOW_HEIGHT) / boardSize;

        int i = x / cellWidth;
        int j = (glutGet(GLUT_WINDOW_HEIGHT) - y) / cellHeight;

        if (i >= 0 && i < boardSize && j >= 0 && j < boardSize) {
            setSquare(i, j, 1); // Color the clicked square red

            // Update the last three clicks history
            if (clickHistory.size() == 3) {
                clickHistory.erase(clickHistory.begin()); // Remove the oldest click
            }
            clickHistory.push_back({ i * cellWidth, j * cellHeight }); // Add new click

            if (clickHistorymod.size() == 3) {
                clickHistorymod.erase(clickHistorymod.begin()); // Remove the oldest click
            }
            clickHistorymod.push_back({ i , j  }); // Add new click

            if (clickHistory.size() == 3) {
                // Draw lines between the last three clicks
                line(clickHistory[0].first / cellWidth, clickHistory[0].second / cellHeight, clickHistory[1].first / cellWidth, clickHistory[1].second / cellHeight);
                line(clickHistory[1].first / cellWidth, clickHistory[1].second / cellHeight, clickHistory[2].first / cellWidth, clickHistory[2].second / cellHeight);
                line(clickHistory[2].first / cellWidth, clickHistory[2].second / cellHeight, clickHistory[0].first / cellWidth, clickHistory[0].second / cellHeight);
                enqueueFillTriangle(clickHistorymod);
                
            }

            glutPostRedisplay();
        }


    }
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, w, 0, h);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}


void createMenu() {
    glutCreateMenu(boardSizeMenu);
    glutAddMenuEntry("21x21", 21);
    glutAddMenuEntry("31x31", 31);
    glutAddMenuEntry("41x41", 41);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(525, 525);
    glutCreateWindow("OpenGL Chessboard");

    boardState.resize(boardSize, std::vector<int>(boardSize, 0)); // Initialize board state

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    
    createMenu();
    glutMouseFunc(mouseClick);
    glutTimerFunc(0, Timer, 0);     // First timer call immediately

    glClearColor(0.0, 0.0, 0.0, 1.0); // Set background color to black
    glutMainLoop();



    return 0;
}