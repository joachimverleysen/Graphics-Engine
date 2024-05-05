//
// Created by joachimverleysen on 2/27/24.
//

#include "LineDrawer.h"
#include "MyTools.h"
#include <fstream>

void LineDrawer::drawLine2D(img::EasyImage &image, Point2D &pt1, Point2D &pt2, Color &lineColor) {
    unsigned int width = image.get_width();
    unsigned int height = image.get_height();
    double m = static_cast<double>((pt1.y - pt2.y) / static_cast<double>(pt1.x - pt2.x));
    unsigned int Xa = min(pt1.x, pt2.x);
    unsigned int Xb = max(pt1.x, pt2.x);
    unsigned int Ya, Yb;
    if (Xa == pt1.x) {
        Ya = pt1.y;
        Yb = pt2.y;
    }
    else {
        Yb = pt1.y;
        Ya = pt2.y;
    }
     int x_distance = Xa-Xb; x_distance = abs(x_distance);
     int y_distance = Ya-Yb; y_distance = abs(y_distance);

    if (Xa==Xb) {   // Vertical line
        for(unsigned int i = Xa; i<=Xb; i++) {
            for (unsigned int j = lround(min(Ya, Yb)); j <= lround(max(Ya, Yb)); j++) {
                if (i == Xa) {
                    image(i, j).red = lineColor.red;
                    image(i, j).green = lineColor.green;
                    image(i, j).blue = lineColor.blue;
                }
            }
        }
    }
    else if (pt1.y == pt2.y) {   // Horizontal line
        for(unsigned int i = Xa; i <= Xb ; i++) {
            for (unsigned int j = Ya; j <= Yb; j++) {
                if (i<0 || i>=width || j<0||j>=height) {
                    cerr<<"Out of range error\n";
                    exit(1);
                }
                if (j == pt1.y) {
                    image(i, j).red = lineColor.red;
                    image(i, j).green = lineColor.green;
                    image(i, j).blue = lineColor.blue;
                }
            }
        }
    }
    else if (m>0 && m<=1 || m >=-1 && m<0) {     // Cursus: geval 1 en 2
        for(unsigned int i = 0; i <= Xb - Xa; i++) {

            int Xi = Xa + i;
            int Yi = lround(Ya + (m*i));
            if (Xi<0 || Xi>=width || Yi<0 || Yi >= height) {
                cerr<<"Out of range error\n";
                exit(1);
            }
            image(Xi, Yi).red = lineColor.red;
            image(Xi, Yi).green = lineColor.green;
            image(Xi, Yi).blue = lineColor.blue;
        }
    }

    else if (m > 1) {     // Cursus: geval 3
        for(unsigned int i = 0; i <= y_distance; i++) {
            int Xi = lround(Xa + (i / m));
            int Yi = Ya + i;
            if (Xi<0 || Xi>=width || Yi<0 || Yi >= height) {
                cerr<<"Out of range error\n";
                exit(1);
            }
            image(Xi, Yi).red = lineColor.red;
            image(Xi, Yi).green = lineColor.green;
            image(Xi, Yi).blue = lineColor.blue;
        }
    }

    else if (m < -1) {     // Cursus: geval 4
        for(unsigned int i = 0; i <= y_distance; i++) {
            int Xi = lround(Xa - (i / m));
            int Yi = Ya - i;
            if (Xi<0 || Xi>=width || Yi<0 || Yi >= height) {
                cerr<<"Out of range error\n";
                exit(1);
            }
            image(Xi, Yi).red = lineColor.red;
            image(Xi, Yi).green = lineColor.green;
            image(Xi, Yi).blue = lineColor.blue;
        }
    }

}

vector<Point2D> LineDrawer::getPointArray(const Lines2D &lines) {
    vector<Point2D> result;
    for (auto line : lines) {
        result.push_back(line.p1);
        result.push_back(line.p2);
    }
    return result;
}


