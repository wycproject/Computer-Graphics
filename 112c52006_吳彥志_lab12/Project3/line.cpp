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
#include <limits>
#include <cstdlib>  // 引入標頭檔以使用 rand() 和 srand()
#include <ctime>    // 引入標頭檔以使用 time()

std::queue<std::pair<int, int>> fillQueue; // Queue to hold points to be filled
int refreshMills = 30; // refresh interval in milliseconds

int boardSize = 21; // Default size of the chessboard
std::vector<std::vector<int>> boardState(boardSize, std::vector<int>(boardSize, 0)); // 0 = none, 1 = red, 2 = green

std::vector<std::pair<int, int>> clickHistory; // Stores the last 5 clicks
std::vector<std::pair<int, int>> clickHistorymod; // Stores the last 5 clicks adjusted

int s = 0;

//判斷是否已經點到同一個點
bool contains_first(const std::vector<std::pair<int, int>>& vec, int num1, int num2) {
    int count = 0;  // 用於計數匹配的次數
    for (const auto& p : vec) {
        if (p.first == num1 && p.second == num2) {
            ++count; // 每找到一次匹配，計數器加一
            if (count >= 2) {
                return false; // 如果計數器達到或超過兩次，返回 true
            }
        }
    }
    return true; // 遍歷完沒有找到，返回 ture
}

void boardSizeMenu(int option) {
    boardSize = option;
    boardState.clear(); // Clear the existing state
    boardState.resize(boardSize, std::vector<int>(boardSize, 0)); // Reinitialize with the new size

    clickHistory.clear();
    clickHistorymod.clear();
    glutPostRedisplay();
}

void setSquare(int i, int j,int x) {
    if (i >= 0 && i < boardSize && j >= 0 && j < boardSize) {

        int color = rand() % 14 + 1;  // 生成1到3之間的隨機數（1=紅色, 2=綠色, 3=藍色）
        boardState[i][j] = color;
    }
}

bool isPointInPolygon(const std::vector<std::pair<int, int>>& polygon, int px, int py) {
    int n = polygon.size();
    bool inside = false;
    for (int i = 0, j = n - 1; i < n; j = i++) {
        int xi = polygon[i].first, yi = polygon[i].second;
        int xj = polygon[j].first, yj = polygon[j].second;

        bool intersect = ((yi > py) != (yj > py)) &&
            (px < (xj - xi)* (py - yi) / (yj - yi) + xi);
        if (intersect) inside = !inside;
    }
    return inside;
}

void fillPentagon(const std::vector<std::pair<int, int>>& polygon) {
    int min_x = std::numeric_limits<int>::max();
    int max_x = std::numeric_limits<int>::min();
    int min_y = std::numeric_limits<int>::max();
    int max_y = std::numeric_limits<int>::min();
    for (const auto& p : polygon) {
        min_x = std::min(min_x, p.first);
        max_x = std::max(max_x, p.first);
        min_y = std::min(min_y, p.second);
        max_y = std::max(max_y, p.second);
    }

    for (int x = min_x; x <= max_x + 1; x++) {
        for (int y = min_y; y <= max_y + 1; y++) {
            if (isPointInPolygon(polygon, x, y)) {
                fillQueue.push({ x, y }); // Fill the square at (x, y)
            }
        }
    }
}

void line(int x0, int y0, int x1, int y1) {


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
    setSquare(originx0, originy0,15);//把起始點畫成紅色
    setSquare(x1, y1,15);
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
    int cellWidth = glutGet(GLUT_WINDOW_WIDTH) / boardSize;
    int cellHeight = glutGet(GLUT_WINDOW_HEIGHT) / boardSize;

    glClear(GL_COLOR_BUFFER_BIT);
    for (int i = 0; i < boardSize; ++i) {
        for (int j = 0; j < boardSize; ++j) {
            switch (boardState[i][j]) {
            case 1:
                glColor3f(1.0, 0.0, 0.0); // Red
                break;
            case 2:
                glColor3f(1.0, 0.5, 0.0);
                break;
            case 3:
                glColor3f(1.0, 1.0, 0.0); // Blue
                break;
            case 4:
                glColor3f(1.0, 0.0, 0.5); //  Red
                break;
            case 5:
                glColor3f(1.0, 0.0, 1.0); // Green
                break;
            case 6:
                glColor3f(0.5, 0.0, 0.0); // Blue
                break;
            case 7:
                glColor3f(0.5, 0.5, 0.0); // Red
                break;
            case 8:
                glColor3f(0.5, 1.0, 0.0); // Green
                break;
            case 9:
                glColor3f(0.5, 0.0, 0.5); // Blue
                break;
            case 10:
                glColor3f(0.5, 0.0, 1.0); // Red
                break;
            case 11:
                glColor3f(0.0, 0.5, 0.0); // Red
                break;
            case 12:
                glColor3f(0.0, 1.0, 0.0); // Red
                break;
            case 13:
                glColor3f(0.0, 0.0, 0.5); // Red
                break;
            case 14:
                glColor3f(0.0, 0.0, 1.0); // Red
                break;
            case 15:
                glColor3f(1.0, 1.0, 1.0); // Red
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
            std::pair<int, int> newClick = { i, j };

            // Check if the new click is a duplicate of any previous click
            auto it = std::find(clickHistory.begin(), clickHistory.end(), newClick);
            if (it != clickHistory.end()) {
                // Close and fill the polygon
                for (size_t k = 0; k < clickHistory.size(); ++k) {
                    size_t next = (k + 1) % clickHistory.size();
                    line(clickHistory[k].first, clickHistory[k].second,
                        clickHistory[next].first, clickHistory[next].second);
                }

                fillPentagon(clickHistory); // This function will need to be generalized to fill any polygon
                clickHistory.clear();
            }
            else {
                setSquare(i, j, 15);// Color the clicked square red
                clickHistory.push_back(newClick);
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
    srand(time(0));  // 使用當前時間作為隨機數生成的種子
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(525, 525);
    glutCreateWindow("OpenGL Chessboard");

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    createMenu();
    glutMouseFunc(mouseClick);
    glutTimerFunc(0, Timer, 0);     // First timer call immediately
    
    glClearColor(0.0, 0.0, 0.0, 1.0); // Set background color to black
    glutMainLoop();

    return 0;
}
