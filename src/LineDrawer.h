//
// Created by joachimverleysen on 2/27/24.
//

#ifndef MYENGINE_LINEDRAWER_H
#define MYENGINE_LINEDRAWER_H


#include "../tools/easy_image.h"
#include "ZBuffer.h"
#include "../tools/vector3d.h"
#include <list>
#include <cmath>
#include "Light.h"
#include "Color.h"



using namespace std;




class Point2D {
public:
    Point2D(double x, double y) : x(x), y(y) {};
    double x;
    double y;

    ///@brief z-value of the original 3D-point
    double z_val;
};

class Line2D {
public:
    Line2D(Point2D p1, Point2D p2, Color col) : p1(p1), p2(p2), color(col) {};
    Point2D p1;
    Point2D p2;
    double z1;
    double z2;
    Color color;
};

using Lines2D = std::list<Line2D>;

struct Dimensions {
    double infty = std::numeric_limits<double>::infinity();

    double Xmin=infty, Ymin=infty, Xmax=-infty, Ymax=-infty;
    double xRange, yRange;
    double imgX, imgY;
    double d;
    double DCx, DCy, dx, dy;
    int width, height;
};


struct ZBuffData {
    double one_on_zG;
    double xG;
    double yG;
    double dzdx;
    double dzdy;
    Vector3D w;
};



class LineDrawer {
public:

    void setBackground(img::EasyImage &image, Color &bgColor);

    vector<Point2D> getPointArray (const Lines2D &lines);

    img::EasyImage draw2Dlines(Lines2D &lines, const int size, Color &bgColor);;

    void draw_zbuf_line(ZBuffer &zbuffer, img::EasyImage &image, Point2D &pt1, Point2D &pt2, Color &color,
                        ZBuffData zbd);

    void drawZbuffTriangLighted(ZBuffer &zbuffer, img::EasyImage &img, const Vector3D &A, const Vector3D &B,
                                const Vector3D &C,
                                double d, double dx, double dy, Color color, Color ambienReflection,
                                Color diffuseReflection,
                                double reflectionCoeff, Lights3D &lights);


    Dimensions computeDims(Lines2D &lines, const int size);

    void rescalePoint2D(Point2D &p, double d, double dx, double dy);

    void drawZbuffTriang(ZBuffer &zbuffer, img::EasyImage &img, const Vector3D &A, const Vector3D &B, const Vector3D &C,
                         double d, double dx, double dy, Color color);

};


#endif //MYENGINE_LINEDRAWER_H
