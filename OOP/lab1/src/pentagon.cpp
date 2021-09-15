//
// Created by djent on 09.09.2021.
//

#include "../headers/pentagon.h"

Pentagon::Pentagon(std::istream &in) {
    std::cout << "Enter 5 points of Pentagon in clockwise or counterclockwise order::::::::::::\n";
    in >> a >> b >> c >> d >> e;
    valid = this->isValid();
}

double Pentagon::square() {
    if (isValid())
        return sqr;

    std::cout << "Unable to calculate invalid Pentagon's square, 0 returned!\n";
    return 0;
}

void Pentagon::print() {
    valid ? std::cout << "Valid " : std::cout << "Invalid!!! ";
    std::cout << "Pentagon: " << a << b << c << d << e << '\n';
}

bool Pentagon::isValid() {
    double ab = a.dist(b);
    double bc = b.dist(c);
    double cd = c.dist(d);
    double de = d.dist(e);
    double ea = e.dist(a);

    double ac = a.dist(c);
    double ce = c.dist(e);

    double p1 = (ab + bc + ac) / 2;
    double p2 = (cd + de + ce) / 2;
    double p3 = (ac + ce + ea) / 2;

    double sqr1 = sqrt(p1 * (p1 - ab) * (p1 - bc) * (p1 - ac));
    double sqr2 = sqrt(p2 * (p2 - cd) * (p2 - de) * (p2 - ce));
    double sqr3 = sqrt(p3 * (p3 - ac) * (p3 - ce) * (p3 - ea));

    if (sqr1 == 0 || sqr2 == 0 || sqr3 == 0) {
        std::cout << "Pentagon invalid\n";
        return false;
    }

    sqr = sqr1 + sqr2 + sqr3;
    return true;
}

int Pentagon::VertexesNumber() {
    return 5;
}
