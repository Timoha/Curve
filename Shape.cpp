

#include <iostream>
#include <math.h>

#include <Eigen/Dense>

#ifdef OSX
#include <GLUT/glut.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#include <GL/glu.h>
#endif

#include "Shape.h"

using namespace std;
using namespace Eigen;


Triangle::Triangle(const Vector3d& inVertexA, const Vector3d& inVertexB, const Vector3d& inVertexC) {
    vertexA = inVertexA;
    vertexB = inVertexB;
    vertexC = inVertexC;

    Vector3d ab = vertexA - vertexB;
    Vector3d ac = vertexA - vertexC;
    Vector3d n = ab.cross(ac);

    normalA = n;
    normalB = n;
    normalC = n;
}


Triangle::Triangle(const Vector3d& inVertexA, const Vector3d& inVertexB, const Vector3d& inVertexC,
                   const Vector3d& inNormalA, const Vector3d& inNormalB, const Vector3d& inNormalC) {
    vertexA = inVertexA;
    vertexB = inVertexB;
    vertexC = inVertexC;
    normalA = inNormalA;
    normalB = inNormalB;
    normalC = inNormalC;
}


void Triangle::draw() const {
    glBegin(GL_TRIANGLES);
        glNormal3d(normalA[0], normalA[1], normalA[2]);
        glVertex3d(vertexA[0], vertexA[1], vertexA[2]);

        glNormal3d(normalB[0], normalB[1], normalB[2]);
        glVertex3d(vertexB[0], vertexB[1], vertexB[2]);

        glNormal3d(normalC[0], normalC[1], normalC[2]);
        glVertex3d(vertexC[0], vertexC[1], vertexC[2]);
    glEnd();
}


void Triangle::drawNormal() const {
    Vector3d normalLine = vertexA + 0.3 * normalA;

    glBegin(GL_LINES);
        glVertex3d(normalLine[0], normalLine[1], normalLine[2]);
        glVertex3d(vertexA[0], vertexA[1], vertexA[2]);
    glEnd();
}



Quad::Quad(const Vector3d& inVertexA, const Vector3d& inVertexB, const Vector3d& inVertexC, const Vector3d& inVertexD) {
    vertexA = inVertexA;
    vertexB = inVertexB;
    vertexC = inVertexC;
    vertexD = inVertexD;

    Vector3d ab = vertexA - vertexB;
    Vector3d ac = vertexA - vertexC;
    Vector3d n = ab.cross(ac);

    normalA = n;
    normalB = n;
    normalC = n;
    normalD = n;
}


Quad::Quad(const Vector3d& inVertexA, const Vector3d& inVertexB, const Vector3d& inVertexC, const Vector3d& inVertexD,
           const Vector3d& inNormalA, const Vector3d& inNormalB, const Vector3d& inNormalC, const Vector3d& inNormalD) {
    vertexA = inVertexA;
    vertexB = inVertexB;
    vertexC = inVertexC;
    vertexD = inVertexD;
    normalA = inNormalA;
    normalB = inNormalB;
    normalC = inNormalC;
    normalD = inNormalD;
}


void Quad::draw() const {
    glBegin(GL_QUADS);

        glNormal3d(normalA[0], normalA[1], normalA[2]);
        glVertex3d(vertexA[0], vertexA[1], vertexA[2]);

        glNormal3d(normalB[0], normalB[1], normalB[2]);
        glVertex3d(vertexB[0], vertexB[1], vertexB[2]);

        glNormal3d(normalC[0], normalC[1], normalC[2]);
        glVertex3d(vertexC[0], vertexC[1], vertexC[2]);

        glNormal3d(normalD[0], normalD[1], normalD[2]);
        glVertex3d(vertexD[0], vertexD[1], vertexD[2]);
    glEnd();
}


void Quad::drawNormal() const {
    Vector3d normalLine = vertexA + 0.3 * normalA;

    glBegin(GL_LINES);
        glVertex3d(normalLine[0], normalLine[1], normalLine[2]);
        glVertex3d(vertexA[0], vertexA[1], vertexA[2]);
    glEnd();
}