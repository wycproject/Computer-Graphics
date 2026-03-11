#include <iostream>
#include <math.h>
#include <stdlib.h>

#include "C:\OpenglLib\freeglut\include\GL\freeglut.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "C:\OpenglLib\header\math3d.h" 


//以下測試

using namespace std;

const GLfloat Pi = 3.1415926536f;

// 攝影機與物體的距離
float G_fDistance = 3.6f;
// 機器人的旋轉角度
float G_fAngle_horizon = 0.0f;
float G_fAngle_vertical = 0.0f;

float G_left_upper_arm1 = 0.0f; // 左上臂活動角度(前後活動)
float G_left_upper_arm2 = 0.0f; // 左上臂活動角度(左右活動)
float G_left_lower_arm1 = 0.0f; // 左下臂活動角度(前後活動)
float G_left_lower_arm2 = 0.0f; // 左下臂活動角度(左右活動)
float G_right_upper_arm1 = 0.0f; // 右上臂活動角度(前後活動)
float G_right_upper_arm2 = 0.0f; // 右上臂活動角度(左右活動)
float G_right_lower_arm1 = 0.0f; // 右下臂活動角度(前後活動)
float G_right_lower_arm2 = 0.0f; // 右下臂活動角度(左右活動)

float G_left_upper_leg = 0.0f; // 左大腿活動角度(前後活動)
float G_left_lower_leg = 0.0f; // 左小腿活動角度(前後活動)
float G_right_upper_leg = 0.0f; // 右大腿活動角度(前後活動)
float G_right_lower_leg = 0.0f; // 右小腿活動角度(前後活動)

void myinit(void);
void myReshape(GLsizei w, GLsizei h);
void display(void);
void processSpecialKeys(int key, int x, int y);
void processNormalKeys(unsigned char key, int x, int y);
void timer(int value);

int main(int argc, char* argv[])
{
    glutInit(&argc, argv);

    // 初始化OPENGL顯示方式
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

    // 設定OPENGL窗口位置和大小
    glutInitWindowSize(500, 800);
    glutInitWindowPosition(100, 100);

    // 打開窗口
    glutCreateWindow("簡單機器人");
    cout << "w,s,a,d控制機器人右上臂前後左右" << "\n";
    cout << "5,2,1,3控制機器人左上臂前後左右" << "\n";
    cout << "t,g,f,h控制機器人右下臂前後左右" << "\n";
    cout << "i,k,j,l控制機器人左下臂前後左右" << "\n";
    cout << "6,7控制機器人右大腿前後" << "\n";
    cout << "8,9控制機器人左大腿前後" << "\n";
    cout << "z,x控制機器人右小腿前後" << "\n";
    cout << "c,v控制機器人左小腿前後" << "\n";
    cout << "[ , ]用來控制機器人距離螢幕" << "\n";
    cout << "鍵盤上的上下左右也可以旋轉機器人" << "\n";

    // 調用初始化函數
    myinit();

    // 設定窗口大小變化的回調函數
    glutReshapeFunc(myReshape);

    // 設定繪畫控制的回調函數
    glutSpecialFunc(processSpecialKeys);
    glutKeyboardFunc(processNormalKeys);

    // 開始OPENGL循環
    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
}

void myinit(void) // 初始化
{
    glEnable(GL_DEPTH_TEST);
}

void myReshape(GLsizei w, GLsizei h) // 設定窗口大小變化的回調函數
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, (GLfloat)w / (GLfloat)h, 1, 30);
}

void drawstar(GLfloat a, GLfloat x, GLfloat y) // 畫星星的函數
{
    // a為星星的半徑, x, y為平移的量
    GLfloat bx = a * cos(18 * Pi / 180);
    GLfloat by = a * sin(18 * Pi / 180);
    GLfloat cy = -a * cos(36 * Pi / 180);
    GLfloat cx = a * sin(36 * Pi / 180);
    GLfloat pointA[2] = { 0, a };
    GLfloat pointB[2] = { bx, by };
    GLfloat pointC[2] = { cx, cy };
    GLfloat pointD[2] = { -cx, cy };
    GLfloat pointE[2] = { -bx, by };

    // 按照A->C->E->B->D->A的順序,可以一筆將五角星畫出
    glTranslatef(x, y, 0.0);
    glColor3f(1.0, 1.0, 0.0);
    glBegin(GL_LINE_LOOP);
    glVertex2fv(pointA);
    glVertex2fv(pointC);
    glVertex2fv(pointE);
    glVertex2fv(pointB);
    glVertex2fv(pointD);
    glEnd();
}

