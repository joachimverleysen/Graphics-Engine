#include "../tools/easy_image.h"
#include "../tools/ini_configuration.h"
#include "LineDrawer.h"
#include "LSystem.h"
#include "../tools/vector3d.h"
#include "Figure.h"
#include "Drawing3D.h"
#include "Solid3D.h"

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <cmath>

using namespace std;


double degree_to_rad(double deg) {
    return deg*M_PI/180;
}

Point2D _do_point_projection(const Vector3D &point, const double d= 1) {
    // Point should already be in eye-coordinate system
    double x_new = d*point.x/-point.z;
    double y_new = d*point.y/-point.z;
    Point2D p(x_new, y_new);
    return p;
}

struct polars {
    double r;
    double theta;
    double phi;
    polars(double r, double theta, double phi) : r(r), theta(theta), phi(phi) {};
};

polars toPolar(const Vector3D &point) {
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

Matrix eyePointTrans(const Vector3D &eyepoint) {
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

void _convert_fig_to_eyesys(Figure &fig, const Vector3D &eye) {
    Matrix eyeTrans = eyePointTrans(eye);
    vector<Vector3D> convertedPoints;
    for (const auto& p : fig.getPoints()) {
        Vector3D point_new = p*eyeTrans;
        convertedPoints.push_back(point_new);
    }
    fig.setPoints(convertedPoints);
}

vector<Point2D> _compute_proj_points(Figure &fig) {
    vector<Point2D> result;
    for (auto p : fig.getPoints()) {
        Point2D proj = _do_point_projection(p);
        result.push_back(proj);
    }
    return result;
}

Lines2D _getLineArray(const vector<Point2D> &points, Color col) {
    // Pairs the Points in the pointArray 2 by 2 to create lines
    Lines2D result;
    for (int i=0; i<points.size(); i+=2) {
        Line2D line(points[i], points[i+1], col);
        result.push_back(line);
    }
    return result;
}

Lines2D _computeLines(Figure &fig) {
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
            Lines2D lines = _getLineArray(points, col);
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
            Lines2D lines = _getLineArray(points, col);
            result.insert(result.end(), lines.begin(), lines.end());
        }

    }
    return result;
}


Matrix get_rot_y(const double angle_deg) {
    double angle = degree_to_rad(angle_deg);
    Matrix a;
    a(1,1) = cos(angle);
    a(1,3) = -sin(angle);
    a(3,1) = sin(angle);
    a(3,3) = cos(angle);
    return a;
}

Matrix get_rot_x(const double angle_deg) {
    double angle = degree_to_rad(angle_deg);
    Matrix a;
    a(2,2) = cos(angle);
    a(2,3) = sin(angle);
    a(3,2) = -sin(angle);
    a(3,3) = cos(angle);
    return a;
}

Matrix get_rot_z(const double angle_deg) {
    double angle = degree_to_rad(angle_deg);
    Matrix a;
    a(1,1) = cos(angle);
    a(1,2) = sin(angle);
    a(2,1) = -sin(angle);
    a(2,2) = cos(angle);
    return a;
}

Matrix get_scale_mtx(const double scale) {
    Matrix a;
    a(1,1) = scale;
    a(2,2) = scale;
    a(3,3) = scale;
    return a;
}

Matrix get_translate_mtx(const Vector3D &vector) {
    Matrix a;
    a(4,1) = vector.x;
    a(4,2) = vector.y;
    a(4,3) = vector.z;
    return a;
}

