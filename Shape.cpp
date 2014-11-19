

#include <iostream>
#include <math.h>

#include "Shape.h"

#include <Eigen/Dense>

#ifdef OSX
#include <GLUT/glut.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#include <GL/glu.h>
#endif


using namespace std;
using namespace Eigen;



Triangle::Triangle(const Vector3d& inVertexA, const Vector3d& inVertexB, const Vector3d& inVertexC) {
    vertexA = inVertexA;
    vertexB = inVertexB;
    vertexC = inVertexC;
}


void Triangle::draw() const {
    glBegin(GL_TRIANGLES);
        glVertex3d(vertexA[0], vertexA[1], vertexA[2]);
        glVertex3d(vertexB[0], vertexB[1], vertexB[2]);
        glVertex3d(vertexC[0], vertexC[1], vertexC[2]);
    glEnd();
}



Quad::Quad(const Vector3d& inVertexA, const Vector3d& inVertexB, const Vector3d& inVertexC, const Vector3d& inVertexD) {
    vertexA = inVertexA;
    vertexB = inVertexB;
    vertexC = inVertexC;
    vertexD = inVertexD;
}


void Quad::draw() const {
    glBegin(GL_QUADS);
        glVertex3d(vertexA[0], vertexA[1], vertexA[2]);
        glVertex3d(vertexB[0], vertexB[1], vertexB[2]);
        glVertex3d(vertexC[0], vertexC[1], vertexC[2]);
        glVertex3d(vertexD[0], vertexD[1], vertexD[2]);
    glEnd();
}