void drawfinger(GLfloat x, GLfloat y, GLfloat z) // 畫手指
{
    glPushMatrix();
    glColor3f(0.0, 0.0, 1.0);
    glTranslatef(x, y, z);
    glPushMatrix();
    glScalef(0.025, 0.05, 0.025);
    glutSolidCube(1.0);
    glPopMatrix();
    glPopMatrix();
}

void drawfoot(GLfloat x, GLfloat y, GLfloat z) // 畫腳
{
    glPushMatrix();
    glColor3f(0.0, 0.0, 1.0);
    glTranslatef(x, y, z);
    glPushMatrix();
    glScalef(0.25, 0.05, 0.25);
    glutSolidCube(1.0);
    glPopMatrix();
    glPopMatrix();
}

void display(void)
{
    // 清除顏色和深度緩存
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClearDepth(1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // 視角的變化
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -G_fDistance);
    glRotatef(G_fAngle_horizon, 0.0f, 1.0f, 0.0f);
    glRotatef(G_fAngle_vertical, 1.0f, 0.0f, 0.0f);

    // 身體
    glColor3f(1.0, 0.0, 0.0);
    glPushMatrix();
    glScalef(1.0, 0.75, 0.5);
    glutSolidCube(1.0);
    glPopMatrix();

    // 畫五星紅旗
    glPushMatrix();
    GLfloat distance = 0.15; // 大的五角星到小的五角星的距離
    GLfloat x1 = distance * cos(54 * Pi / 180);
    GLfloat y1 = distance * sin(54 * Pi / 180);
    GLfloat x2 = distance * cos(18 * Pi / 180);
    GLfloat y2 = distance * sin(18 * Pi / 180);
    drawstar(0.1, -0.4, 0.225);
    glPushMatrix();
    drawstar(0.05, x1, y1);
    glPopMatrix();
    glPushMatrix();
    drawstar(0.05, x2, y2);
    glPopMatrix();
    glPushMatrix();
    drawstar(0.05, x1, -y1);
    glPopMatrix();
    glPushMatrix();
    drawstar(0.05, x2, -y2);
    glPopMatrix();
    glPopMatrix();

        
    // 頭和眼
    glPushMatrix();
    // 頭
    glTranslatef(0.0, 0.625, 0.0);
    glColor3f(0.8, 0.5, 0.2);
    glutSolidSphere(0.25, 20, 20);

    // 綠帽子
    glPushMatrix();
    glColor3f(0.0, 1.0, 0.0);
    glTranslatef(0.0, 0.25, 0.0);
    glRotatef(90, 1.0, 0.0, 0.0);
    glutSolidTorus(0.1, 0.15, 20, 20);
    glPopMatrix();

    // 左眼
    glPushMatrix();
    glColor3f(1.0, 0.0, 0.0);
    glTranslatef(-0.12, 0.0, 0.0);
    glScalef(1.0, 0.75, 0.5);
    glutSolidSphere(0.1, 20, 20);
    glPopMatrix();

    // 右眼
    glPushMatrix();
    glColor3f(1.0, 0.0, 0.0);
    glTranslatef(0.12, 0.0, 0.0);
    glScalef(1.0, 0.75, 0.5);
    glutSolidSphere(0.1, 20, 20);
    glPopMatrix();

    glPopMatrix();

    // 左臂
    glPushMatrix();
    // 左上臂
    glColor3f(1.0, 0.0, 1.0);
    glTranslatef(-0.575, 0.375, 0.0);
    glRotatef(G_left_upper_arm1, 0.0, 0.0, 1.0);
    glRotatef(G_left_upper_arm2, 1.0, 0.0, 0.0);
    glTranslatef(0.0, -0.225, 0.0);
    glPushMatrix();
    glScalef(0.125, 0.45, 0.3);
    glutSolidCube(1.0);
    glPopMatrix();

    // 左肘
    glColor3f(0.0, 1.0, 1.0);
    glTranslatef(0.0, -0.25625, 0.0);
    glutSolidSphere(0.0625, 20, 20);

    // 左下臂
    glColor3f(1.0, 0.0, 1.0);
    glTranslatef(0.0, -0.03125, 0.0);
    glRotatef(G_left_lower_arm1, 0.0, 0.0, 1.0);
    glRotatef(G_left_lower_arm2, 1.0, 0.0, 0.0);
    glTranslatef(0.0, -0.225, 0.0);
    glPushMatrix();
    glScalef(0.125, 0.45, 0.15);
    glutSolidCube(1.0);
    glPopMatrix();

    // 畫手指
    drawfinger(-0.0675, -0.25, 0.075);
    drawfinger(0.0675, -0.25, 0.075);
    drawfinger(0, -0.25, -0.075);
    glPopMatrix();
    
    // 右臂
    glPushMatrix();
    // 右上臂
    glColor3f(1.0, 0.0, 1.0);
    glTranslatef(0.575, 0.375, 0.0);
    glRotatef(G_right_upper_arm1, 0.0, 0.0, 1.0);
    glRotatef(G_right_upper_arm2, 1.0, 0.0, 0.0);
    glTranslatef(0.0, -0.225, 0.0);
    glPushMatrix();
    glScalef(0.125, 0.45, 0.3);
    glutSolidCube(1.0);
    glPopMatrix();

    // 右肘
    glColor3f(0.0, 1.0, 1.0);
    glTranslatef(0.0, -0.25625, 0.0);
    glutSolidSphere(0.0625, 20, 20);

    // 右下臂
    glColor3f(1.0, 0.0, 1.0);
    glTranslatef(0.0, -0.03125, 0.0);
    glRotatef(G_right_lower_arm1, 0.0, 0.0, 1.0);
    glRotatef(G_right_lower_arm2, 1.0, 0.0, 0.0);
    glTranslatef(0.0, -0.225, 0.0);
    glPushMatrix();
    glScalef(0.125, 0.45, 0.15);
    glutSolidCube(1.0);
    glPopMatrix();

    // 畫手指
    drawfinger(-0.0675, -0.25, 0.075);
    drawfinger(0.0675, -0.25, 0.075);
    drawfinger(0, -0.25, -0.075);
    glPopMatrix();


    // 左腿
    glPushMatrix();
    // 左大腿
    glColor3f(1.0, 0.0, 1.0);
    glTranslatef(0.15, -0.375, 0.0);
    glRotatef(G_left_upper_leg, 1.0, 0.0, 0.0);
    glTranslatef(0.0, -0.225, 0.0);
    glPushMatrix();
    glScalef(0.25, 0.45, 0.3);
    glutSolidCube(1.0);
    glPopMatrix();
    // 左膝蓋
    glColor3f(0.0, 1.0, 1.0);
    glTranslatef(0.0, -0.25625, 0.0);
    glutSolidSphere(0.0625, 20, 20);
    // 左小腿
    glColor3f(1.0, 0.0, 1.0);
    glTranslatef(0.0, -0.0625, 0.0);
    glRotatef(G_left_lower_leg, 1.0, 0.0, 0.0);
    glTranslatef(0.0, -0.225, 0.0);
    glPushMatrix();
    glScalef(0.125, 0.45, 0.3);
    glutSolidCube(1.0);
    glPopMatrix();
    drawfoot(0.0, -0.25, 0.0);
    glPopMatrix();
    // 右腿
    glPushMatrix();
    // 右大腿
    glColor3f(1.0, 0.0, 1.0);
    glTranslatef(-0.15, -0.375, 0.0);
    glRotatef(G_right_upper_leg, 1.0, 0.0, 0.0);
    glTranslatef(0.0, -0.225, 0.0);
    glPushMatrix();
    glScalef(0.25, 0.45, 0.3);
    glutSolidCube(1.0);
    glPopMatrix();
    // 右膝蓋
    glColor3f(0.0, 1.0, 1.0);
    glTranslatef(0.0, -0.25625, 0.0);
    glutSolidSphere(0.0625, 20, 20);
    // 右小腿
    glColor3f(1.0, 0.0, 1.0);
    glTranslatef(0.0, -0.0625, 0.0);
    glRotatef(G_right_lower_leg, 1.0, 0.0, 0.0);
    glTranslatef(0.0, -0.225, 0.0);
    glPushMatrix();
    glScalef(0.125, 0.45, 0.3);
    glutSolidCube(1.0);
    glPopMatrix();
    drawfoot(0.0, -0.25, 0.0);
    glPopMatrix();
    //交換前後緩衝區
    glutSwapBuffers();
}

