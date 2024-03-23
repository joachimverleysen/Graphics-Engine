//
// Created by joachimverleysen on 2/27/24.
//

#ifndef MYENGINE_LSYSTEM_H
#define MYENGINE_LSYSTEM_H

#include "LineDrawer.h"

#include <string>
#include <vector>
#include <set>
#include <cmath>
#include <map>
#include <fstream>
#include <algorithm>

using namespace std;

class Color;

class LSystem {
public:
    string inputfile;
    set<char> alphabet;
    double Angle;
    string initiator;
    double starting_angle;
    string finalString;    // Result string after the rules rules have been applied (i times)
    map<char, int> drawFunction;
    vector<Point2D> Points;
    Lines2D lines;
    map<char, string> rules;
    int size;
    Color color;
    Color bgColor;
    unsigned int iterations;

public:
    LSystem(string inputfile) : inputfile(inputfile) {parser(inputfile);}

    int parser(string inputfile);

    void computePoints(vector<Point2D> &points);

    void _str2Points(const string &str, vector<Point2D> &points);

    Point2D _computeNextPoint(Point2D &p1, double angle);

    void setAlphabet(const set<char> &alphabet);

    void setAngle(double angle);

    void setInitiator(const string &initiator);

    void setStartingAngle(double startingAngle);

    void setPoints(const vector<Point2D> &points);

    void applyReplacement(string &s);

    void recursiveReplace(string &s, int iterations, Point2D &currPos, double angl, vector<Point2D> &_points);
};



#endif //MYENGINE_LSYSTEM_H
