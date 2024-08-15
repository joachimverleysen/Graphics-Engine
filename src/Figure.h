//
// Created by joachimverleysen on 3/6/24.
//

#ifndef MYENGINE_FIGURE_H
#define MYENGINE_FIGURE_H
#include "tools/vector3d.h"
#include "Color.h"
#include "LineDrawer.h"
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
    Color ambientReflection;
    Color diffuseReflection;
    int size = 768;
    double scale=1.0;
    double rotateX=0;
    double rotateY=0;
    double rotateZ=0;

    int nrIterations;
    double fractalscale;
    std::string type;
    std::string name;


public:
    const std::string &getName() const;

    void setName(const std::string &name);

public:
    const std::string &getType() const;

    void setType(const std::string &type);

public:
    double getFractalscale() const;

    void setFractalscale(double fractalscale);

private:

    Vector3D center = Vector3D::point(0,0,0);


public:
    int getNrIterations() const;

    void setNrIterations(int nrIterations);

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

    const Color &getAmbientReflection() const;

    void setAmbientReflection(const Color &ammbientReflection);

    const Color &getDiffuseReflection() const;

    void setDiffuseReflection(const Color &diffuseReflection);


    static void triangulateFigure(Figure &fig);

    static Lines2D computeLines(Figure &fig);

    static Lines2D getLineArray2D(const vector<Point2D> &points, Color col);
};

typedef std::list<Figure> Figures3D;


#endif //MYENGINE_FIGURE_H
