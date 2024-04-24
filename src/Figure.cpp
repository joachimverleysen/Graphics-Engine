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

