//
// Created by joachimverleysen on 8/14/24.
//

#include "MyParser.h"
#include "tools/ini_configuration.h"
#include "Drawing3D.h"
#include "LineDrawer.h"
#include "Solid3D.h"
#include "MyLSystem2D.h"
#include "MyLSystem3D.h"
#include "Light.h"


void MyParser::parse_solid3d(Figure &figure, const ini::Configuration &conf, Drawing3D &drawing) {
    Solid3D solid3D;
    string figname = figure.getName();
    int n = conf[figname]["n"].as_int_or_default(0);
    double height = conf[figname]["height"].as_double_or_default(0);
    double r = conf[figname]["r"].as_double_or_default(0);
    double R = conf[figname]["R"].as_double_or_default(0);
    int m = conf[figname]["m"].as_int_or_default(0);    if (figure.getType() == "Cube" || figure.getType() == "FractalCube") {
        solid3D.generateCube(figure);
    }
    if (figure.getType() == "Tetrahedron" || figure.getType() == "FractalTetrahedron") {
        solid3D.generateTetrahedron(figure);
    }
    if (figure.getType() == "Octahedron" || figure.getType() == "FractalOctahedron") {
        solid3D.generateOctahedron(figure);
    }
    if (figure.getType() == "Icosahedron" || figure.getType() == "FractalIcosahedron") {
        solid3D.generateIcosahedron(figure);
    }
    if (figure.getType() == "Dodecahedron" || figure.getType() == "FractalDodecahedron") {
        solid3D.generateDodecahedron(figure);
    }
    if (figure.getType() == "Sphere" || figure.getType() == "FractalSphere") {
        solid3D.generateSphere(figure, n);
    }
    if (figure.getType() == "Cone" || figure.getType() == "FractalCone") {
        solid3D.generateCone(figure, n, height);
    }
    if (figure.getType() == "Cylinder" || figure.getType() == "FractalCylinder") {
        solid3D.generateCylinder(figure, n, height);
    }
    if (figure.getType() == "Torus" || figure.getType() == "FractalTorus") {
        solid3D.generateTorus(figure, r, R, n, m);
    }


}
void MyParser::parse_figure(Figure &figure, const ini::Configuration &conf, Drawing3D &drawing, const int i) {
    string figname = "Figure" + to_string(i);

    string figType = conf[figname]["type"].as_string_or_die();


    vector<double> color = conf[figname]["color"].as_double_tuple_or_default({0,0,0});
    double scale = conf[figname]["scale"].as_double_or_default(1.0);
    double rotateX = conf[figname]["rotateX"].as_double_or_die();
    double rotateY = conf[figname]["rotateY"].as_double_or_die();
    double rotateZ = conf[figname]["rotateZ"].as_double_or_die();
    vector<double> center_ = conf[figname]["center"].as_double_tuple_or_die();
    Vector3D center = Vector3D::point(center_[0], center_[1], center_[2]);

    int nrIterations = conf[figname]["nrIterations"].as_int_or_default(0);
    double fractalscale = conf[figname]["fractalScale"].as_double_or_default(0);


    // Light reflection
    vector<double> ambReflec = conf[figname]["ambientReflection"].as_double_tuple_or_default({0, 0, 0});
    vector<double> diffReflec = conf[figname]["diffuseReflection"].as_double_tuple_or_default({0, 0, 0});
    Color ambientreflection = Color(ambReflec[0], ambReflec[1], ambReflec[2]);
    Color diffReflection = Color(diffReflec[0], diffReflec[1], diffReflec[2]);

    figure.setType(figType);
    figure.setName(figname);
    figure.setColor(Color(color[0], color[1], color[2]));
    figure.setScale(scale);
    figure.setRotateX(rotateX);
    figure.setRotateY(rotateY);
    figure.setRotateZ(rotateZ);
    figure.setCenter(center);
    figure.setAmbientReflection(ambientreflection);
    figure.setDiffuseReflection(diffReflection);
    figure.setNrIterations(nrIterations);
    figure.setFractalscale(fractalscale);


    int nrPoints = conf[figname]["nrPoints"].as_int_or_default(0);
    int nrLines = conf[figname]["nrLines"].as_int_or_default(0);

    //POINTS
    vector<Vector3D> points;
    for (int i = 0; i < nrPoints; i++) {
        string pntname = "point" + to_string(i);
        vector<double> _pt = conf[figname][pntname].as_double_tuple_or_die();
        Vector3D point = Vector3D::point(_pt[0], _pt[1], _pt[2]);
        points.push_back(point);
    }
    //LINES
    vector<Face> faces;
    for (int i = 0; i < nrLines; i++) {
        string linename = "line" + to_string(i);
        vector<int> line = conf[figname][linename].as_int_tuple_or_die();
        Face face;
        face.point_indexes = line;
        faces.push_back(face);

    }

    string lsys_inputfile = conf[figname]["inputfile"].as_string_or_default("");

    if (figure.getType() == "3DLSystem") {
        MyLSystem3D lsys(lsys_inputfile);
        figure.setType("3DLsystem");
        lsys.generateFigure(figure);
    };

    if (figure.getType() == "LineDrawing") {
        figure.setFaces(faces);
        figure.setPoints(points);
    }





}