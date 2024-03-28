//
// Created by joachimverleysen on 3/27/24.
//

#include "MyParser.h"
#include "../tools/ini_configuration.h"
#include "Drawing3D.h"
#include "LineDrawer.h"
#include "Solid3D.h"
#include "LSystem.h"


void MyParser::drawing3D_parse(const ini::Configuration &conf, Drawing3D &drawing) {
    //GENERAL
    std::string genType = conf["General"]["type"].as_string_or_die();
    int size = conf["General"]["size"].as_int_or_die();
    vector<double> bgColor = conf["General"]["backgroundcolor"].as_double_tuple_or_die();
    int nrFigures = conf["General"]["nrFigures"].as_int_or_die();
    vector<double> eye = conf["General"]["eye"].as_double_tuple_or_die();

    drawing.setBgColor(Color(bgColor[0], bgColor[1], bgColor[2]));
    drawing.setEye(Vector3D::point(eye[0], eye[1], eye[2]));
    drawing.setSize(size);

    Solid3D s3d;

    //FIGURES
    for (int i = 0; i < nrFigures; i++) {
        string figname = "Figure" + to_string(i);
        Figure fig;
        string figType = conf[figname]["type"].as_string_or_die();
        bool lineDrawing = (figType == "LineDrawing");

        int nrPoints = conf[figname]["nrPoints"].as_int_or_default(0);
        int nrLines = conf[figname]["nrLines"].as_int_or_default(0);
        vector<double> color = conf[figname]["color"].as_double_tuple_or_die();
        double scale = conf[figname]["scale"].as_double_or_default(1.0);
        int rotateX = conf[figname]["rotateX"].as_int_or_die();
        int rotateY = conf[figname]["rotateY"].as_int_or_die();
        int rotateZ = conf[figname]["rotateZ"].as_int_or_die();
        vector<double> center_ = conf[figname]["center"].as_double_tuple_or_die();
        Vector3D center = Vector3D::point(center_[0], center_[1], center_[2]);
        int n = conf[figname]["n"].as_int_or_default(0);
        double height = conf[figname]["height"].as_double_or_default(0);
        double r = conf[figname]["r"].as_double_or_default(0);
        double R = conf[figname]["R"].as_double_or_default(0);
        int m = conf[figname]["m"].as_int_or_default(0);

        fig.setColor(Color(color[0], color[1], color[2]));
        fig.setSize(size);
        fig.setScale(scale);
        fig.setRotateX(rotateX);
        fig.setRotateY(rotateY);
        fig.setRotateZ(rotateZ);
        fig.setCenter(center);

        if (figType == "Cube") {
            s3d.generateCube(fig);
        };
        if (figType == "Tetrahedron") {
            s3d.generateTetrahedron(fig);
        };
        if (figType == "Octahedron") {
            s3d.generateOctahedron(fig);
        };
        if (figType == "Icosahedron") {
            s3d.generateIcosahedron(fig);
        };
        if (figType == "Dodecahedron") {
            s3d.generateDodecahedron(fig);
        };
        if (figType == "Sphere") {
            s3d.generateSphere(fig, n);
        };
        if (figType == "Cone") {
            s3d.generateCone(fig, n, height);
        };
        if (figType == "Cylinder") {
            s3d.generateCylinder(fig, n, height);
        };
        if (figType == "Torus") {
            s3d.generateTorus(fig, r, R, n, m);
        };


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
        if (lineDrawing) {
            fig.setFaces(faces);
            fig.setPoints(points);
        }

        drawing.addFigure(fig);
    }
}

LSystem MyParser::lsys_parse(const ini::Configuration &conf) {
    std::string type = conf["General"]["type"].as_string_or_die();
    int size = conf["General"]["size"].as_int_or_die();

    vector<double> bgColor = conf["General"]["backgroundcolor"].as_double_tuple_or_die();
    string inputfile = conf["2DLSystem"]["inputfile"].as_string_or_die();
    vector<double> color = conf["2DLSystem"]["color"].as_double_tuple_or_die();
    LSystem result(inputfile);
    result.size = (size);
    result.color = (Color(*(color.begin()), *(color.begin()+1), *(color.begin()+2)));
    result.bgColor = (Color(*(bgColor.begin()), *(bgColor.begin()+1), *(bgColor.begin()+2)));
    return result;
}