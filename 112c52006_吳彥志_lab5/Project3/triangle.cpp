#include <stdio.h> 
#include <stdlib.h>
/*** freeglut***/
#include "C:\OpenglLib\freeglut\include\GL\freeglut.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>
#include <string.h> 


void ChangeSize(int, int);
void RenderScene(void);
void SpecialKeys(int key, int x, int y);
void MouseFunc(int button, int state, int x, int y); // 滑鼠點擊
void DrawPoint(float x, float y); // 繪製標記點


#define DEG2RAD(angle) ((angle) * M_PI / 180.0)

GLfloat angleX = 0.0f;   // 初始角度
GLfloat angleY = 0.0f;
GLfloat angleZ = 0.0f;
GLfloat posX = 0.0f;     // 初始位置
GLfloat posY = 0.0f;
GLfloat posZ = 0.0f;
GLfloat scaleX = 1.0f;
GLfloat scaleY = 1.0f;
GLfloat scaleZ = 1.0f;

GLfloat angleN = 0.0f;
GLfloat clickedX = 0.0f; // 紀錄點擊點的位置
GLfloat clickedY = 0.0f;
GLboolean isClicked = GL_FALSE; // 紀錄是否有點擊事件發生

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GL_RGB | GL_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(400, 400);
    glutInitWindowPosition(600, 80);
    glutCreateWindow("Simple Triangle");

    glEnable(GL_DEPTH_TEST);

    glutReshapeFunc(ChangeSize);
    glutDisplayFunc(RenderScene);
    glutSpecialFunc(SpecialKeys);  // 註冊特殊鍵盤事件
    glutMouseFunc(MouseFunc); // 註冊滑鼠點擊事件
    

    glutMainLoop();	//http://www.programmer-club.com.tw/ShowSameTitleN/opengl/2288.html



    return 0;
}
void ChangeSize(int w, int h)
{
    printf("Window Size= %d X %d\n", w, h);
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-10, 10, -10, 10, -20, 20);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, 0, 10.0f, 0, 0, 0, 0, 1, 0);

}



void RenderScene(void)
{
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



    glPushMatrix();

    // 移動和旋轉
    //glTranslatef(posX, posY, posZ);
    //glRotatef(angleX, 1.0, 0.0, 0.0);
    //glRotatef(angleY, 0.0, 1.0, 0.0);
    //glRotatef(angleZ, 0.0, 0.0, 1.0);
    
    float cosX = cos(angleX);
    float sinX = sin(angleX);
    float cosY = cos(angleY);
    float sinY = sin(angleY);
    float cosZ = cos(angleZ);
    float sinZ = sin(angleZ);
    float cosN = cos(angleN);
    float sinN = sin(angleN);
    
    GLfloat rotMatrixy[] = { cosY, 0.0, -sinY, 0.0, 0.0, 1.0, 0.0, 0.0, sinY, 0.0, cosY, 0.0, 0.0, 0.0, 0.0, 1.0 };
    GLfloat rotMatrixx[] = { 1,0,0,0,0,cosX,sinX,0,0,-sinX,cosX,0,0,0,0,1 };
    GLfloat rotMatrixz[] = { cosZ,sinZ,0,0,-sinZ,cosZ,0,0,0,0,1,0,0,0,0,1 };
    GLfloat translateMatrix[] = { 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, posX, posY, posZ, 1.0 };
    GLfloat scaleMatrix[] = { scaleX, 0.0, 0.0, 0.0, 0.0, scaleY , 0.0, 0.0, 0.0, 0.0, scaleZ , 0, 0, 0, 0, 1 };
    
    GLfloat rotMatrixN[] = { cosN + (1 - cosN) * clickedX * clickedX, (1 - cosN) * clickedY * clickedX, -sinN * clickedY, 0, (1-cosN) * clickedX * clickedY , cosN + (1-cosN) * clickedY * clickedY , sinN * clickedX, 0, sinN * clickedY, -sinN * clickedX, cosN, 0,0,0,0,1 };

    glMultMatrixf(rotMatrixy);
    glMultMatrixf(rotMatrixx);
    glMultMatrixf(rotMatrixz);
    glMultMatrixf(translateMatrix);
    glMultMatrixf(scaleMatrix);
    glMultMatrixf(rotMatrixN);


    glBegin(GL_TRIANGLES);
    glColor3f(0.7, 0.7, 1.0); glVertex3f(5, 0, 5);
    glColor3f(0.7, 0.7, 1.0); glVertex3f(0, 5, 5);
    glColor3f(0.7, 0.7, 1.0); glVertex3f(-5, 0, 5);

    glBegin(GL_TRIANGLES);
    glColor3f(1, 0, 0); glVertex3f(-5, 0, 0);
    glColor3f(1, 0, 0); glVertex3f(5, 0, 5);
    glColor3f(1, 0, 0); glVertex3f(5, 0, 0);

    glBegin(GL_TRIANGLES);
    glColor3f(1, 0, 0); glVertex3f(-5, 0, 0);
    glColor3f(1, 0, 0); glVertex3f(-5, 0, 5);
    glColor3f(1, 0, 0); glVertex3f(5, 0, 5);

    glBegin(GL_TRIANGLES);
    glColor3f(0.7, 0.7, 0); glVertex3f(5, 0, 0);
    glColor3f(0.7, 0.7, 0); glVertex3f(0, 5, 0);
    glColor3f(0.7, 0.7, 0); glVertex3f(-5, 0, 0);



    glBegin(GL_TRIANGLES);
    glColor3f(0, 0, 1.0); glVertex3f(0, 5, 0);
    glColor3f(0, 0, 1.0); glVertex3f(-5, 0, 5);
    glColor3f(0, 0, 1.0); glVertex3f(0, 5, 5);
    glBegin(GL_TRIANGLES);
    glColor3f(0, 0, 1.0); glVertex3f(0, 5, 0);
    glColor3f(0, 0, 1.0); glVertex3f(-5, 0, 0);
    glColor3f(0, 0, 1.0); glVertex3f(-5, 0, 5);

    glBegin(GL_TRIANGLES);
    glColor3f(0, 1, 0); glVertex3f(5, 0, 0);
    glColor3f(0, 1, 0); glVertex3f(0, 5, 5);
    glColor3f(0, 1, 0); glVertex3f(0, 5, 0);
    glBegin(GL_TRIANGLES);
    glColor3f(0, 1, 0); glVertex3f(5, 0, 5);
    glColor3f(0, 1, 0); glVertex3f(0, 5, 5);
    glColor3f(0, 1, 0); glVertex3f(5, 0, 0);



    glEnd();

    glPopMatrix();
    // 畫XYZ軸線
    glBegin(GL_LINES);
    glColor3f(1.0, 0.0, 0.0);  // X軸紅色
    glVertex3f(-10.0, 0.0, 0.0);
    glVertex3f(10.0, 0.0, 0.0);

    glColor3f(0.0, 1.0, 0.0);  // Y軸綠色
    glVertex3f(0.0, -10.0, 0.0);
    glVertex3f(0.0, 10.0, 0.0);

    glColor3f(0.0, 0.0, 1.0);  // Z軸藍色
    glVertex3f(0.0, 0.0, -10.0);
    glVertex3f(0.0, 0.0, 10.0);
    glEnd();

    
    DrawPoint(clickedX, clickedY);//畫點擊的位置
    glBegin(GL_LINES);

    glColor3f(1.0, 0.0, 0.0);  // 畫點擊的位置連到原點
    glVertex3f(0.0, 0.0, 0.0); //原點
    glVertex3f(clickedX, clickedY, 0.0); //點擊的位置

    glEnd();


    glutSwapBuffers();
}

