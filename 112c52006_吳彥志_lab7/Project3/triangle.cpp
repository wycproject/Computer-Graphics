#include <stdio.h> 
#include <stdlib.h>
#include "C:\OpenglLib\freeglut\include\GL\freeglut.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm> 

struct Vertex {
    GLfloat x, y, z;
};

struct TextureCoord {
    GLfloat u, v;
};

struct Normal {
    GLfloat nx, ny, nz;
};

struct Face {
    std::vector<int> vertexIndices;
    std::vector<int> textureIndices;
    std::vector<int> normalIndices;
};

std::vector<Vertex> vertices;
std::vector<TextureCoord> textureCoords;
std::vector<Normal> normals;
std::vector<Face> faces;

void parseOBJ(const char* filename);
void display();
void reshape(int w, int h);
void renderModel();

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("OBJ Viewer");

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    glEnable(GL_DEPTH_TEST);

    parseOBJ("C:\\OpenglLib\\OBJs4\\octahedron.obj");

    glutMainLoop();
    return 0;
}

void parseOBJ(const char* filename) {
    std::ifstream file(filename);
    std::string line;
    while (getline(file, line)) {
        std::istringstream iss(line);
        std::string prefix;
        iss >> prefix;
        if (prefix == "v") {
            Vertex v;
            iss >> v.x >> v.y >> v.z;
            vertices.push_back(v);
        }
        else if (prefix == "vt") {
            TextureCoord tc;
            iss >> tc.u >> tc.v;
            textureCoords.push_back(tc);
        }
        else if (prefix == "vn") {
            Normal n;
            iss >> n.nx >> n.ny >> n.nz;
            normals.push_back(n);
        }
        else if (prefix == "f") {
            Face f;
            std::string vertex;
            while (iss >> vertex) {
                std::replace(vertex.begin(), vertex.end(), '/', ' ');
                std::istringstream vertexStream(vertex);
                int vIndex, tIndex, nIndex;
                vertexStream >> vIndex >> tIndex >> nIndex;
                f.vertexIndices.push_back(vIndex - 1);
                f.textureIndices.push_back(tIndex - 1);
                f.normalIndices.push_back(nIndex - 1);
            }
            faces.push_back(f);
        }
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    renderModel();
    glutSwapBuffers();
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (GLfloat)w / (GLfloat)h, 1.0, 200.0);
    glMatrixMode(GL_MODELVIEW);
}

void renderModel() {
    glBegin(GL_TRIANGLES);
    for (const auto& face : faces) {
        for (size_t i = 0; i < face.vertexIndices.size(); ++i) {
            if (!normals.empty()) glNormal3f(normals[face.normalIndices[i]].nx, normals[face.normalIndices[i]].ny, normals[face.normalIndices[i]].nz);
            if (!textureCoords.empty()) glTexCoord2f(textureCoords[face.textureIndices[i]].u, textureCoords[face.textureIndices[i]].v);
            glVertex3f(vertices[face.vertexIndices[i]].x, vertices[face.vertexIndices[i]].y, vertices[face.vertexIndices[i]].z);
        }
    }
    glEnd();
}
