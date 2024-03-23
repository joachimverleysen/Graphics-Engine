//
// Created by joachimverleysen on 3/6/24.
//

#ifndef MYENGINE_FIGURE_H
#define MYENGINE_FIGURE_H
#include "LineDrawer.h"
#include "vector3d.h"
#include <vector>

class Face {
public:
    vector<int> point_indexes;
};

class Figure {
    vector<Vector3D> points;
    vector<Face> faces; // vlakken
    Color color;
    int size;
    double scale;
    int rotateX;
    int rotateY;
    int rotateZ;
    Vector3D center;
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

    int getRotateX() const;

    void setRotateX(int rotateX);

    int getRotateY() const;

    void setRotateY(int rotateY);

    int getRotateZ() const;

    void setRotateZ(int rotateZ);

    const Vector3D &getCenter() const;

    void setCenter(const Vector3D &center);


};

typedef std::list<Figure> Figures3D;


#endif //MYENGINE_FIGURE_H
