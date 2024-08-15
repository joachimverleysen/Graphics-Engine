//
// Created by joachimverleysen on 3/29/24.
//

#include "MyLSystem3D.h"
#include "../tools/l_parser.h"
#include "../tools/vector3d.h"


void MyLSystem3D::setAlphabet(const std::set<char> &alphabet) {
    MyLSystem3D::alphabet = alphabet;
}

void MyLSystem3D::setAngle(double angle) {
    Angle = angle;
}

void MyLSystem3D::setInitiator(const std::string &initiator) {
    MyLSystem3D::initiator = initiator;
}

void MyLSystem3D::setStartingAngle(double startingAngle) {
    starting_angle = startingAngle;
}

int MyLSystem3D::parser(std::string inputfile) {
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

struct State3D {
    Vector3D H;
    Vector3D L;
    Vector3D U;
    Vector3D currPos;
    State3D(const Vector3D &h, const Vector3D &l, const Vector3D &u, const Vector3D &currPos) : H(h), L(l), U(u),
                                                                                               currPos(currPos) {}
};


void MyLSystem3D::_str2Points(const std::string &str) {
    vector<Vector3D> _points;
    vector<Face> _faces;
    double angle=starting_angle;
    Vector3D currPos = Vector3D::point(0, 0, 0);
    Vector3D nextPt = Vector3D::point(0,0,0);
    Vector3D H = Vector3D::vector(1, 0, 0);
    Vector3D L = Vector3D::vector(0, 1, 0);
    Vector3D U = Vector3D::vector(0, 0, 1);
    State3D currState{H, L, U, currPos};
    vector<vector<Vector3D>> stack;
    int ptCounter=0;
    // Converts the initiator string to a vector of _points
    Vector3D Hnew, Lnew, Unew;
    for (char c : str) {
        if (c == '+') {
            Hnew=H*cos(angle)+L*sin(angle);
            Lnew=-H*sin(angle)+L*cos(angle);
            H=Hnew;
            L = Lnew;

            continue;
        }
        else if (c == '-') {
            Hnew=H*cos(-angle)+L*sin(-angle);
            Lnew=-H*sin(-angle)+L*cos(-angle);
            H=Hnew;
            L=Lnew;

            continue;
        }
        else if (c == '^') {
            Hnew=H*cos(angle)+U*sin(angle);
            Unew=-H*sin(angle)+U*cos(angle);
            H=Hnew;
            U=Unew;

            continue;
        }
        else if (c == '&') {
            Hnew=H*cos(-angle)+U*sin(-angle);
            Unew=-H*sin(-angle)+U*cos(-angle);
            H=Hnew;
            U=Unew;
            continue;
        }
        else if (c == '\\') {
            Lnew=L*cos(angle)-U*sin(angle);
            Unew=L*sin(angle)+U*cos(angle);
            L=Lnew;
            U=Unew;
            continue;
        }
        else if (c == '/') {
            Lnew=L*cos(-angle)-U*sin(-angle);
            Unew=L*sin(-angle)+U*cos(-angle);
            L=Lnew;
            U=Unew;
            continue;
        }
        else if (c == '|') {
            H*=-1;
            L*=-1;
            continue;
        }
        else if (c == '(') {
            stack.push_back({nextPt, H, L, U});
            continue;
        }
        else if (c == ')') {

            std::vector<Vector3D> prevPos = stack.back();
            stack.pop_back();

            nextPt = prevPos[0];
            H = prevPos[1];
            L = prevPos[2];
            U = prevPos[3];
        }
        else if (alphabet.find(c) == alphabet.end()) {
            throw std::invalid_argument("Unrecognized char");
        }
        else  {
            currPos = nextPt;

            nextPt = currPos+H;
            _points.emplace_back(currPos);
            _points.emplace_back(nextPt);

            if (drawFunction[c] == 1) {
                _faces.emplace_back(Face({ptCounter, ptCounter + 1}));
            }
            ptCounter++;
        }
    }
    setFaces(_faces);
    setPoints(_points);
}

//todo: combine lsystem3d and 2d in one func

void MyLSystem3D::applyReplacement(std::string &s) {
    std::string str_new;
    for (char c : s) {
        if (c == '+') str_new+='+';
        else if (c == '-') str_new+='-';
        else if (c == '&') str_new+='&';
        else if (c == '/') str_new+='/';
        else if (c == '\\') str_new+='\\';
        else if (c == '|') str_new+='|';
        else if (c == '^') str_new+='^';
        else if (c == '(') str_new+='(';
        else if (c == ')') str_new+=')';
        else if (alphabet.find(c) == alphabet.end()) {
            throw std::invalid_argument("Undefined char");
        }
        else str_new += rules[c];
    }
    s = str_new;
}

void MyLSystem3D::computePoints() {
    // Creates _points-vector based off the initiator string, applying the replacement rules <iterations> times
    std::string s = initiator;
    for (unsigned int i=0; i<iterations; i++) {
        std::string str_new;
        applyReplacement(s);
    }
    _str2Points(s);
}

void MyLSystem3D::generateFigure(Figure &fig) {
    computePoints();
    fig.setPoints(getPoints());
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
