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



using namespace std;

class Color {
public:
    Color() : red(0), green(0), blue(0) {}
    Color(double r, double g, double b) : red(r*255), green(g*255), blue(b*255) {};
    // RRB values should be percentages (between 0 and 1)
    double red;
    double green;
    double blue;
};

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
    double Xmin=0, Ymin=0, Xmax=0, Ymax=0;
    double xRange, yRange;
    double imgX, imgY;
    double d;
    double DCx, DCy, dx, dy;
    int width, height;
};

class LineDrawer {
public:

    void setBackground(img::EasyImage &image, Color &bgColor);

    void drawLine2D(img::EasyImage &image, Point2D &pt1, Point2D &pt2, Color &lineColor);

    vector<Point2D> getPointArray (const Lines2D &lines);

    img::EasyImage draw2Dlines(Lines2D &lines, const int size, Color &bgColor);;

    void draw_zbuf_line(ZBuffer &zbuffer, img::EasyImage &image, Point2D &pt1, Point2D &pt2, Color &color);

    void draw_zbuf_triag(ZBuffer& zbuffer, img::EasyImage& img,
                         Vector3D const& A,
                         Vector3D const& B,
                         Vector3D const& C,
                         double d,
                         double dx,
                         double dy,
                         Color color);


    void computeDims(Dimensions &dims, Lines2D &lines, const int size);

};


#endif //MYENGINE_LINEDRAWER_H
