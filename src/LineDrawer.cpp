//
// Created by joachimverleysen on 2/27/24.
//

#include "LineDrawer.h"
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
                    return;
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
                return;
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
                return;
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
                return;
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


void LineDrawer::computeDims(Dimensions& dims, Lines2D& lines, const int size) {
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
}

img::EasyImage LineDrawer::draw2Dlines(Lines2D &lines, const int size, Color &bgColor) {
    Dimensions dims;
    computeDims(dims, lines, size);


    img::EasyImage myImage(dims.width, dims.height);
    setBackground(myImage, bgColor);
    
    for (Line2D &line : lines) {
        // scale factor
        line.p1.x *= dims.d;
        line.p1.y *= dims.d;
        // Verschuiven door offset (dx, dy) op te tellen
        line.p1.x += dims.dx;
        line.p1.y += dims.dy;
        // afronden
        line.p1.x = lround(line.p1.x);
        line.p1.y = lround(line.p1.y);

        // scale factor
        line.p2.x *= dims.d;
        line.p2.y *= dims.d;
        // Verschuiven door offset (dx, dy) op te tellen
        line.p2.x += dims.dx;
        line.p2.y += dims.dy;

        // afronden
        line.p2.x = lround(line.p2.x);
        line.p2.y = lround(line.p2.y);
    }

    ZBuffer zBuffer(dims.width, dims.height);

    cout<<"Drawing "<<lines.size()<<" "<<"lines. Check 'out.bmp'"<<endl;
    for (auto line : lines) {
        draw_zbuf_line(zBuffer, myImage, line.p1, line.p2, line.color);
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

double compute_1_on_z(const Point2D& p1, const Point2D& p2, double i, double a) {
    double result;
    double p = i/a;
    result = (p/p1.z_val) + ((1-p)/p2.z_val);
    return result;
}




void
LineDrawer::draw_zbuf_line(ZBuffer &zbuffer, img::EasyImage &image, Point2D &pt1, Point2D &pt2, Color &color) {

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
    if (Xa==Xb) {   // Vertical line
//        a=y_distance;
        for(unsigned int i = Xa; i<=Xb; i++) {
            z_val = compute_1_on_z(pt1, pt2, a-i, a);

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
                z_val = compute_1_on_z(pt1, pt2, a-i, a);
                if (i<0 || i>=width || j<0||j>=height) {
                    cerr<<"Out of range error\n";
                    return;
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
            z_val = compute_1_on_z(pt1, pt2, a-i, a);
            if (z_val>zbuffer[Xi][Yi]) continue;

            if (Xi<0 || Xi>=width || Yi<0 || Yi >= height) {
                cerr<<"Out of range error\n";
                return;
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
            z_val = compute_1_on_z(pt1, pt2, a-i, a);
            if (z_val>zbuffer[Xi][Yi]) continue;
            if (Xi<0 || Xi>=width || Yi<0 || Yi >= height) {
                cerr<<"Out of range error\n";
                return;
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
            z_val = compute_1_on_z(pt1, pt2, a-i, a);

            if (z_val>zbuffer[Xi][Yi]) continue;
            if (Xi<0 || Xi>=width || Yi<0 || Yi >= height) {
                cerr<<"Out of range error\n";
                return;
            }
            zbuffer[Xi][Yi] = z_val;
            image(Xi, Yi).red = color.red;
            image(Xi, Yi).green = color.green;
            image(Xi, Yi).blue = color.blue;
        }
    }
}

Point2D proj_point(const Vector3D &A, const double d) {
    double Ax = d*A.x/-A.z;
    double Ay = d*A.y/-A.z;
    Point2D result(Ax, Ay);
    return result;

}

void LineDrawer::draw_zbuf_triag(ZBuffer &zbuffer, img::EasyImage &img,
                                 const Vector3D &A,
                                 const Vector3D &B,
                                 const Vector3D &C,
                                 double d, double dx, double dy,
                                 Color color) {

    double INFTY = std::numeric_limits<double>::infinity();

    Point2D a = proj_point(A, 0.95);
    Point2D b = proj_point(B, 0.95);
    Point2D c = proj_point(C, 0.95);

    int Ymin = lround(min(min(a.y, b.y), c.y) + 0.5);
    int Ymax = lround(max(max(a.y, b.y), c.y) - 0.5);

    for (int y=Ymin; y!=Ymax; y++) {
        double xL_AB, xL_AC, xL_BC = INFTY;
        double xR_AB, xR_AC, xR_BC = -INFTY;
        int xL, xR;

        // Note: A=P, B=Q
        if ((y - a.y) * (y - b.y) <= 0) {     // Test for intersection
            double Xi = b.x + (a.x - b.x) * ((y - b.y) / (a.y - b.y));
            xL_AB = Xi;
            xR_AB = Xi;
        }
        if ((y - a.y) * (y - c.y) <= 0) {     // Test for intersection
            double Xi = c.x + (a.x - c.x) * ((y - c.y) / (a.y - c.y));
            xL_AC = Xi;
            xR_AC = Xi;
        }
        if ((y - b.y) * (y - c.y) <= 0) {     // Test for intersection
            double Xi = c.x + (b.x - c.x) * ((y - c.y) / (b.y - c.y));
            xL_BC = Xi;
            xR_BC = Xi;
        }
        xL = lround(min(min(xL_AB, xL_AC), xL_BC)+0.5);
        xR = lround(min(min(xR_AB, xR_AC), xR_BC)-0.5);
        Point2D p1(xL, y);
        Point2D p2(xR, y);
        drawLine2D(img, p1, p2, color);
    }
}

