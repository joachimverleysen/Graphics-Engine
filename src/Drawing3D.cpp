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

const Lights3D &Drawing3D::getLights() const {
    return lights;
}

void Drawing3D::setLights(const Lights3D &lights) {
    Drawing3D::lights = lights;
}

int Drawing3D::getNrLights() const {
    return nrLights;
}

void Drawing3D::setNrLights(int nrLights) {
    Drawing3D::nrLights = nrLights;
}


img::EasyImage Drawing3D::zbuffDrawing(Drawing3D &drawing) {
    // do transformations

    LineDrawer ld;
    Vector3D eye = drawing.getEye();
    Figures3D figs = drawing.getFigures();
    Transformations::apply_transformations(figs);
    Transformations::to_eye_system(figs, drawing.getEye());

    for (auto& f : figs) {
        MyTools::triangulateFigure(f);
    }

    Lines2D lines = Transformations::do_projection(figs);
    int size = drawing.getSize();

    Dimensions dim = ld.computeDims(lines, size);

    img::EasyImage image(dim.width, dim.height);

    ZBuffer zbuffer(dim.width, dim.height);

    auto lights = drawing.getLights();

    // Convert direction to eyesystem
    //todo: optimize
    for (Light& l : lights) {
        Vector3D direction = Vector3D::point(0,0,0);
        direction.x = l.direction.x;
        direction.y = l.direction.y;
        direction.z = l.direction.z;
        Transformations::to_eye_system(direction, eye);
        l.direction.x = direction.x;
        l.direction.y = direction.y;
        l.direction.z = direction.z;
    }
//    Transformations::to_eye_system(figs, eye);

    for (auto& f : figs) {
        Color color = f.getColor();
        for (auto& face : f.getFaces()) {
            if (face.point_indexes.size() != 3) {
                cerr<<"Error - Face is not a triangle\n";
                exit(1);
            }
            Vector3D p1 = f.getPoints()[face.point_indexes[0]];
            Vector3D p2 = f.getPoints()[face.point_indexes[1]];
            Vector3D p3 = f.getPoints()[face.point_indexes[2]];

            Color ambientReflec = f.getAmbientReflection();

            ld.drawZbuffTriang(zbuffer, image, p1, p2, p3, dim.d, dim.dx, dim.dy, color, ambientReflec,
                               lights);
        }
    }
    return image;
}