Dimensions LineDrawer::computeDims(Lines2D &lines, const int size) {
    Dimensions dims;
    vector<Point2D> points = getPointArray(lines);

// Compute max values
    for (auto &p : points) {
        if (p.y < 0) {
        }
        dims.Xmax= max(dims.Xmax, p.x);
        dims.Xmin = min(dims.Xmin, p.x);
        dims.Ymax = max(dims.Ymax, p.y);
        dims.Ymin = min(dims.Ymin, p.y);
    }
    dims.xRange = dims.Xmax-dims.Xmin;
    dims.yRange = dims.Ymax-dims.Ymin;
    dims.imgX = size * (dims.xRange/max(dims.xRange, dims.yRange));
    dims.imgY = size * (dims.yRange / max(dims.xRange, dims.yRange));
    dims.d = 0.95 * (dims.imgX/dims.xRange);    // scale factor

    dims.DCx = dims.d* ((dims.Xmin + dims.Xmax)/2);
    dims.DCy = dims.d* ((dims.Ymin + dims.Ymax)/2);
    dims.dx = (dims.imgX/2) - dims.DCx;
    dims.dy = (dims.imgY/2) - dims.DCy;
    dims.width = lround(dims.imgX);
    dims.height = lround(dims.imgY);
    return dims;
}

void LineDrawer::rescalePoint2D(Point2D& p, double d, double dx, double dy) {
    // scale factor
    p.x *= d;
    p.y *= d;
    // Verschuiven door offset (dx, dy) op te tellen
    p.x += dx;
    p.y += dy;
    // afronden
    p.x = lround(p.x);
    p.y = lround(p.y);

}

img::EasyImage LineDrawer::draw2Dlines(Lines2D &lines, const int size, Color &bgColor) {
    Dimensions dims = computeDims(lines, size);


    img::EasyImage myImage(dims.width, dims.height);
    setBackground(myImage, bgColor);

    for (Line2D &line : lines) {
        rescalePoint2D(line.p1, dims.d, dims.dx, dims.dy);
        rescalePoint2D(line.p2, dims.d, dims.dx, dims.dy);

    }

    ZBuffer zBuffer(dims.width, dims.height);

    cout<<"Drawing "<<lines.size()<<" "<<"lines. Check 'out.bmp'"<<endl;
    for (auto line : lines) {

        //todo: shouldn't be zbuff, change to draw2Dline()
        draw_zbuf_line(zBuffer, myImage, line.p1, line.p2, line.color, false, ZBuffData());
    }

    return myImage;
}

void LineDrawer::setBackground(img::EasyImage &image, Color &color) {
    int width = image.get_width();
    int height = image.get_height();
    for(unsigned int i = 0; i<width; i++) {
        for (unsigned int j = 0; j < height; j++) {
            image(i, j).red = color.red;
            image(i, j).green = color.green;
            image(i, j).blue = color.blue;
        }
    }
}

double compute_1_on_z_WF(const Point2D& p1, const Point2D& p2, double i, double a) {
    double result;
    double p = i/a;
    result = (p/p1.z_val) + ((1-p)/p2.z_val);
    return result;
}

double one_on_z(int x, int y, ZBuffData &zbd) {
    return (zbd.one_on_zG + (x-zbd.xG)*zbd.dzdx + (y-zbd.yG)*zbd.dzdy);
}


