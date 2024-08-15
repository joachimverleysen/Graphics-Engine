//
// Created by joachimverleysen on 3/27/24.
//

#include "MyParser.h"
#include "tools/ini_configuration.h"
#include "Drawing3D.h"
#include "LineDrawer.h"
#include "Solid3D.h"
#include "MyLSystem2D.h"
#include "MyLSystem3D.h"
#include "Light.h"
#include <algorithm>


std::vector<std::string> supported_figures = {
        "Cube",
        "FractalCube",
        "Tetrahedron",
        "FractalTetrahedron",
        "Octahedron",
        "FractalOctahedron",
        "Icosahedron",
        "FractalIcosahedron",
        "Dodecahedron",
        "FractalDodecahedron",
        "Sphere",
        "FractalSphere",
        "Cone",
        "FractalCone",
        "Cylinder",
        "FractalCylinder",
        "Torus",
        "FractalTorus",
        "LineDrawing",
        "3DLSystem",
        "2DLSystem"
};


SuccessEnum MyParser::drawing3D_parse(const ini::Configuration &conf, Drawing3D &drawing) {
    //GENERAL
    std::string genType = conf["General"]["type"].as_string_or_die();
    int size = conf["General"]["size"].as_int_or_die();
    vector<double> bgColor = conf["General"]["backgroundcolor"].as_double_tuple_or_die();
    int nrFigures = conf["General"]["nrFigures"].as_int_or_default(0);
    vector<double> eye = conf["General"]["eye"].as_double_tuple_or_default({0,0,0});
    int nrLights = conf["General"]["nrLights"].as_int_or_default(0);

    drawing.setBgColor(Color(bgColor[0], bgColor[1], bgColor[2]));
    drawing.setEye(Vector3D::point(eye[0], eye[1], eye[2]));
    drawing.setSize(size);
    drawing.setType(genType);
    drawing.setNrFigures(nrFigures);
    drawing.setNrLights(nrLights);

    // Lights
    MyParser::parse_lights(conf, drawing);

    //FIGURES
    for (int i = 0; i < nrFigures; i++) {
        Figure figure;
        figure.setSize(size);
        std::string figname = "Figure" + std::to_string(i);

        std::string figtype = conf[figname]["type"].as_string_or_die();
        auto it = std::find(supported_figures.begin(), supported_figures.end(), figtype);
        if (it==supported_figures.end()) {
            return failure;

        }


        MyParser::parse_figure(figure, conf, drawing, i);


        MyParser::parse_solid3d(figure, conf, drawing);

        // Generate fractals
        Solid3D solid3D;
        Figures3D fractal;

        solid3D.generateFractal(figure, fractal, figure.getNrIterations(), figure.getFractalscale());

        for (auto& f : fractal) {
            drawing.addFigure(f);
        }
        if (fractal.empty()) drawing.addFigure(figure);

    }
    return success;


}

