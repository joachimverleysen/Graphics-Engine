//
// Created by joachimverleysen on 2/27/24.
//

#ifndef MYENGINE_MYLSYSTEM2D_H
#define MYENGINE_MYLSYSTEM2D_H

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

class MyLSystem2D {
    vector<Point2D> points;

private:
    string inputfile;
    set<char> alphabet;
    double Angle;
    string initiator;
    double starting_angle;
    string finalString;    // Result string after the rules rules have been applied (i times)
    map<char, int> drawFunction;
    Lines2D lines;
    map<char, string> rules;
    int size;
    Color color;
    Color bgColor;
    unsigned int iterations;

public:
    MyLSystem2D(string inputfile) : inputfile(inputfile) {parser(inputfile);}

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

    vector<Point2D> const getPoints();

    const string &getInputfile() const;

    void setInputfile(const string &inputfile);

    const set<char> &getAlphabet() const;

    double getAngle() const;

    const string &getInitiator() const;

    double getStartingAngle() const;

    const string &getFinalString() const;

    void setFinalString(const string &finalString);

    const map<char, int> &getDrawFunction() const;

    void setDrawFunction(const map<char, int> &drawFunction);

    const Lines2D &getLines() const;

    void setLines(const Lines2D &lines);

    const map<char, string> &getRules() const;

    void setRules(const map<char, string> &rules);

    int getSize() const;

    void setSize(int size);

    const Color &getColor() const;

    void setColor(const Color &color);

    const Color &getBgColor() const;

    void setBgColor(const Color &bgColor);

    unsigned int getIterations() const;

    void setIterations(unsigned int iterations);
};



#endif //MYENGINE_MYLSYSTEM2D_H