void processSpecialKeys(int key, int x, int y) 
{
    switch (key) {
    case GLUT_KEY_LEFT:
        G_fAngle_horizon -= 5.0f;
        break;
    case GLUT_KEY_RIGHT:
        G_fAngle_horizon += 5.0f;
        break;
    case GLUT_KEY_UP:
        G_fAngle_vertical -= 5.0f;
        break;
    case GLUT_KEY_DOWN:
        G_fAngle_vertical += 5.0f;
        break;
    }
    glutPostRedisplay();
}

void processNormalKeys(unsigned char key, int x, int y) {
    switch (key) {
    case 91: // [
        G_fDistance -= 0.3f;
        break;
    case 93: // ]
        G_fDistance += 0.3f;
        break;
    case 49: // 1
        G_left_upper_arm1 -= 15.0;
        break;
    case 51: // 3
        G_left_upper_arm1 += 15.0;
        break;
    case 50: // 2
        G_left_upper_arm2 -= 15.0;
        break;
    case 53: // 5
        G_left_upper_arm2 += 15.0;
        break;
        // i、k、j、L控制機器人左臂下臂前後左右
    case 106: // j
        G_left_lower_arm1 -= 15.0;
        break;
    case 108: // l
        G_left_lower_arm1 += 15.0;
        break;
    case 105: // i
        G_left_lower_arm2 -= 15.0;
        break;
    case 107: // k
        G_left_lower_arm2 += 15.0;
        break;
    case 97: // a
        G_right_upper_arm1 -= 15.0;
        break;
    case 100: // d
        G_right_upper_arm1 += 15.0;
        break;
    case 119: // w
        G_right_upper_arm2 -= 15.0;
        break;
    case 115: // s
        G_right_upper_arm2 += 15.0;
        break;
    case 102: // f
        G_right_lower_arm1 -= 15.0;
        break;
    case 104: // h
        G_right_lower_arm1 += 15.0;
        break;
    case 116: // t
        G_right_lower_arm2 -= 15.0;
        break;
    case 103: // g
        G_right_lower_arm2 += 15.0;
        break;
    case 56: // 8
        G_left_upper_leg -= 15.0;
        break;
    case 57: // 9
        G_left_upper_leg += 15.0;
        break;
    case 99: // c
        G_left_lower_leg -= 15.0;
        break;
    case 118: // v
        G_left_lower_leg += 15.0;
        break;
    case 54: // 6
        G_right_upper_leg -= 15.0;
        break;
    case 55: // 7
        G_right_upper_leg += 15.0;
        break;
    case 122: // z
        G_right_lower_leg -= 15.0;
        break;
    case 120: // x
        G_right_lower_leg += 15.0;
        break;
    case 27: // esc
        exit(0);
        break;
    }
    glutPostRedisplay();
}