void
LineDrawer::draw_zbuf_line(ZBuffer &zbuffer, img::EasyImage &image, Point2D &pt1, Point2D &pt2, Color &color,
                           bool triag_filling, ZBuffData zbd= ZBuffData()) {

    unsigned int width = image.get_width();
    unsigned int height = image.get_height();
    double m = static_cast<double>((pt1.y - pt2.y) / static_cast<double>(pt1.x - pt2.x));
    unsigned int Xa = lround(min(pt1.x, pt2.x));
    unsigned int Xb = lround(max(pt1.x, pt2.x));
    unsigned int Ya, Yb;

    if (Xa == pt1.x) {
        Ya = pt1.y;
        Yb = pt2.y;
    }
    else {
        Yb = pt1.y;
        Ya = pt2.y;
    }
    int x_distance = Xa-Xb; x_distance = abs(x_distance);
    int y_distance = Ya-Yb; y_distance = abs(y_distance);

    int a;
    double z_val;
    a = max(image.get_width(), image.get_height());

    if (Xa==Xb && !triag_filling) {   // Vertical line
//        a=y_distance;
        for(unsigned int i = Xa; i<=Xb; i++) {
            z_val = compute_1_on_z_WF(pt1, pt2, a - i, a);

            for (unsigned int j = lround(min(Ya, Yb)); j <= lround(max(Ya, Yb)); j++) {

                if (i == Xa) {
                        if (z_val>zbuffer[i][j]) continue;
                        zbuffer[i][j] = z_val;
                        image(i, j).red = color.red;
                    image(i, j).green = color.green;
                    image(i, j).blue = color.blue;
                }
            }
        }
    }
    else if (pt1.y == pt2.y) {   // Horizontal line
//        a=x_distance;

        for(unsigned int i = Xa; i <= Xb ; i++) {
            for (unsigned int j = Ya; j <= Yb; j++) {
                z_val = compute_1_on_z_WF(pt1, pt2, a - i, a);
                if (triag_filling) z_val = one_on_z(i, j, zbd);
                if (i>=width || j>=height) {
                    cerr<<"Out of range error\n";
                    exit(1);
                }
                if (j == pt1.y) {
                    if (z_val>zbuffer[i][j]) continue;
                    zbuffer[i][j] = z_val;
                    image(i, j).red = color.red;
                    image(i, j).green = color.green;
                    image(i, j).blue = color.blue;
                }
            }
        }
    }
    else if (m>0 && m<=1 || m >=-1 && m<0) {     // Cursus: geval 1 en 2
//        a=x_distance;

        for(unsigned int i = 0; i <= Xb - Xa; i++) {

            int Xi = Xa + i;
            int Yi = lround(Ya + (m*i));
            z_val = compute_1_on_z_WF(pt1, pt2, a - i, a);
            if (z_val>zbuffer[Xi][Yi]) continue;

            if (Xi<0 || Xi>=width || Yi<0 || Yi >= height) {
                cerr<<"Out of range error\n";
                exit(1);
            }
            zbuffer[Xi][Yi] = z_val;
            image(Xi, Yi).red = color.red;
            image(Xi, Yi).green = color.green;
            image(Xi, Yi).blue = color.blue;
        }
    }

    else if (m > 1) {     // Cursus: geval 3
//        a=y_distance;
        for(unsigned int i = 0; i <= y_distance; i++) {

            int Xi = lround(Xa + (i / m));
            int Yi = Ya + i;
            z_val = compute_1_on_z_WF(pt1, pt2, a - i, a);
            if (triag_filling) z_val = one_on_z(Xi, Yi, zbd);

            if (z_val>zbuffer[Xi][Yi]) continue;
            if (Xi<0 || Xi>=width || Yi<0 || Yi >= height) {
                cerr<<"Out of range error\n";
                exit(1);
            }
            zbuffer[Xi][Yi] = z_val;
            image(Xi, Yi).red = color.red;
            image(Xi, Yi).green = color.green;
            image(Xi, Yi).blue = color.blue;
        }
    }

    else if (m < -1) {     // Cursus: geval 4
//        a=y_distance;
        for(unsigned int i = 0; i <= y_distance; i++) {
            int Xi = lround(Xa - (i / m));
            int Yi = Ya - i;
            z_val = compute_1_on_z_WF(pt1, pt2, a - i, a);
            if (triag_filling) z_val = one_on_z(Xi, Yi, zbd);


            if (z_val>zbuffer[Xi][Yi]) continue;
            if (Xi<0 || Xi>=width || Yi<0 || Yi >= height) {
                cerr<<"Out of range error\n";
                exit(1);
            }
            zbuffer[Xi][Yi] = z_val;
            image(Xi, Yi).red = color.red;
            image(Xi, Yi).green = color.green;
            image(Xi, Yi).blue = color.blue;
        }
    }
}

Point2D proj_triag_point(const Vector3D &A, const double d, double dx, double dy) {
    double Ax = (d*A.x/-A.z)+dx;
    double Ay = (d*A.y/-A.z)+dy;
    Point2D result(Ax, Ay);
    return result;

}

