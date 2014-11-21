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
    ~Shape() {}
    // virtual ~Shape() = 0;
    virtual void draw() const = 0;
    virtual void drawNormal() const = 0;
};


class Triangle : public Shape
{
public:
    Triangle();
    Triangle(const Vector3d& inVertexA, const Vector3d& inVertexB, const Vector3d& inVertexC);
    Triangle(const Vector3d& inVertexA, const Vector3d& inVertexB, const Vector3d& inVertexC,
             const Vector3d& inNormalA, const Vector3d& inNormalB, const Vector3d& inNormalC);
    void draw() const;
    void drawNormal() const;
private:
    Vector3d vertexA, vertexB, vertexC;
    Vector3d normalA, normalB, normalC;
};



class Quad : public Shape
{
public:
    Quad();
    Quad(const Vector3d& inVertexA, const Vector3d& inVertexB, const Vector3d& inVertexC, const Vector3d& inVertexD);
    Quad(const Vector3d& inVertexA, const Vector3d& inVertexB, const Vector3d& inVertexC, const Vector3d& inVertexD,
         const Vector3d& inNormalA, const Vector3d& inNormalB, const Vector3d& inNormalC, const Vector3d& inNormalD);
    void draw() const;
    void drawNormal() const;
private:
    Vector3d vertexA, vertexB, vertexC, vertexD;
    Vector3d normalA, normalB, normalC, normalD;
};



class LocalInfo
{
public:
    Vector3d point;
    Vector3d normal;
    LocalInfo(const Vector3d& inPoint, const Vector3d& inNormal) {
        point = inPoint;
        normal = inNormal;
    }
};




#endif