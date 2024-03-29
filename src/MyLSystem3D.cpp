//
// Created by joachimverleysen on 3/29/24.
//

#include "MyLSystem3D.h"
#include "../tools/l_parser.h"
#include "../tools/vector3d.h"


void MyLSystem3D::setAlphabet(const set<char> &alphabet) {
    MyLSystem3D::alphabet = alphabet;
}

void MyLSystem3D::setAngle(double angle) {
    Angle = angle;
}

void MyLSystem3D::setInitiator(const string &initiator) {
    MyLSystem3D::initiator = initiator;
}

void MyLSystem3D::setStartingAngle(double startingAngle) {
    starting_angle = startingAngle;
}

int MyLSystem3D::parser(string inputfile) {
    LParser::LSystem3D l_system;
    std::ifstream input_stream(inputfile);
    input_stream >> l_system;
    input_stream.close();
    setAlphabet(l_system.get_alphabet());
    setAngle(l_system.get_angle()* (M_PI / 180));   // Conversion to radians
    setStartingAngle(l_system.get_angle() * (M_PI / 180));  // Conversion to radians
    if (l_system.get_angle() < 0 ) {
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

void MyLSystem3D::_str2Points(const string &str) {
    vector<Vector3D> _points;
    vector<Face> _faces;
    double angle=starting_angle;
    Vector3D currPos = Vector3D::point(0, 0, 0);
    _points.emplace_back(currPos);
    Vector3D H = Vector3D::vector(1, 0, 0);
    Vector3D L = Vector3D::vector(0, 1, 0);
    Vector3D U = Vector3D::vector(0, 0, 1);
    int ptCounter=0;
    // Converts the initiator string to a vector of _points
    for (char c : str) {
        if (c == '+') {
            H=H*cos(angle)+L*sin(angle);
            L=-H*sin(angle)+L*cos(angle);
            continue;
        }
        else if (c == '-') {
            H=H*cos(-angle)+L*sin(-angle);
            L=-H*sin(-angle)+L*cos(-angle);
            continue;
        }
        else if (c == '^') {
            H=H*cos(angle)+U*sin(angle);
            U=-H*sin(angle)+U*cos(angle);
            continue;
        }
        else if (c == '&') {
            H=H*cos(-angle)+U*sin(-angle);
            U=-H*sin(-angle)+U*cos(-angle);
            continue;
        }
        else if (c == '\\') {
            L=L*cos(angle)-U*sin(angle);
            U=L*sin(angle)+U*cos(angle);
            continue;
        }
        else if (c == '/') {
            L=L*cos(-angle)-U*sin(-angle);
            U=L*sin(-angle)+U*cos(-angle);
            continue;
        }
        else if (c == '|') {
            H=-H;
            L=-L;
            continue;
        }
        else if (alphabet.find(c) == alphabet.end()) cerr<<"Undefined char"<<endl;
        else  {
            Vector3D nextPt = currPos+H;
            _points.emplace_back(nextPt);

            if (drawFunction[c] == 1) {
                _faces.emplace_back(Face({ptCounter, ptCounter + 1}));
            }
            currPos = nextPt;
            ptCounter++;
        }
    }
    setFaces(_faces);
    setPoints(_points);
}

void MyLSystem3D::applyReplacement(string &s) {
    string str_new;
    for (char c : s) {
        if (c == '+') str_new+='+';
        else if (c == '-') str_new+='-';
        else if (c == '&') str_new+='&';
        else if (c == '/') str_new+='/';
        else if (c == '\\') str_new+='\\';
        else if (c == '|') str_new+='|';
        else if (c == '^') str_new+='^';
        else if (alphabet.find(c) == alphabet.end()) cerr<<"Undefined char"<<endl;
        else str_new += rules[c];
    }
    s = str_new;
}

void MyLSystem3D::computePoints() {
    // Creates _points-vector based off the initiator string, applying the replacement rules <iterations> times
    string s = initiator;
    for (unsigned int i=0; i<iterations; i++) {
        string str_new;
        applyReplacement(s);
    }
    _str2Points(s);
}

void MyLSystem3D::generateFigure(Figure &fig) {
    computePoints();
    fig.setPoints(getPoints());
    fig.setColor(color);
    fig.setFaces(faces);
}

vector<Vector3D> const MyLSystem3D::getPoints() {
    return points;
}

void MyLSystem3D::setPoints(const vector<Vector3D> &points) {
    MyLSystem3D::points = points;
}

void MyLSystem3D::setFaces(const vector<Face> &faces) {
    MyLSystem3D::faces = faces;
}