ZBuffData compute_zbuff_data(const Vector3D A, const Vector3D B, const Vector3D C, const double d, double dx, double dy) {
    ZBuffData zbd;


    Point2D a = proj_triag_point(A, d, dx, dy);
    Point2D b = proj_triag_point(B, d, dx, dy);
    Point2D c = proj_triag_point(C, d, dx, dy);

    zbd.xG = (a.x+b.x+c.x)/3;
    zbd.yG = (a.y+b.y+c.y)/3;
    zbd.one_on_zG = (1/(3*A.z))+(1/(3*B.z))+(1/(3*C.z));

    double dzdx, dzdy;
    Vector3D AB = Vector3D::vector(0,0,0);
    Vector3D AC = Vector3D::vector(0,0,0);
    Vector3D w = Vector3D::vector(0,0,0);
    AB = B-A;
    AC = C-A;

    w = AB.cross_equals(AC);    // Vectorieel product (kruisproduct)

    double k = (w.x*A.x + w.y*A.y + w.z*A.z);
//    k = 404.2;
    zbd.dzdx = w.x/(-d*k);
    zbd.dzdy = w.y/(-d*k);
    return zbd;
}




void LineDrawer::draw_zbuf_triag(ZBuffer &zbuffer, img::EasyImage &img, const Vector3D &A, const Vector3D &B,
                                 const Vector3D &C,
                                 double d, double dx, double dy, Color color, Color ambienReflection,
                                 double reflectionCoeff,
                                 Lights3D &lights) {

    double INFTY = std::numeric_limits<double>::infinity();
    MyTools mt;
    Point2D a = proj_triag_point(A, d, dx, dy);
    Point2D b = proj_triag_point(B, d, dx, dy);
    Point2D c = proj_triag_point(C, d, dx, dy);
/*    Point2D a = proj_point(A, d);
    Point2D b = proj_point(B, d);
    Point2D c = proj_point(C, d);*/

/*
    rescalePoint2D(a, d, dx, dy);
    rescalePoint2D(b, d, dx, dy);
    rescalePoint2D(c, d, dx, dy);*/

    ZBuffData zbd = compute_zbuff_data(A, B, C, d, dx, dy);

  /*  draw_zbuf_line(zbuffer, img, a, b, color, false, zbd);
    draw_zbuf_line(zbuffer, img, b, a, color, false, zbd);
    draw_zbuf_line(zbuffer, img, a, c, color, false, zbd);
*/
    int Ymin = std::lround(min(min(a.y, b.y), c.y) + 0.5);
    int Ymax = std::lround(max(max(a.y, b.y), c.y) - 0.5);

    double xL_AB, xL_AC, xL_BC, xR_AB, xR_AC, xR_BC;
    for (int y=min(Ymin, Ymax); y<=max(Ymin, Ymax); y++) {
        xL_AB = INFTY, xL_AC = INFTY, xL_BC = INFTY;
        xR_AB = -INFTY, xR_AC = -INFTY, xR_BC = -INFTY;
        int xL, xR;

        Point2D P = a;
        Point2D Q = b;

        if ((y - P.y) * (y - Q.y) <= 0 && P.y != Q.y) {     // Test for intersection
            double Xi = Q.x + (P.x - Q.x) * ((y - Q.y) / (P.y - Q.y));
            xL_AB = Xi;
            xR_AB = Xi;
        }

        P = b; Q = c;
        if ((y - P.y) * (y - Q.y) <= 0 && P.y != Q.y) {     // Test for intersection
            double Xi = Q.x + (P.x - Q.x) * ((y - Q.y) / (P.y - Q.y));
            xL_BC = Xi;
            xR_BC = Xi;
        }

        P = a; Q = c;
        if ((y - P.y) * (y - Q.y) <= 0 && P.y != Q.y) {     // Test for intersection
            double Xi = Q.x + (P.x - Q.x) * ((y - Q.y) / (P.y - Q.y));
            xL_AC = Xi;
            xR_AC = Xi;
        }
            xL = lround(min(min(xL_AB, xL_AC), xL_BC)+0.5);
            xR = lround(max(max(xR_AB, xR_AC), xR_BC)-0.5);
            Point2D p1(xL, y);
            Point2D p2(xR, y);
            if (p1.y != p2.y || (p1.x==0 && p2.x==0)) return;

            draw_zbuf_line(zbuffer, img, p1, p2, color, true, zbd);
//            drawLine2D(img, p1, p2, color);

    }
}
