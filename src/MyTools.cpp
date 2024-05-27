//
// Created by joachimverleysen on 4/1/24.
//

#include "MyTools.h"
#include "LineDrawer.h"
#include "Figure.h"

double degree_to_rad(double deg) {
    return deg*M_PI/180;
}

Point2D MyTools::_do_point_projection(const Vector3D &point, const double d= 1) {
    // Point should already be in eye-coordinate system
    double x_new = d*point.x/-point.z;
    double y_new = d*point.y/-point.z;
    Point2D p(x_new, y_new);
    p.z_val=point.z;
    return p;
}



polars MyTools::toPolar(const Vector3D &point) {
    double theta;
    double phi;
    double r;
    r = sqrt(pow(point.x, 2) + pow(point.y, 2) + pow(point.z, 2));
    theta = atan2(point.y, point.x);
    if (theta < 0) {
        theta += 2 * M_PI; // Voeg 2π toe aan negatieve waarden om het interval te verschuiven naar [0, 2π)
    }
    phi = acos(point.z/r);
    // !!! atan in interval ]-pi/2, pi/2[
    polars p(r, theta, phi);
    return p;

}

Matrix MyTools::eyePointTrans(const Vector3D &eyepoint) {
    Matrix m;
    polars p = toPolar(eyepoint);
    m(1,1) = -sin(p.theta);
    m(1,2) = -cos(p.theta)*cos(p.phi);
    m(1,3) = cos(p.theta)*sin(p.phi);
    m(2,1) = cos(p.theta);
    m(2,2) = -sin(p.theta)*cos(p.phi);
    m(2,3) = sin(p.theta)*sin(p.phi);
    m(3,2) = sin(p.phi);
    m(3,3) = cos(p.phi);
    m(4, 3) = -p.r;
    return m;
}

void MyTools::convert_fig_to_eyesys(Figure &fig, const Vector3D &eye) {
    Matrix eyeTrans = eyePointTrans(eye);
    vector<Vector3D> convertedPoints;
    for (const auto& p : fig.getPoints()) {
        Vector3D point_new = p*eyeTrans;
        convertedPoints.push_back(point_new);
    }
    fig.setPoints(convertedPoints);
}

void MyTools::convert_point_to_eyesys(Vector3D& point, const Vector3D &eye) {
    Matrix eyeTrans = eyePointTrans(eye);
    Vector3D point_new = point*eyeTrans;
    point = point_new;
}

void MyTools::point_to_eyesys(Vector3D& point, const Vector3D &eye) {
    Matrix eyeTrans = eyePointTrans(eye);
    point*=eyeTrans;
}

vector<Point2D> MyTools::compute_proj_points(Figure &fig) {
    vector<Point2D> result;
    for (auto p : fig.getPoints()) {
        Point2D proj = _do_point_projection(p);
        result.push_back(proj);
    }
    return result;
}

Lines2D MyTools::getLineArray(const vector<Point2D> &points, Color col) {
    // Pairs the points in the pointArray 2 by 2 to create lines
    Lines2D result;
    for (int i=0; i<points.size(); i+=2) {
        Line2D line(points[i], points[i+1], col);
        line.z1 = points[i].z_val;
        line.z2 = points[i+1].z_val;
        result.push_back(line);
    }
    return result;
}

Lines2D MyTools::computeLines(Figure &fig) {
    // Make sure the fig.proj_points vector has been made when calling this
    Color col = fig.getColor();
    Lines2D result;
    for (auto face : fig.getFaces()) {
        vector<Point2D> points;
        if (face.point_indexes.size()==2) {
            for (auto i : face.point_indexes) {
                Point2D point = fig.getProjPoints()[i];
                points.push_back(point);
            }
            Lines2D lines = getLineArray(points, col);
            result.insert(result.end(), lines.begin(), lines.end());
        }
        else if (face.point_indexes.size() > 2) {
            for (int i = 0; i < face.point_indexes.size(); i++) {
                Point2D p1 = fig.getProjPoints()[face.point_indexes[i]];
                Point2D p2(-1,-1);
                if (i == face.point_indexes.size()-1) {
                    p2 = fig.getProjPoints()[face.point_indexes[0]];    // Connect last point with first point
                }
                else p2 = fig.getProjPoints()[face.point_indexes[i+1]];

                // Adding points in pairs to the vector to facilitate generating lineArray
                points.push_back(p1);
                points.push_back(p2);
            }
            Lines2D lines = getLineArray(points, col);
            result.insert(result.end(), lines.begin(), lines.end());
        }

    }
    return result;
}


Matrix MyTools::get_rot_y(const double angle_deg) {
    double angle = degree_to_rad(angle_deg);
    Matrix a;
    a(1,1) = cos(angle);
    a(1,3) = -sin(angle);
    a(3,1) = sin(angle);
    a(3,3) = cos(angle);
    return a;
}

Matrix MyTools::get_rot_x(const double angle_deg) {
    double angle = degree_to_rad(angle_deg);
    Matrix a;
    a(2,2) = cos(angle);
    a(2,3) = sin(angle);
    a(3,2) = -sin(angle);
    a(3,3) = cos(angle);
    return a;
}

Matrix MyTools::get_rot_z(const double angle_deg) {
    double angle = degree_to_rad(angle_deg);
    Matrix a;
    a(1,1) = cos(angle);
    a(1,2) = sin(angle);
    a(2,1) = -sin(angle);
    a(2,2) = cos(angle);
    return a;
}

Matrix MyTools::get_scale_mtx(const double scale) {
    Matrix a;
    a(1,1) = scale;
    a(2,2) = scale;
    a(3,3) = scale;
    return a;
}

Matrix MyTools::get_translate_mtx(const Vector3D &vector) {
    Matrix a;
    a(4,1) = vector.x;
    a(4,2) = vector.y;
    a(4,3) = vector.z;
    return a;
}

void MyTools::doTransitions(Figure &fig) {
    Matrix scale = get_scale_mtx(fig.getScale());
    Matrix rotateX = get_rot_x(fig.getRotateX());
    Matrix rotateY = get_rot_y(fig.getRotateY());
    Matrix rotateZ = get_rot_z(fig.getRotateZ());
    Matrix translate = get_translate_mtx(fig.getCenter());
    vector<Vector3D> convertedPoints;
    for (const auto& p : fig.getPoints()) {
        Vector3D point_new = p*scale*rotateX*rotateY*rotateZ*translate;
        convertedPoints.push_back(point_new);
    }
    fig.setPoints(convertedPoints);
}

void MyTools::triangulateFigure(Figure &fig) {
    vector<Face> result_faces;
    vector<Face> faces = fig.getFaces();
    for (auto& f : faces) {
        int n = f.point_indexes.size();
        for (int i=1; i<n-1; i++) {
            Face triangle({f.point_indexes[0], f.point_indexes[i], f.point_indexes[i+1]});
            result_faces.push_back(triangle);
        }
    }
    fig.setFaces(result_faces);
}

void MyTools::applyTransformation(Figure &fig, Matrix &mtx) {
    auto points = fig.getPoints();
    vector<Vector3D> points_new;
    for (auto p : points) {
        Vector3D point_new = Vector3D::point(p.x, p.y, p.z);
        point_new *= mtx;
        points_new.push_back(point_new);
    }
    fig.setPoints(points_new);

}
