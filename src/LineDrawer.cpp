//
// Created by joachimverleysen on 2/27/24.
//

#include "LineDrawer.h"
#include "MyTools.h"
#include <fstream>

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
    dims.xRange = abs(dims.Xmax-dims.Xmin);
    dims.yRange = abs(dims.Ymax-dims.Ymin);
    dims.imgX = size * (dims.xRange/max(dims.xRange, dims.yRange));
    dims.imgY = size * (dims.yRange / max(dims.xRange, dims.yRange));
    dims.d = 0.95 * (dims.imgX/dims.xRange);    // scale factor

    dims.DCx = dims.d* ((dims.Xmin + dims.Xmax)/2);
    dims.DCy = dims.d* ((dims.Ymin + dims.Ymax)/2);
    dims.dx = (dims.imgX/2) - dims.DCx;
    dims.dy = (dims.imgY/2) - dims.DCy;
    dims.width = round(dims.imgX);
    dims.height = round(dims.imgY);
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
  /*  p.x = round(p.x);
    p.y = round(p.y);*/

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

double compute_1_on_z_WF(const double& z0, const double& z1, double i, double a) {
    double result;

    double p;
    if (a==0) {
        a=1;
    }

    p = i/a;
    result = (p / z0) + ((1 - p) / z1);
    return result;
}

double get_inv_z(int x, int y, ZBuffData &zbd) {
    return 1.0001*(zbd.one_on_zG + (x-zbd.xG)*zbd.dzdx + (y-zbd.yG)*zbd.dzdy);
}

