#ifndef SHAPE_H
#define SHAPE_H

#include <iostream>
#include <math.h>

#include <Eigen/Dense>


using namespace std;
using namespace Eigen;


class Shape
{
public:
    Shape() {};
    virtual ~Shape() = 0;
    virtual void draw() const = 0;
};


class Triangle : public Shape
{
public:
    Triangle();
    Triangle(const Vector3d& inVertexA, const Vector3d& inVertexB, const Vector3d& inVertexC);
    void draw() const;
private:
    Vector3d vertexA, vertexB, vertexC;
};


class Quad : public Shape
{
public:
    Quad();
    Quad(const Vector3d& inVertexA, const Vector3d& inVertexB, const Vector3d& inVertexC, const Vector3d& inVertexD);
    void draw() const;
private:
    Vector3d vertexA, vertexB, vertexC, vertexD;
};


#endif