//以上測試


/*
// Shadow.cpp
// OpenGL SuperBible
// Demonstrates simple planar shadows
// Program by Richard S. Wright Jr.


// Rotation amounts
static GLfloat xRot = 0.0f;
static GLfloat yRot = 0.0f;

// These values need to be available globally
// Light values and coordinates
GLfloat  ambientLight[] = { 0.3f, 0.3f, 0.3f, 1.0f };
GLfloat  diffuseLight[] = { 0.7f, 0.7f, 0.7f, 1.0f };
GLfloat  specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat	 lightPos[] = { -75.0f, 150.0f, -50.0f, 0.0f };
GLfloat  specref[] = { 1.0f, 1.0f, 1.0f, 1.0f };

// Transformation matrix to project shadow
M3DMatrix44f shadowMat;


//以下測試

int activeLight = 0;

GLfloat lightPoss[4][4] = {
    { -75.0f, 150.0f, -50.0f, 0.0f },
    { 75.0f, 150.0f, -50.0f, 0.0f },
    { -75.0f, 100.0f, -50.0f, 0.0f },
    { 75.0f, 100.0f, -50.0f, 0.0f }
};





//以上測試

////////////////////////////////////////////////
// This function just specifically draws the jet
void DrawJet(int nShadow)
{
    M3DVector3f vNormal;	// Storeage for calculated surface normal

    // Nose Cone /////////////////////////////
    // Set material color, note we only have to set to black
    // for the shadow once
    if (nShadow == 0)
        glColor3ub(255, 0, 0);
    else
        glColor3ub(100, 0, 0);


    // Nose Cone - Points straight down
    // Set material color
    glBegin(GL_TRIANGLES);
    glNormal3f(0.0f, -1.0f, 0.0f);
    glNormal3f(0.0f, -1.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 60.0f);
    glVertex3f(-15.0f, 0.0f, 30.0f);
    glVertex3f(15.0f, 0.0f, 30.0f);


    // Verticies for this panel
    {
        M3DVector3f vPoints[3] = { { 15.0f, 0.0f,  30.0f},
                                        { 0.0f,  15.0f, 30.0f},
                                        { 0.0f,  0.0f,  60.0f} };

        // Calculate the normal for the plane
        m3dFindNormal(vNormal, vPoints[0], vPoints[1], vPoints[2]);
        glNormal3fv(vNormal);
        glVertex3fv(vPoints[0]);
        glVertex3fv(vPoints[1]);
        glVertex3fv(vPoints[2]);
    }


    {
        M3DVector3f vPoints[3] = { { 0.0f, 0.0f, 60.0f },
                                  { 0.0f, 15.0f, 30.0f },
                                  { -15.0f, 0.0f, 30.0f } };

        m3dFindNormal(vNormal, vPoints[0], vPoints[1], vPoints[2]);
        glNormal3fv(vNormal);
        glVertex3fv(vPoints[0]);
        glVertex3fv(vPoints[1]);
        glVertex3fv(vPoints[2]);
    }


    // Body of the Plane ////////////////////////
    {
        M3DVector3f vPoints[3] = { { -15.0f, 0.0f, 30.0f },
                                  { 0.0f, 15.0f, 30.0f },
                                  { 0.0f, 0.0f, -56.0f } };

        m3dFindNormal(vNormal, vPoints[0], vPoints[1], vPoints[2]);
        glNormal3fv(vNormal);
        glVertex3fv(vPoints[0]);
        glVertex3fv(vPoints[1]);
        glVertex3fv(vPoints[2]);
    }

    {
        M3DVector3f vPoints[3] = { { 0.0f, 0.0f, -56.0f },
                                  { 0.0f, 15.0f, 30.0f },
                                  { 15.0f,0.0f,30.0f } };

        m3dFindNormal(vNormal, vPoints[0], vPoints[1], vPoints[2]);
        glNormal3fv(vNormal);
        glVertex3fv(vPoints[0]);
        glVertex3fv(vPoints[1]);
        glVertex3fv(vPoints[2]);
    }


    glNormal3f(0.0f, -1.0f, 0.0f);
    glVertex3f(15.0f, 0.0f, 30.0f);
    glVertex3f(-15.0f, 0.0f, 30.0f);
    glVertex3f(0.0f, 0.0f, -56.0f);

    ///////////////////////////////////////////////
    // Left wing
    // Large triangle for bottom of wing
    {
        M3DVector3f vPoints[3] = { { 0.0f,2.0f,27.0f },
                                  { -60.0f, 2.0f, -8.0f },
                                  { 60.0f, 2.0f, -8.0f } };

        m3dFindNormal(vNormal, vPoints[0], vPoints[1], vPoints[2]);
        glNormal3fv(vNormal);
        glVertex3fv(vPoints[0]);
        glVertex3fv(vPoints[1]);
        glVertex3fv(vPoints[2]);
    }


    {
        M3DVector3f vPoints[3] = { { 60.0f, 2.0f, -8.0f},
                                   {0.0f, 7.0f, -8.0f},
                                   {0.0f,2.0f,27.0f } };

        m3dFindNormal(vNormal, vPoints[0], vPoints[1], vPoints[2]);
        glNormal3fv(vNormal);
        glVertex3fv(vPoints[0]);
        glVertex3fv(vPoints[1]);
        glVertex3fv(vPoints[2]);
    }

    {
        M3DVector3f vPoints[3] = { {60.0f, 2.0f, -8.0f},
                    {-60.0f, 2.0f, -8.0f},
                    {0.0f,7.0f,-8.0f } };

        m3dFindNormal(vNormal, vPoints[0], vPoints[1], vPoints[2]);
        glNormal3fv(vNormal);
        glVertex3fv(vPoints[0]);
        glVertex3fv(vPoints[1]);
        glVertex3fv(vPoints[2]);
    }

    {
        M3DVector3f vPoints[3] = { {0.0f,2.0f,27.0f},
                                    {0.0f, 7.0f, -8.0f},
                                    {-60.0f, 2.0f, -8.0f} };

        m3dFindNormal(vNormal, vPoints[0], vPoints[1], vPoints[2]);
        glNormal3fv(vNormal);
        glVertex3fv(vPoints[0]);
        glVertex3fv(vPoints[1]);
        glVertex3fv(vPoints[2]);
    }


    // Tail section///////////////////////////////
    // Bottom of back fin
    glNormal3f(0.0f, -1.0f, 0.0f);
    glVertex3f(-30.0f, -0.50f, -57.0f);
    glVertex3f(30.0f, -0.50f, -57.0f);
    glVertex3f(0.0f, -0.50f, -40.0f);

    {
        M3DVector3f vPoints[3] = { { 0.0f,-0.5f,-40.0f },
                                  {30.0f, -0.5f, -57.0f},
                                {0.0f, 4.0f, -57.0f } };

        m3dFindNormal(vNormal, vPoints[0], vPoints[1], vPoints[2]);
        glNormal3fv(vNormal);
        glVertex3fv(vPoints[0]);
        glVertex3fv(vPoints[1]);
        glVertex3fv(vPoints[2]);
    }


    {
        M3DVector3f vPoints[3] = { { 0.0f, 4.0f, -57.0f },
                { -30.0f, -0.5f, -57.0f },
                { 0.0f,-0.5f,-40.0f } };

        m3dFindNormal(vNormal, vPoints[0], vPoints[1], vPoints[2]);
        glNormal3fv(vNormal);
        glVertex3fv(vPoints[0]);
        glVertex3fv(vPoints[1]);
        glVertex3fv(vPoints[2]);
    }

    {
        M3DVector3f vPoints[3] = { { 30.0f,-0.5f,-57.0f },
                    { -30.0f, -0.5f, -57.0f },
                    { 0.0f, 4.0f, -57.0f } };

        m3dFindNormal(vNormal, vPoints[0], vPoints[1], vPoints[2]);
        glNormal3fv(vNormal);
        glVertex3fv(vPoints[0]);
        glVertex3fv(vPoints[1]);
        glVertex3fv(vPoints[2]);
    }

    {
        M3DVector3f vPoints[3] = { { 0.0f,0.5f,-40.0f },
                    { 3.0f, 0.5f, -57.0f },
                    { 0.0f, 25.0f, -65.0f } };

        m3dFindNormal(vNormal, vPoints[0], vPoints[1], vPoints[2]);
        glNormal3fv(vNormal);
        glVertex3fv(vPoints[0]);
        glVertex3fv(vPoints[1]);
        glVertex3fv(vPoints[2]);
    }


    {
        M3DVector3f vPoints[3] = { { 0.0f, 25.0f, -65.0f },
                    { -3.0f, 0.5f, -57.0f},
                    { 0.0f,0.5f,-40.0f } };

        m3dFindNormal(vNormal, vPoints[0], vPoints[1], vPoints[2]);
        glNormal3fv(vNormal);
        glVertex3fv(vPoints[0]);
        glVertex3fv(vPoints[1]);
        glVertex3fv(vPoints[2]);
    }

    {
        M3DVector3f vPoints[3] = { { 3.0f,0.5f,-57.0f },
                    { -3.0f, 0.5f, -57.0f },
                    { 0.0f, 25.0f, -65.0f } };

        m3dFindNormal(vNormal, vPoints[0], vPoints[1], vPoints[2]);
        glNormal3fv(vNormal);
        glVertex3fv(vPoints[0]);
        glVertex3fv(vPoints[1]);
        glVertex3fv(vPoints[2]);
    }


    glEnd();
}

// Called to draw scene
void RenderScene(void)
{
    // Clear the window with current clearing color
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Draw the ground, we do manual shading to a darker green
    // in the background to give the illusion of depth
    glBegin(GL_QUADS);
    glColor3ub(0, 32, 0); //light green ground
    glVertex3f(400.0f, -150.0f, -200.0f);
    glVertex3f(-400.0f, -150.0f, -200.0f);
    glColor3ub(0, 255, 0);  // make it in green gradient
    glVertex3f(-400.0f, -150.0f, 200.0f);
    glVertex3f(400.0f, -150.0f, 200.0f);
    glEnd();

    // Save the matrix state and do the rotations
    glPushMatrix();

    // Draw jet at new orientation, put light in correct position
    // before rotating the jet
    glEnable(GL_LIGHTING);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPoss[activeLight]);
    glRotatef(xRot, 1.0f, 0.0f, 0.0f);
    glRotatef(yRot, 0.0f, 1.0f, 0.0f);

    DrawJet(0);

    // Restore original matrix state
    glPopMatrix();


    // Get ready to draw the shadow and the ground
    // First disable lighting and save the projection state
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glPushMatrix();

    // Multiply by shadow projection matrix
    glMultMatrixf((GLfloat*)shadowMat);

    // Now rotate the jet around in the new flattend space
    glRotatef(xRot, 1.0f, 0.0f, 0.0f);
    glRotatef(yRot, 0.0f, 1.0f, 0.0f);

    // Pass true to indicate drawing shadow
    DrawJet(1);

    // Restore the projection to normal
    glPopMatrix();

    // Draw the light source
    glPushMatrix();
    glTranslatef(lightPoss[activeLight][0], lightPoss[activeLight][1], lightPoss[activeLight][2]);
    glColor3ub(255, 255, 0);
    glutSolidSphere(5.0f, 10, 10);
    glPopMatrix();

    // Restore lighting state variables
    glEnable(GL_DEPTH_TEST);

    // Display the results
    glutSwapBuffers();
}

// This function does any needed initialization on the rendering
// context. 
void SetupRC()
{
    // Any three points on the ground (counter clockwise order)
    M3DVector3f points[3] = { { -30.0f, -149.0f, -20.0f },
                            { -30.0f, -149.0f, 20.0f },
                            { 40.0f, -149.0f, 20.0f } };

    glEnable(GL_DEPTH_TEST);	// Hidden surface removal
    glFrontFace(GL_CCW);		// Counter clock-wise polygons face out
    glEnable(GL_CULL_FACE);		// Do not calculate inside of jet

    // Setup and enable light 0
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT0, GL_AMBIENT, diffuseLight);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPoss[activeLight]);
    glEnable(GL_LIGHT0);

    // Enable color tracking
    glEnable(GL_COLOR_MATERIAL);

    // Set Material properties to follow glColor values
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    // All materials hereafter have full specular reflectivity
    // with a high shine
    glMaterialfv(GL_FRONT, GL_SPECULAR, specref);
    glMateriali(GL_FRONT, GL_SHININESS, 128);

    // Light blue background
    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);

    // Get the plane equation from three points on the ground
    M3DVector4f vPlaneEquation;
    m3dGetPlaneEquation(vPlaneEquation, points[0], points[1], points[2]);

    // Calculate projection matrix to draw shadow on the ground
    m3dMakePlanarShadowMatrix(shadowMat, vPlaneEquation, lightPoss[activeLight]);

    glEnable(GL_NORMALIZE);
}

void SpecialKeys(int key, int x, int y)
{
    if (key == GLUT_KEY_UP)
        xRot -= 5.0f;

    if (key == GLUT_KEY_DOWN)
        xRot += 5.0f;

    if (key == GLUT_KEY_LEFT)
        yRot -= 5.0f;

    if (key == GLUT_KEY_RIGHT)
        yRot += 5.0f;

    if (key > 356.0f)
        xRot = 0.0f;

    if (key < -1.0f)
        xRot = 355.0f;

    if (key > 356.0f)
        yRot = 0.0f;

    if (key < -1.0f)
        yRot = 355.0f;

    if (key == GLUT_KEY_F1) {
        glutDisplayFunc(RenderScene);
        SetupRC();
        activeLight = (activeLight + 1) % 4;
    }




    // Refresh the Window
    glutPostRedisplay();
}


void ChangeSize(int w, int h)
{
    GLfloat fAspect;

    // Prevent a divide by zero
    if (h == 0)
        h = 1;


    // Set Viewport to window dimensions
    glViewport(0, 0, w, h);

    // Reset coordinate system
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    fAspect = (GLfloat)w / (GLfloat)h;
    gluPerspective(60.0f, fAspect, 200.0, 500.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Move out Z axis so we can see everything
    glTranslatef(0.0f, 0.0f, -400.0f);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPoss[activeLight]);
}

int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Shadow");
    glutReshapeFunc(ChangeSize);
    glutSpecialFunc(SpecialKeys);
    glutDisplayFunc(RenderScene);
    SetupRC();
    glutMainLoop();

    return 0;
}


*/