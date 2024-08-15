//
// Created by joachimverleysen on 3/6/24.
//

#include "Figure.h"


const vector<Vector3D> &Figure::getPoints() const {
    return points;
}

void Figure::setPoints(const vector<Vector3D> &points) {
    Figure::points = points;
}

const vector<Face> &Figure::getFaces() const {
    return faces;
}

void Figure::setFaces(const vector<Face> &faces) {
    Figure::faces = faces;
}

const Color &Figure::getColor() const {
    return color;
}

void Figure::setColor(const Color &color) {
    Figure::color = color;
}

int Figure::getSize() const {
    return size;
}

void Figure::setSize(int size) {
    Figure::size = size;
}

const vector<Point2D> &Figure::getProjPoints() const {
    return proj_points;
}

void Figure::setProjPoints(const vector<Point2D> &projPoints) {
    proj_points = projPoints;
}

double Figure::getScale() const {
    return scale;
}

void Figure::setScale(double scale) {
    Figure::scale = scale;
}

double Figure::getRotateX() const {
    return rotateX;
}

void Figure::setRotateX(double rotateX) {
    Figure::rotateX = rotateX;
}

double Figure::getRotateY() const {
    return rotateY;
}

void Figure::setRotateY(double rotateY) {
    Figure::rotateY = rotateY;
}

double Figure::getRotateZ() const {
    return rotateZ;
}

void Figure::setRotateZ(double rotateZ) {
    Figure::rotateZ = rotateZ;
}

const Vector3D &Figure::getCenter() const {
    return center;
}

void Figure::setCenter(const Vector3D &center) {
    Figure::center = center;
}

const Color &Figure::getAmbientReflection() const {
    return ambientReflection;
}

void Figure::setAmbientReflection(const Color &ammbientReflection) {
    Figure::ambientReflection = ammbientReflection;
}

const Color &Figure::getDiffuseReflection() const {
    return diffuseReflection;
}

void Figure::setDiffuseReflection(const Color &diffuseReflection) {
    Figure::diffuseReflection = diffuseReflection;
}

int Figure::getNrIterations() const {
    return nrIterations;
}

void Figure::setNrIterations(int nrIterations) {
    Figure::nrIterations = nrIterations;
}

double Figure::getFractalscale() const {
    return fractalscale;
}

void Figure::setFractalscale(double fractalscale) {
    Figure::fractalscale = fractalscale;
}

const std::string &Figure::getType() const {
    return type;
}

void Figure::setType(const std::string &type) {
    Figure::type = type;
}

const std::string &Figure::getName() const {
    return name;
}

void Figure::setName(const std::string &name) {
    Figure::name = name;
}

void Figure::triangulateFigure(Figure &fig) {
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

Lines2D Figure::getLineArray2D(const vector<Point2D> &points, Color col) {
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


Lines2D Figure::computeLines(Figure &fig) {
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
            Lines2D lines = getLineArray2D(points, col);
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
            Lines2D lines = getLineArray2D(points, col);
            result.insert(result.end(), lines.begin(), lines.end());
        }

    }
    return result;
}


