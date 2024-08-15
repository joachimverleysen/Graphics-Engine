#include "easy_image.h"
#include "LineDrawer.h"
#include "Figure.h"
#include "../tools/vector3d.h"
#include <string>
#include <vector>
#include <iostream>
#include "Light.h"
#include "Transformations.h"
#include "ini_configuration.h"
#include "MyLSystem2D.h"
#include "MyParser.h"

//
// Created by joachimverleysen on 8/15/24.
//
void Drawing3D::dispatch_drawing_by_type(Drawing3D &drawing, const ini::Configuration &conf, img::EasyImage &image) {
    string type = drawing.getType();
    if (type=="ZBufferedWireframe") {
        Vector3D eye = drawing.getEye();
        Color bgColor = drawing.getBgColor();
        Figures3D figs = drawing.getFigures();
        Transformations::apply_transformations(figs);
        Transformations::to_eye_system(figs, drawing.getEye());
        Lines2D lines = Transformations::do_projection(figs);
        image = LineDrawer::draw2Dlines(lines, drawing.getSize(), bgColor);
    }
    else if (type=="ZBuffering") {
        image = Drawing3D::zbuffDrawing(drawing);
    }
    else if (type=="Wireframe") {
        Vector3D eye = drawing.getEye();
        Color bgColor = drawing.getBgColor();
        Figures3D figs = drawing.getFigures();
        Transformations::apply_transformations(figs);
        Transformations::to_eye_system(figs, drawing.getEye());

        Lines2D lines = Transformations::do_projection(figs);
        image = LineDrawer::draw2Dlines(lines, drawing.getSize(), bgColor);
    }
    else if (type == "2DLSystem") {
        MyLSystem2D ls = MyParser::parse_Lsystem2D(conf);
        Color bgColor = ls.getBgColor();
        vector<Point2D> points;
        ls.computePoints(points);
        ls.setPoints(points);
        Lines2D lines = MyTools::getLineArray2D(points, ls.getColor());
        image = LineDrawer::draw2Dlines(lines, drawing.getSize(), bgColor);
    }
    else if (type == "LightedZBuffering") {
        image = Drawing3D::zbuffDrawing(drawing);
    }

}