void SpecialKeys(int key, int x, int y)
{
    switch (key)
    {
    case GLUT_KEY_UP:
        angleX += 0.5f;  // 沿X軸旋轉
        break;
    case GLUT_KEY_DOWN:
        angleX -= 0.5f;
        break;
    case GLUT_KEY_LEFT:
        angleY += 0.5f;  // 沿Y軸旋轉
        break;
    case GLUT_KEY_RIGHT:
        angleY -= 0.5f;
        break;
    case GLUT_KEY_F1:
        angleZ += 0.5f;  // 沿Z軸旋轉
        break;
    case GLUT_KEY_F2:
        angleZ -= 0.5f;
        break;
    case GLUT_KEY_F3:
        posX += 1.0f;  // 沿X軸移動
        break;
    case GLUT_KEY_F4:
        posX -= 1.0f;
        break;
    case GLUT_KEY_F5:
        posY += 1.0f;  // 沿Y軸移動
        break;
    case GLUT_KEY_F6:
        posY -= 1.0f;
        break;
    case GLUT_KEY_F7:
        posZ += 1.0f;  // 沿Z軸移動
        break;
    case GLUT_KEY_F8:
        posZ -= 1.0f;
        break;
    case GLUT_KEY_F9:
        posX = posY = posZ = 0.0f;  // 回到原點
        angleX = angleY = angleZ = 0.0f;
        scaleX = scaleY = scaleZ = 1.0f;
        angleN = 0.0f;
        break;
    case GLUT_KEY_F10:
        scaleX += 0.5f;  
        scaleY += 0.5f;  
        scaleZ += 0.5f;  
        break;
    case GLUT_KEY_F11:
        scaleX -= 0.5f;
        scaleY -= 0.5f;
        scaleZ -= 0.5f;
        break;

    case GLUT_KEY_HOME:
        angleN += 0.5f;        
        break;
    
    }
    

    glutPostRedisplay();  // 重新繪製場景
}

void MouseFunc(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        // 紀錄點擊的位置
        clickedX = (x - 200) / 20.0f; // 這裡的200視窗口寬度的一半，假設窗口寬度為400
        
        clickedY = (200 - y) / 20.0f; // 這裡的200視窗口高度的一半，假設窗口高度為400
        
        isClicked = GL_TRUE; // 設置點擊事件標誌為真
    }
}

void DrawPoint(float x, float y)
{
    glPointSize(5.0); // 設置點的大小
    glBegin(GL_POINTS); // 開始繪製點
    glColor3f(1.0, 0.0, 0.0); // 設置點的顏色為紅色
    glVertex3f(x, y, 0.0); // 指定點的位置
    glEnd(); // 結束繪製點
}

void RotateAroundPoint(float centerX, float centerY, float angle, float& x, float& y)
{
    // 将旋转轴心移动到原点
    float tempX = x - centerX;
    float tempY = y - centerY;

    // 计算旋转后的坐标
    float rotatedX = tempX * cos(angle) - tempY * sin(angle);
    float rotatedY = tempX * sin(angle) + tempY * cos(angle);

    // 将旋转轴心移回原来的位置
    x = rotatedX + centerX;
    y = rotatedY + centerY;
}