void
LineDrawer::draw_zbuf_line(ZBuffer &zbuffer, img::EasyImage &image, Point2D &pt1, Point2D &pt2, Color &color,
                           bool triag_filling, ZBuffData zbd= ZBuffData()) {

    unsigned int x0, x1, y0, y1;

    x0 = round(pt1.x);
    x1 = round(pt2.x);
    y0 = round(pt1.y);
    y1 = round(pt2.y);
    double z0, z1;
    z0 = pt1.z_val;
    z1 = pt2.z_val;

    unsigned int width = image.get_width();
    unsigned int height = image.get_height();
    int Xa = round(min(pt1.x, pt2.x));
    int Xb = lround(max(pt1.x, pt2.x));

    // Check for switching points

    bool switch_points = false;
    if (x0==x1 && y0>y1) {    // Vertical line
        switch_points = true;

    }
    else if (y0==y1 && x0 > x1) {   // Horizontal line
        switch_points = true;
    }

    if (switch_points) {
        std::swap(x0, x1);
        std::swap(y0, y1);
        std::swap(z0, z1);
    }

    int x_distance = x1-x0;
    int y_distance = y1-y0;
    x_distance = abs(x_distance);
    y_distance = abs(y_distance);

    int a;
    double inv_z;

    if (x0==x1 && !triag_filling) {   // Vertical line
    int i = x0;
    a=y_distance;
        try {
            if (y_distance < 0) {
                throw std::invalid_argument("Y-distance can't be negative");
            }


        } catch (const std::invalid_argument& e) {
            std::cerr << "Caught exception: " << e.what() << std::endl;
        }
        for (int j = y0; j <= y1; j++) {
            inv_z = compute_1_on_z_WF(z0, z1, a - j + y0, a);

            if (i == x0) {
                    if (inv_z > zbuffer[i][j]) continue;
                    zbuffer[i][j] = inv_z;
                    image(i, j).red = color.red;
                image(i, j).green = color.green;
                image(i, j).blue = color.blue;
            }
        }
        return;
    }

    else if (y0 == y1) {   // Horizontal line
        a=x_distance;
        try {
            if (x_distance < 0) {
                throw std::invalid_argument("X-distance can't be negative");
            }


        } catch (const std::invalid_argument& e) {
            std::cerr << "Caught exception: " << e.what() << std::endl;
        }

        for(int i = x0; i <= x1 ; i++) {
            inv_z = compute_1_on_z_WF(z0, z1, a - i + x0, a);

            int j = y0;
            if (triag_filling) inv_z = get_inv_z(i, j, zbd);
            if (i>=width || j>=height) {
                throw std::out_of_range("Out of range error");
            }
            if (inv_z > zbuffer[i][j]) continue;
            zbuffer[i][j] = inv_z;
            image(i, j).red = color.red;
            image(i, j).green = color.green;
            image(i, j).blue = color.blue;


        }
        return;
    }

    if (x0>x1) {
        std::swap(x0, x1);
        std::swap(y0, y1);
        std::swap(z0, z1);
    }
    x_distance = x1-x0;

    double m = ((double) y1-(double) y0)/ ((double) x1 - (double) x0);

    if (-1 <= m && m <= 1) {     // Cursus: geval 1 en 2
        a=x_distance;
        try {
            if (x_distance < 0) {
                throw std::invalid_argument("X-distance can't be negative");
            }


        } catch (const std::invalid_argument& e) {
            std::cerr << "Caught exception: " << e.what() << std::endl;
        }

        for(int i = 0; i <= x_distance; i++) {
            int Xi = x0 + i;
            int Yi = round(y0 + (m*i));
            inv_z = compute_1_on_z_WF(z0, z1, a - i, a);

            if (inv_z > zbuffer[Xi][Yi]) continue;

            if (Xi<0 || Xi>=width || Yi<0 || Yi >= height) {
                throw std::out_of_range("Out of range error");
            }/*if (distance == 0) {
        throw std::invalid_argument("Distance = 0");
    }*/
            zbuffer[Xi][Yi] = inv_z;
            image(Xi, Yi).red = color.red;
            image(Xi, Yi).green = color.green;
            image(Xi, Yi).blue = color.blue;
        }
    }

    else if (m > 1) {     // Cursus: geval 3
        // Ya < Yb (stijgend)

        a=y_distance;
        try {
            if (y_distance < 0) {
                throw std::invalid_argument("Y-distance can't be negative");
            }


        } catch (const std::invalid_argument& e) {
            std::cerr << "Caught exception: " << e.what() << std::endl;
        }
        for(int i = 0; i <= y_distance; i++) {

            int Xi = round(x0 + (i / m));
            int Yi = y0 + i;
            inv_z = compute_1_on_z_WF(z0, z1, a - i, a);


            if (triag_filling) inv_z = get_inv_z(Xi, Yi, zbd);

            if (inv_z > zbuffer[Xi][Yi]) continue;
            if (Xi<0 || Xi>=width || Yi<0 || Yi >= height) {
                throw std::out_of_range("Out of range error");
            }
            zbuffer[Xi][Yi] = inv_z;
            image(Xi, Yi).red = color.red;
            image(Xi, Yi).green = color.green;
            image(Xi, Yi).blue = color.blue;
        }
    }

    else if (m < -1) {     // Cursus: geval 4
        // Ya > Yb (dalend)
        a=y_distance;
        try {
            if (y_distance < 0) {
                throw std::invalid_argument("Y-distance can't be negative");
            }


        } catch (const std::invalid_argument& e) {
            std::cerr << "Caught exception: " << e.what() << std::endl;
        }
        for(int i = 0; i <= y_distance; i++) {
            int Xi = round(x0 - (i / m));
            int Yi = y0 - i;

            inv_z = compute_1_on_z_WF(z0, z1, a - i, a);


            if (triag_filling) inv_z = get_inv_z(Xi, Yi, zbd);

            if (inv_z > zbuffer[Xi][Yi]) continue;
            if (Xi<0 || Xi>=width || Yi<0 || Yi >= height) {
                throw std::out_of_range("Out of range error");
            }
            zbuffer[Xi][Yi] = inv_z;
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
    Vector3D BC = Vector3D::vector(0,0,0);
    Vector3D w = Vector3D::vector(0,0,0);
    AB = B-A;
    AC = C-A;

    w = AB.cross_equals(AC);    // Vectorieel product (kruisproduct)
    zbd.w = w;

    double k = (w.x*A.x + w.y*A.y + w.z*A.z);
//    k = 404.2;
    zbd.dzdx = w.x/(-d*k);
    zbd.dzdy = w.y/(-d*k);
    return zbd;
}




void LineDrawer::drawZbuffTriang(ZBuffer &zbuffer, img::EasyImage &img, const Vector3D &A, const Vector3D &B,
                                 const Vector3D &C,
                                 double d, double dx, double dy, Color color) {

    double INFTY = std::numeric_limits<double>::infinity();
    Point2D a = proj_triag_point(A, d, dx, dy);
    Point2D b = proj_triag_point(B, d, dx, dy);
    Point2D c = proj_triag_point(C, d, dx, dy);


    ZBuffData zbd = compute_zbuff_data(A, B, C, d, dx, dy);

    int Ymin = static_cast<int>(std::round(min(min(a.y, b.y), c.y) + 0.5));
    int Ymax = static_cast<int>(std::round(max(max(a.y, b.y), c.y) - 0.5));

    for (int y=Ymin; y<=Ymax; y++) {

        double xL = INFTY, xR = -INFTY;
        auto updateBounds = [&](const Point2D &P, const Point2D &Q) {
            if ((y - P.y) * (y - Q.y) <= 0 && P.y != Q.y) {
                double Xi = Q.x + (P.x - Q.x) * ((y - Q.y) / (P.y - Q.y));
                xL = std::min(xL, Xi);
                xR = std::max(xR, Xi);
            }
        };

        updateBounds(a, b);
        updateBounds(b, c);
        updateBounds(c, a);

            double xLint = round(xL+0.5);
            double xRint = round(xR-0.5);
            Point2D p1(xLint, y);
            Point2D p2(xRint, y);

            draw_zbuf_line(zbuffer, img, p1, p2, color, true, zbd);

    }
}

Color getAmbientComponent(Light l, Color ambientReflec) {
    double redPCT=0;  // Percentage value
    double bluePCT=0;  // Percentage value
    double greenPCT=0;  // Percentage value

    redPCT += (l.ambientLight.red/255);
    greenPCT += (l.ambientLight.green/255);
    bluePCT += (l.ambientLight.blue/255);

    redPCT *= (ambientReflec.red/255);
    greenPCT *= (ambientReflec.green/255);
    bluePCT *= (ambientReflec.blue/255);

    Color result(redPCT, greenPCT, bluePCT);
    return result;
}

Color getDiffuseComponent(Vector3D w, Light li, Color diffReflec) {
    MyTools mt;
    Vector3D direction = li.direction;
    direction.normalise();
    double redPCT = 0;  // Percentage value
    double bluePCT = 0;  // Percentage value
    double greenPCT = 0;  // Percentage value

    Vector3D Ld = direction;

    Vector3D n = w;
    n.normalise();
    Vector3D l = Ld;
    l.normalise();
    l *= -1;

    double scalprod = n.x * l.x + n.y * l.y + n.z * l.z;
    if (scalprod<0) return Color(0,0,0);


    redPCT += (li.diffuseLight.red / 255);
    greenPCT += (li.diffuseLight.green / 255);
    bluePCT += (li.diffuseLight.blue / 255);


    redPCT *= (diffReflec.red/255) * scalprod;
    greenPCT *= (diffReflec.green / 255) * scalprod;
    bluePCT *= (diffReflec.blue / 255) * scalprod;

    if (redPCT>1) redPCT=1;
    if (greenPCT>1) greenPCT=1;
    if (bluePCT>1) bluePCT=1;

    Color result(redPCT, greenPCT, bluePCT);
    return result;

}


    void LineDrawer::drawZbuffTriangLighted(ZBuffer &zbuffer, img::EasyImage &img, const Vector3D &A, const Vector3D &B,
                                        const Vector3D &C,
                                        double d, double dx, double dy, Color color, Color ambienReflection,
                                        Color diffuseReflection,
                                        double reflectionCoeff, Lights3D &lights) {

    double INFTY = std::numeric_limits<double>::infinity();
    MyTools mt;
    Point2D a = proj_triag_point(A, d, dx, dy);
    Point2D b = proj_triag_point(B, d, dx, dy);
    Point2D c = proj_triag_point(C, d, dx, dy);


    ZBuffData zbd = compute_zbuff_data(A, B, C, d, dx, dy);

    int Ymin = std::round(min(min(a.y, b.y), c.y) + 0.5);
    int Ymax = std::round(max(max(a.y, b.y), c.y) - 0.5);

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
        xL = round(min(min(xL_AB, xL_AC), xL_BC)+0.5);
        xR = round(max(max(xR_AB, xR_AC), xR_BC)-0.5);
        Point2D p1(xL, y);
        Point2D p2(xR, y);
        if (p1.y != p2.y || (p1.x==0 && p2.x==0)) return;

        Color resultingColor;
        for (auto l : lights) {
            Color ambientComponent = getAmbientComponent(l, ambienReflection);
            Color diffuseComponent = getDiffuseComponent(zbd.w, l, diffuseReflection);
            resultingColor = resultingColor+ambientComponent;
            if (l.diffuse()) resultingColor = resultingColor+diffuseComponent;

        }
        if (resultingColor.red>255) resultingColor.red = 255;
        if (resultingColor.green>255) resultingColor.green = 255;
        if (resultingColor.blue>255) resultingColor.blue = 255;


        draw_zbuf_line(zbuffer, img, p1, p2, resultingColor, true, zbd);
//            drawLine2D(img, p1, p2, color);

    }
}
