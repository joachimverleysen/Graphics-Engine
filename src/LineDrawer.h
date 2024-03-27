//
// Created by joachimverleysen on 2/27/24.
//

#ifndef MYENGINE_LINEDRAWER_H
#define MYENGINE_LINEDRAWER_H


#include "../tools/easy_image.h"
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
};

class Line2D {
public:
    Line2D(Point2D p1, Point2D p2, Color col) : p1(p1), p2(p2), color(col) {};
    Point2D p1;
    Point2D p2;
    Color color;
};

using Lines2D = std::list<Line2D>;


class LineDrawer {
public:

    void setBackground(img::EasyImage &image, Color &bgColor);

    void drawLine2D(img::EasyImage &image, Point2D &pt1, Point2D &pt2, Color &lineColor);

    vector<Point2D> getPointArray (const Lines2D &lines);

    img::EasyImage draw2Dlines(Lines2D &lines, const int size, Color &bgColor);;


};


#endif //MYENGINE_LINEDRAWER_H
