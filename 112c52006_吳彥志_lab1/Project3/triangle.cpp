
#include <stdio.h> 
#include <stdlib.h>
/*** freeglut***/
#include "C:\OpenglLib\freeglut\include\GL\freeglut.h"

void ChangeSize(int , int );					 
void RenderScene(void);	
void menu(int value);

int main(int argc, char** argv) 
{ 
   glutInit(&argc, argv); 
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB); 
   glutInitWindowSize(400,400);				 
   glutInitWindowPosition(600,80);           
   glutCreateWindow("Simple Triangle");      
    
   glutCreateMenu(menu);
   glutAddMenuEntry("flat", 1);
   glutAddMenuEntry("smooth", 2);
   glutAttachMenu(GLUT_RIGHT_BUTTON);
    
   glutReshapeFunc(ChangeSize); 
   glutDisplayFunc(RenderScene); 
   glutMainLoop();	//http://www.programmer-club.com.tw/ShowSameTitleN/opengl/2288.html
   return 0; 
} 
void ChangeSize(int w, int h) 
{ 
   printf("Window Size= %d X %d\n",w,h); 
   glViewport(0, 0, w, h);            
   glMatrixMode(GL_PROJECTION); 
   glLoadIdentity(); 
   glOrtho(-10,10,-10,10,-10,10);      
   glMatrixMode(GL_MODELVIEW); 
   glLoadIdentity(); 
} 
void RenderScene(void) 
{ 
   glClearColor(1.0, 1.0, 1.0, 1.0);     
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   gluLookAt(0, 0, 10.0f, 0, 0, 0, 0, 1, 0);

   glBegin(GL_QUADS);
   glColor3f(0.7, 0.7, 1.0); glVertex3f(8.6603, -8.6603, 0);   
   glColor3f(0.7, 0.7, 1.0); glVertex3f(8.6603, 8.6603, 0);    
   glColor3f(0.7, 0.7, 1.0); glVertex3f(-8.6603, 8.6603, 0);   
   glColor3f(0.7, 0.7, 1.0); glVertex3f(-8.6603, -8.6603, 0);  
   glEnd();

   glutSwapBuffers();
}

void menu(int value)
{
    switch (value) {
    case 1:
        // 執行 Option 1 的功能
        printf("flat selected!\n");
        
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt(0, 0, 10.0f, 0, 0, 0, 0, 1, 0);

        glBegin(GL_QUADS);
        glColor3f(0.7, 0.7, 1.0); glVertex3f(8.6603, -8.6603, 0);   
        glColor3f(0.7, 0.7, 1.0); glVertex3f(8.6603, 8.6603, 0);    
        glColor3f(0.7, 0.7, 1.0); glVertex3f(-8.6603, 8.6603, 0);   
        glColor3f(0.7, 0.7, 1.0); glVertex3f(-8.6603, -8.6603, 0);  
        glEnd();

        glutSwapBuffers();
        
        break;
    case 2:
        // 執行 Option 2 的功能
       
            glClearColor(1.0, 1.0, 1.0, 1.0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            gluLookAt(0, 0, 10.0f, 0, 0, 0, 0, 1, 0);
            glBegin(GL_QUADS);
                        

            glColor3f(0, 0, 1); glVertex3f(8.6603, -8.6603, 0);   
            glColor3f(1, 0, 0); glVertex3f(8.6603, 8.6603, 0);    
            glColor3f(0, 1, 0); glVertex3f(-8.6603, 8.6603, 0);   
            glColor3f(0.7, 0.7, 1.0); glVertex3f(-8.6603, -8.6603, 0);
            glEnd();

            glEnd();
            glutSwapBuffers();
        
        printf("smooth selected!\n");
        break;
    default:
        break;
    }
}