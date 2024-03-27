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
                    continue;
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
                continue;
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
                continue;
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
                continue;
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

//todo: correct colors from ini file
img::EasyImage LineDrawer::draw2Dlines(Lines2D &lines, const int size, Color &bgColor) {
    double Xmin = 0;
    double Ymin = 0;
    double Xmax = 0;
    double Ymax = 0;
    vector<Point2D> points = getPointArray(lines);
    // Compute max values
    for (auto &p : points) {
        if (p.y < 0) {
        }
        Xmax = max(Xmax, p.x);
        Xmin = min(Xmin, p.x);
        Ymax = max(Ymax, p.y);
        Ymin = min(Ymin, p.y);
    }
    double xRange = Xmax-Xmin;
    double yRange = Ymax-Ymin;
    double imgX = size * (xRange/max(xRange, yRange));
    double imgY = size * (yRange / max(xRange, yRange));
    double d = 0.95 * (imgX/xRange);    // scale factor

    double DCx = d* ((Xmin + Xmax)/2);
    double DCy = d* ((Ymin + Ymax)/2);
    double dx = (imgX/2) - DCx;
    double dy = (imgY/2) - DCy;

    img::EasyImage myImage(lround(imgX), lround(imgY));
    setBackground(myImage, bgColor);
    
    for (Line2D &line : lines) {
        // scale factor
        line.p1.x *= d;
        line.p1.y *= d;
        // Verschuiven door offset (dx, dy) op te tellen
        line.p1.x += dx;
        line.p1.y += dy;
        // afronden
        line.p1.x = lround(line.p1.x);
        line.p1.y = lround(line.p1.y);

        // scale factor
        line.p2.x *= d;
        line.p2.y *= d;
        // Verschuiven door offset (dx, dy) op te tellen
        line.p2.x += dx;
        line.p2.y += dy;

        // afronden
        line.p2.x = lround(line.p2.x);
        line.p2.y = lround(line.p2.y);
    }

    cout<<"Drawing "<<lines.size()<<" "<<"lines. Check 'out.bmp'"<<endl;
    for (auto line : lines) {
        drawLine2D(myImage, line.p1, line.p2, line.color);
    }
    std::ofstream fout("../out/out.bmp", std::ios::binary);
    fout << myImage;
    fout.close();
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

