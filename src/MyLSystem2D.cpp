//
// Created by joachimverleysen on 2/27/24.
//

#include "MyLSystem2D.h"
#include "../tools/l_parser.h"

int MyLSystem2D::parser(string inputfile) {
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

void MyLSystem2D::setAlphabet(const set<char> &alphabet) {
    MyLSystem2D::alphabet = alphabet;
}

void MyLSystem2D::setAngle(double angle) {
    MyLSystem2D::Angle = angle;
}

void MyLSystem2D::setInitiator(const string &initiator) {
    MyLSystem2D::initiator = initiator;
}

void MyLSystem2D::setStartingAngle(double startingAngle) {
    starting_angle = startingAngle;
}

typedef pair<Point2D, double> State;



void MyLSystem2D::_str2Points(const string &str, vector<Point2D> &points) {
    double angle=starting_angle;
    Point2D currPos(0, 0);
    State currState(currPos, starting_angle);
    vector<State> stack;

    // Converts the initiator string to a vector of _points
    for (char c : str) {
            if (c == '+') {
                currState.second += Angle;
                continue;
            }
            else if (c == '-') {
                currState.second -= Angle;
                continue;
            }
            else if (c == '(') {
                stack.push_back(currState);
            }
            else if (c == ')') {
                currState = stack.back();
                stack.pop_back();
            }
            else if (alphabet.find(c) == alphabet.end()) {
                cerr<<"Undefined char"<<endl;
            }
            else  {
                Point2D nextPt = _computeNextPoint(currState.first, currState.second);
                if (drawFunction[c] == 1) {
                    points.emplace_back(currState.first);
                    points.emplace_back(nextPt);
                }
                currState.first = nextPt;
                currState.first = currState.first;

            }
    }

}

Point2D MyLSystem2D::_computeNextPoint(Point2D &p1, double angle) {
    double x2 = p1.x + cos(angle);
    double y2 = p1.y + sin(angle);

    Point2D result(x2, y2);
    return result;
}


void MyLSystem2D::applyReplacement(string &s) {
    string str_new;
    for (char c : s) {
        if (c == '+') str_new+='+';
        else if (c == '-') str_new+='-';
        else if (c == '(') str_new+='(';
        else if (c == ')') str_new+=')';
        else if (alphabet.find(c) == alphabet.end()) cerr<<"Undefined char"<<endl;
        else str_new += rules[c];
    }
    s = str_new;
}

void MyLSystem2D::computePoints(vector<Point2D> &points) {
    // Creates _points-vector based off the initiator string, applying the replacement rules <iterations> times
    string s = initiator;
    for (unsigned int i=0; i<iterations; i++) {
        string str_new;
        applyReplacement(s);
    }
    _str2Points(s, points);
}

void MyLSystem2D::setPoints(const vector<Point2D> &points) {
    MyLSystem2D::points = points;
}

vector<Point2D> const MyLSystem2D::getPoints() {
    vector<Point2D> result;
    computePoints(result);
    return result;
}

const string &MyLSystem2D::getInputfile() const {
    return inputfile;
}

void MyLSystem2D::setInputfile(const string &inputfile) {
    MyLSystem2D::inputfile = inputfile;
}

const set<char> &MyLSystem2D::getAlphabet() const {
    return alphabet;
}

double MyLSystem2D::getAngle() const {
    return Angle;
}

const string &MyLSystem2D::getInitiator() const {
    return initiator;
}

double MyLSystem2D::getStartingAngle() const {
    return starting_angle;
}

const string &MyLSystem2D::getFinalString() const {
    return finalString;
}

void MyLSystem2D::setFinalString(const string &finalString) {
    MyLSystem2D::finalString = finalString;
}

const map<char, int> &MyLSystem2D::getDrawFunction() const {
    return drawFunction;
}

void MyLSystem2D::setDrawFunction(const map<char, int> &drawFunction) {
    MyLSystem2D::drawFunction = drawFunction;
}

const Lines2D &MyLSystem2D::getLines() const {
    return lines;
}

void MyLSystem2D::setLines(const Lines2D &lines) {
    MyLSystem2D::lines = lines;
}

const map<char, string> &MyLSystem2D::getRules() const {
    return rules;
}

void MyLSystem2D::setRules(const map<char, string> &rules) {
    MyLSystem2D::rules = rules;
}

int MyLSystem2D::getSize() const {
    return size;
}

void MyLSystem2D::setSize(int size) {
    MyLSystem2D::size = size;
}

const Color &MyLSystem2D::getColor() const {
    return color;
}

void MyLSystem2D::setColor(const Color &color) {
    MyLSystem2D::color = color;
}

const Color &MyLSystem2D::getBgColor() const {
    return bgColor;
}

void MyLSystem2D::setBgColor(const Color &bgColor) {
    MyLSystem2D::bgColor = bgColor;
}

unsigned int MyLSystem2D::getIterations() const {
    return iterations;
}

void MyLSystem2D::setIterations(unsigned int iterations) {
    MyLSystem2D::iterations = iterations;
}




