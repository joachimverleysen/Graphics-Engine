//
// Created by joachimverleysen on 3/29/24.
//

#ifndef MYENGINE_MYLSYSTEM3D_H
#define MYENGINE_MYLSYSTEM3D_H

#include "LineDrawer.h"
#include "../tools/vector3d.h"
#include "Figure.h"
#include <string>
#include <vector>
#include <set>
#include <cmath>
#include <map>
#include <fstream>
#include <algorithm>


class MyLSystem3D {
    std::string inputfile;
    std::set<char> alphabet;
    double Angle;
    std::string initiator;
    double starting_angle;
    std::string finalString;    // Result string after the rules have been applied (i times)
    std::map<char, int> drawFunction;
    vector<Vector3D> points;
    vector<Face> faces;
    Lines2D lines;
    std::map<char, std::string> rules;
    int size;
    Color color;
    Color bgColor;
    unsigned int iterations;

public:
    MyLSystem3D(std::string inputfile) : inputfile(inputfile) {parser(inputfile);}

    int parser(std::string inputfile);

    void setAlphabet(const std::set<char> &alphabet);

    void setAngle(double angle);

    void setInitiator(const std::string &initiator);

    void setStartingAngle(double startingAngle);


    void computePoints();

    void _str2Points(const std::string &str);

    void applyReplacement(std::string &s);

    void generateFigure(Figure &fig);

    vector<Vector3D> const getPoints();

    void setPoints(const vector<Vector3D> &points);

    void setFaces(const vector<Face> &faces);
};


#endif //MYENGINE_MYLSYSTEM3D_H
