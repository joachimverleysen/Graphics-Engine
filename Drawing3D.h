//
// Created by joachimverleysen on 3/15/24.
//

#ifndef MYENGINE_DRAWING3D_H
#define MYENGINE_DRAWING3D_H

#include "LineDrawer.h"
#include "Figure.h"
#include "vector3d.h"
#include <string>
#include <vector>
#include <iostream>
using namespace std;

class Drawing3D {
int size = 0;
string type;
Color bgColor=Color(0,0,0);
int nrFigures=0;
Vector3D eye=Vector3D::point(0,0,0);
Figures3D figures;

public:
    Drawing3D() {};

    Drawing3D(int size, const string &type, int nrFigures) :
    size(size), type(type), nrFigures(nrFigures) {};

    void addFigure(Figure &fig);

    // Generates 2D-line list (=projection)
    Lines2D compute2DLines();

    Figure do_fig_projection(const Figure &fig);

    int getSize() const;

    void setSize(int size);

    const string &getType() const;

    void setType(const string &type);

    const Color &getBgColor() const;

    void setBgColor(const Color &bgColor);

    int getNrFigures() const;

    void setNrFigures(int nrFigures);

    const Vector3D &getEye() const;

    void setEye(const Vector3D &point);

    const Figures3D &getFigures() const;

    void setFigures(const Figures3D &figures);

};


#endif //MYENGINE_DRAWING3D_H
