//
// Created by joachimverleysen on 3/6/24.
//

#ifndef MYENGINE_FIGURE_H
#define MYENGINE_FIGURE_H
#include "MyTools.h"
#include "../tools/vector3d.h"
#include <vector>
using std::vector;

class Face {
public:
    vector<int> point_indexes;
public:
    Face(vector<int> indexes) : point_indexes(indexes) {};

    Face() {};

};

class Figure {
    vector<Vector3D> points;
    vector<Face> faces; // vlakken
    Color color = Color(1,1,1);
    Color ammbientReflection;
    int size = 768;
    double scale=1.0;
    double rotateX=0;
    double rotateY=0;
    double rotateZ=0;
    Vector3D center = Vector3D::point(0,0,0);
public:

    vector<Point2D> proj_points;

    const vector<Point2D> &getProjPoints() const;

    void setProjPoints(const vector<Point2D> &projPoints);

    const vector<Vector3D> &getPoints() const;

    void setPoints(const vector<Vector3D> &points);

    const vector<Face> &getFaces() const;

    void setFaces(const vector<Face> &faces);

    const Color &getColor() const;

    void setColor(const Color &color);

    int getSize() const;

    void setSize(int size);

    double getScale() const;

    void setScale(double scale);

    double getRotateX() const;

    void setRotateX(double rotateX);

    double getRotateY() const;

    void setRotateY(double rotateY);

    double getRotateZ() const;

    void setRotateZ(double rotateZ);

    const Vector3D &getCenter() const;

    void setCenter(const Vector3D &center);

    const Color &getAmmbientReflection() const;

    void setAmmbientReflection(const Color &ammbientReflection);


};

typedef std::list<Figure> Figures3D;


#endif //MYENGINE_FIGURE_H
