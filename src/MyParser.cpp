//
// Created by joachimverleysen on 3/27/24.
//

#include "MyParser.h"
#include "../tools/ini_configuration.h"
#include "Drawing3D.h"
#include "LineDrawer.h"
#include "Solid3D.h"
#include "MyLSystem2D.h"
#include "MyLSystem3D.h"
#include "Light.h"

void MyParser::drawing3D_parse(const ini::Configuration &conf, Drawing3D &drawing) {
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

    // Lights

    Lights3D lights;
    for (int i=0; i!=nrLights; i++) {
        Light light;

        string lightname = "Light" + to_string(i);
        vector<double> ambient = conf[lightname]["ambientLight"].as_double_tuple_or_default({0, 0, 0});
        vector<double> diffuse = conf[lightname]["diffuseLight"].as_double_tuple_or_default({0, 0, 0});
//        vector<double> specular = conf[lightname]["specular"].as_double_tuple_or_default({0, 0, 0});

        vector<double> direction_ = conf[lightname]["direction"].as_double_tuple_or_default({0, 0, 0});
        Vector3D direction;
        direction.x = direction_[0];
        direction.y = direction_[1];
        direction.z = direction_[2];
        bool infty = conf[lightname]["infinity"].as_bool_or_default(false);


        Color ambLight(ambient[0], ambient[1], ambient[2]);
        Color diffLight(diffuse[0], diffuse[1], diffuse[2]);
        light.ambientLight = ambLight;
        light.diffuseLight = diffLight;

        light.infinity = infty;
        light.direction = direction;




        lights.push_back(light);
    }
    drawing.setLights(lights);


    Solid3D s3d;

    //FIGURES
    for (int i = 0; i < nrFigures; i++) {
        string figname = "Figure" + to_string(i);
        Figure fig;
        string figType = conf[figname]["type"].as_string_or_die();
        bool lineDrawing = (figType == "LineDrawing");

        int nrPoints = conf[figname]["nrPoints"].as_int_or_default(0);
        int nrLines = conf[figname]["nrLines"].as_int_or_default(0);
        vector<double> color = conf[figname]["color"].as_double_tuple_or_default({0,0,0});
        double scale = conf[figname]["scale"].as_double_or_default(1.0);
        double rotateX = conf[figname]["rotateX"].as_double_or_die();
        double rotateY = conf[figname]["rotateY"].as_double_or_die();
        double rotateZ = conf[figname]["rotateZ"].as_double_or_die();
        vector<double> center_ = conf[figname]["center"].as_double_tuple_or_die();
        Vector3D center = Vector3D::point(center_[0], center_[1], center_[2]);
        int n = conf[figname]["n"].as_int_or_default(0);
        double height = conf[figname]["height"].as_double_or_default(0);
        double r = conf[figname]["r"].as_double_or_default(0);
        double R = conf[figname]["R"].as_double_or_default(0);
        int m = conf[figname]["m"].as_int_or_default(0);
        string inputfile = conf[figname]["inputfile"].as_string_or_default("");
        int nrIterations = conf[figname]["nrIterations"].as_int_or_default(0);
        double fractalscale = conf[figname]["fractalScale"].as_double_or_default(0);


        // Light reflection
        vector<double> ambReflec = conf[figname]["ambientReflection"].as_double_tuple_or_default({0, 0, 0});
        vector<double> diffReflec = conf[figname]["diffuseReflection"].as_double_tuple_or_default({0, 0, 0});
        Color ambientreflection = Color(ambReflec[0], ambReflec[1], ambReflec[2]);
        Color diffReflection = Color(diffReflec[0], diffReflec[1], diffReflec[2]);

        fig.setColor(Color(color[0], color[1], color[2]));
        fig.setSize(size);
        fig.setScale(scale);
        fig.setRotateX(rotateX);
        fig.setRotateY(rotateY);
        fig.setRotateZ(rotateZ);
        fig.setCenter(center);
        fig.setAmbientReflection(ambientreflection);
        fig.setDiffuseReflection(diffReflection);
        fig.setNrIterations(nrIterations);
        fig.setFractalscale(fractalscale);

        if (figType == "Cube" || figType == "FractalCube") {
            s3d.generateCube(fig);
        }
        if (figType == "Tetrahedron" || figType == "FractalTetrahedron") {
            s3d.generateTetrahedron(fig);
        }
        if (figType == "Octahedron" || figType == "FractalOctahedron") {
            s3d.generateOctahedron(fig);
        }
        if (figType == "Icosahedron" || figType == "FractalIcosahedron") {
            s3d.generateIcosahedron(fig);
        }
        if (figType == "Dodecahedron" || figType == "FractalDodecahedron") {
            s3d.generateDodecahedron(fig);
        }
        if (figType == "Sphere" || figType == "FractalSphere") {
            s3d.generateSphere(fig, n);
        }
        if (figType == "Cone" || figType == "FractalCone") {
            s3d.generateCone(fig, n, height);
        }
        if (figType == "Cylinder" || figType == "FractalCylinder") {
            s3d.generateCylinder(fig, n, height);
        }
        if (figType == "Torus" || figType == "FractalTorus") {
            s3d.generateTorus(fig, r, R, n, m);
        }

        if (figType == "3DLSystem") {
            MyLSystem3D lsys(inputfile);
            lsys.generateFigure(fig);
        };


        // 2D linedrawings:

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
        Figures3D fractal;
        s3d.generateFractal(fig, fractal, nrIterations, fractalscale);

        for (auto& f : fractal) {
            drawing.addFigure(f);
        }
        if (fractal.empty()) drawing.addFigure(fig);
    }
}

MyLSystem2D MyParser::lsys_parse(const ini::Configuration &conf) {
    std::string type = conf["General"]["type"].as_string_or_die();
    int size = conf["General"]["size"].as_int_or_die();

    vector<double> bgColor = conf["General"]["backgroundcolor"].as_double_tuple_or_die();
    string inputfile = conf["2DLSystem"]["inputfile"].as_string_or_die();
    vector<double> color = conf["2DLSystem"]["color"].as_double_tuple_or_die();
    MyLSystem2D result(inputfile);
    result.setSize(size);
    result.setColor(Color(*(color.begin()), *(color.begin()+1), *(color.begin()+2)));
    result.setBgColor(Color(*(bgColor.begin()), *(bgColor.begin()+1), *(bgColor.begin()+2)));
    return result;
}