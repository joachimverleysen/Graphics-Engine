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
    Point2D _do_point_projection(const Vector3D &point, const double d);

    void doTransitions(Figure &fig);

    Matrix get_translate_mtx(const Vector3D &vector);

    Matrix get_scale_mtx(const double scale);

    Matrix get_rot_z(const double angle_deg);

    Matrix get_rot_x(const double angle_deg);

    Matrix get_rot_y(const double angle_deg);

    Lines2D computeLines(Figure &fig);

    void convert_fig_to_eyesys(Figure &fig, const Vector3D &eye);

    Lines2D getLineArray(const vector<Point2D> &points, Color col);

    Matrix eyePointTrans(const Vector3D &eyepoint);

    polars toPolar(const Vector3D &point);

    vector<Point2D> compute_proj_points(Figure &fig);

    void triangulateFigure(Figure &fig);

    void point_to_eyesys(Vector3D &point, const Vector3D &eye);
};


#endif //MYENGINE_MYTOOLS_H
