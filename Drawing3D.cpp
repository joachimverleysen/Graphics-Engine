//
// Created by joachimverleysen on 3/15/24.
//

#include "Drawing3D.h"

int Drawing3D::getSize() const {
    return size;
}

void Drawing3D::setSize(int size) {
    Drawing3D::size = size;
}

const string &Drawing3D::getType() const {
    return type;
}

void Drawing3D::setType(const string &type) {
    Drawing3D::type = type;
}

const Color &Drawing3D::getBgColor() const {
    return bgColor;
}

void Drawing3D::setBgColor(const Color &bgColor) {
    Drawing3D::bgColor = bgColor;
}

int Drawing3D::getNrFigures() const {
    return nrFigures;
}

void Drawing3D::setNrFigures(int nrFigures) {
    Drawing3D::nrFigures = nrFigures;
}

const Vector3D &Drawing3D::getEye() const {
    return eye;
}

void Drawing3D::setEye(const Vector3D &point) {
    Drawing3D::eye = point;
}

const Figures3D &Drawing3D::getFigures() const {
    return figures;
}

void Drawing3D::setFigures(const Figures3D &figures) {
    Drawing3D::figures = figures;
}

void Drawing3D::addFigure(Figure &fig) {
    figures.push_back(fig);

}

Lines2D Drawing3D::compute2DLines() {
    Figures3D converted_figs;
//    Matrix eyeTrans = eyePointTrans(eye);
    for (auto &fig : getFigures()) {
        Figure converted_fig = do_fig_projection(fig);
    }
    return Lines2D();
}

Figure Drawing3D::do_fig_projection(const Figure &fig) {
    return Figure();
}
