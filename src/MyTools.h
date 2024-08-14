//
// Created by joachimverleysen on 4/1/24.
//

#ifndef MYENGINE_MYTOOLS_H
#define MYENGINE_MYTOOLS_H


#include "LineDrawer.h"

class Figure;



struct polars {
    double r;
    double theta;
    double phi;
    polars(double r, double theta, double phi) : r(r), theta(theta), phi(phi) {};
};



class MyTools {
public:

    void applyTransformation (Figure &fig, Matrix &mtx);

    static Lines2D computeLines(Figure &fig);

    static Lines2D getLineArray2D(const vector<Point2D> &points, Color col);

    static void triangulateFigure(Figure &fig);

};


#endif //MYENGINE_MYTOOLS_H