void _doTransitions(Figure &fig) {
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


Lines2D do_projection(Drawing3D &drawing) {
    Lines2D result;
    Figures3D figs = drawing.getFigures();
    for (Figure &fig : figs) {
        _doTransitions(fig);
        _convert_fig_to_eyesys(fig, drawing.getEye());
        vector<Point2D> proj_points = _compute_proj_points(fig);
        fig.setProjPoints(proj_points);
//        vector<Face> faces = fig.getFaces();
        Lines2D lines = _computeLines(fig);
        result.insert(result.end(), lines.begin(), lines.end());
    }
    return result;
}

void drawing3D_parse(const ini::Configuration &conf, Drawing3D &drawing) {
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
    for (int i=0; i<nrFigures; i++) {
        string figname = "Figure"+ to_string(i);
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
        Vector3D center  = Vector3D::point(center_[0], center_[1], center_[2]);

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

        //POINTS
        vector<Vector3D> points;
        for (int i=0; i<nrPoints; i++) {
            string pntname = "point" + to_string(i);
            vector<double> _pt = conf[figname][pntname].as_double_tuple_or_die();
            Vector3D point = Vector3D::point(_pt[0], _pt[1], _pt[2]);
            points.push_back(point);
        }
        //LINES
        vector<Face> faces;
        for (int i=0; i<nrLines; i++) {
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

LSystem lsys_parse(const ini::Configuration &conf) {
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

img::EasyImage generateImage(const ini::Configuration &conf) {
    img::EasyImage image;
    Figures3D figs;
    Drawing3D drawing;
    LineDrawer ld;
    drawing3D_parse(conf, drawing);

//    Solid3D solid3D;
//    Figure cube;
//    solid3D.generateCube(cube);
//    figs.push_back(cube);
//    drawing.setFigures(figs);
//    drawing.setEye(Vector3D::point(100, 30, 30));


    Lines2D lines = do_projection(drawing);
    Color bgCol = drawing.getBgColor();
    image = ld.draw2Dlines(lines, drawing.getSize(), bgCol);

    return image;
}



int main(int argc, char const* argv[])
{
    int retVal = 0;
    try
    {
        std::vector<std::string> args = std::vector<std::string>(argv+1, argv+argc);
        if (args.empty()) {
            std::ifstream fileIn("filelist");
            std::string filelistName;
            while (std::getline(fileIn, filelistName)) {
                args.push_back(filelistName);
            }
        }
        for(std::string fileName : args)
        {
            ini::Configuration conf;
            try
            {
                std::ifstream fin(fileName);
                if (fin.peek() == std::istream::traits_type::eof()) {
                    std::cout << "Ini file appears empty. Does '" <<
                              fileName << "' exist?" << std::endl;
                    continue;
                }
                fin >> conf;
                fin.close();
            }
            catch(ini::ParseException& ex)
            {
                std::cerr << "Error parsing file: " << fileName << ": " << ex.what() << std::endl;
                retVal = 1;
                continue;
            }

            img::EasyImage image = generateImage(conf);
            if(image.get_height() > 0 && image.get_width() > 0)
            {
                std::string::size_type pos = fileName.rfind('.');
                if(pos == std::string::npos)
                {
                    //filename does not contain a '.' --> append a '.bmp' suffix
                    fileName += ".bmp";
                }
                else
                {
                    fileName = fileName.substr(0,pos) + ".bmp";
                }
                try
                {
                    std::ofstream f_out(fileName.c_str(),std::ios::trunc | std::ios::out | std::ios::binary);
                    f_out << image;

                }
                catch(std::exception& ex)
                {
                    std::cerr << "Failed to write image to file: " << ex.what() << std::endl;
                    retVal = 1;
                }
            }
            else
            {
                std::cout << "Could not generate image for " << fileName << std::endl;
            }
        }
    }
    catch(const std::bad_alloc &exception)
    {
        //When you run out of memory this exception is thrown. When this happens the return value of the program MUST be '100'.
        //Basically this return value tells our automated drawLine2D scripts to run your engine on a pc with more memory.
        //(Unless of course you are already consuming the maximum allowed amount of memory)
        //If your engine does NOT adhere to this requirement you risk losing Points because then our scripts will
        //mark the drawLine2D as failed while in reality it just needed a bit more memory
        std::cerr << "Error: insufficient memory" << std::endl;
        retVal = 100;
    }
    return retVal;
}
