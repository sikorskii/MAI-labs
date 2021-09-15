//
// Created by djent on 09.09.2021.
//

#include "../headers/rhombus.h"

Rhombus::Rhombus(std::istream &in) {
    std::cout << "Enter 4 points of Rhombus in clockwise or counterclockwise order::::::::::::\n";
    in >> a >> b >> c >> d;
    valid = this->isValid();
}


void Rhombus::print() {
    valid ? std::cout << "Valid " : std::cout << "Invalid!!! ";
    std::cout << "rhombus: " << a << b << c << d << '\n';
}

double Rhombus::square() {
    if (isValid())
        return sqr;

    std::cout << "Unable to calculate invalid Rhombus's square, 0 returned!\n";
    return 0;
}

bool Rhombus::isValid() {
    Point v1 = {c.x - a.x, c.y - a.y};
    Point v2 = {d.x - b.x, d.y - b.y};
    Point inter1 = {(a.x + c.x) / 2, (a.y + c.y) / 2};
    Point inter2 = {(b.x + d.x) / 2, (b.y + d.y) / 2};

    if (inter1.x == inter2.x && inter1.y == inter2.y && (v1.x * v2.x + v1.y * v2.y == 0)) {
        sqr = a.dist(c) * b.dist(d) / 2;
        return true;
    }

    std::cout << "Rhombus invalid\n";
    return false;
}

int Rhombus::VertexesNumber() {
    return 4;
}
