//
// Created by joachimverleysen on 2/27/24.
//

#include "LSystem.h"
#include "l_parser.h"

int LSystem::parser(string inputfile) {
    LParser::LSystem2D l_system;
    std::ifstream input_stream(inputfile);
    input_stream >> l_system;
    input_stream.close();
    setAlphabet(l_system.get_alphabet());
    setAngle(l_system.get_angle()* (M_PI / 180));   // Conversion to radians
    setStartingAngle(l_system.get_starting_angle() * (M_PI / 180));  // Conversion to radians
    if (l_system.get_starting_angle() < 0 ) {
//        cout<<"angle<0"<<endl;
    }
    setInitiator(l_system.get_initiator());
    l_system.get_alphabet();
    iterations = l_system.get_nr_iterations();

    // Set drawFunction (map) + rules func (map)
    for (char c : alphabet) {
        rules[c] = l_system.get_replacement(c);

        if (l_system.draw(c)) drawFunction[c] = 1;
        else drawFunction[c] = 0;
    }

    return 0;
}

void LSystem::setAlphabet(const set<char> &alphabet) {
    LSystem::alphabet = alphabet;
}

void LSystem::setAngle(double angle) {
    LSystem::Angle = angle;
}

void LSystem::setInitiator(const string &initiator) {
    LSystem::initiator = initiator;
}

void LSystem::setStartingAngle(double startingAngle) {
    starting_angle = startingAngle;
}




void LSystem::_str2Points(const string &str, vector<Point2D> &points) {
    double angle=starting_angle;
    Point2D currPos(0, 0);

    // Converts the initiator string to a vector of _points
    for (char c : str) {
            if (c == '+') {
                angle += Angle;
                continue;
            }
            else if (c == '-') {
                angle -= Angle;
                continue;
            }
            else if (alphabet.find(c) == alphabet.end()) cerr<<"Undefined char"<<endl;
            else  {
                Point2D nextPt = _computeNextPoint(currPos, angle);
                if (drawFunction[c] == 1) {
                    points.emplace_back(currPos);
                    points.emplace_back(nextPt);
                }
                currPos = nextPt;
            }
    }

}

Point2D LSystem::_computeNextPoint(Point2D &p1, double angle) {
    double x2 = p1.x + cos(angle);
    double y2 = p1.y + sin(angle);

    Point2D result(x2, y2);
    return result;
}

void LSystem::recursiveReplace(string &str, int iters, Point2D &currPos, double angl, vector<Point2D> &_points) {
    for (char c : str) {
        if (iters == 0) {
            if (c == '+') {
            angl += Angle;
            }
            else if (c == '-') {
                angl-= Angle;
            }
            else if (alphabet.find(c) == alphabet.end()) cerr<<"Undefined char"<<endl;
            else {
                Point2D nextPos = _computeNextPoint(currPos, angl);
                if (drawFunction[c]==1) {
                    _points.push_back(currPos);
                }
                _points.push_back(nextPos);
                currPos = nextPos;
            }
        }
        else {
            recursiveReplace(rules[c], iters - 1, currPos, angl, _points);
        }
}


}

void LSystem::applyReplacement(string &s) {
    string str_new;
    for (char c : s) {
        if (c == '+') str_new+='+';
        else if (c == '-') str_new+='-';
        else if (alphabet.find(c) == alphabet.end()) cerr<<"Undefined char"<<endl;
        else str_new += rules[c];
    }
    s = str_new;
}

void LSystem::computePoints(vector<Point2D> &points) {
    // Creates _points-vector based off the initiator string, applying the replacement rules <iterations> times
    string s = initiator;
    for (unsigned int i=0; i<iterations; i++) {
        string str_new;
        applyReplacement(s);
    }
    _str2Points(s, points);
}

void LSystem::setPoints(const vector<Point2D> &points) {
    LSystem::Points = points;
}




