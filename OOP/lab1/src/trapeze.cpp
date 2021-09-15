//
// Created by djent on 07.09.2021.
//


#include "../headers/trapeze.h"

Trapeze::Trapeze(std::istream &in) {
    std::cout << "Enter 4 points of Trapeze in clockwise or counterclockwise order::::::::::::\n";
    in >> a >> b >> c >> d;
    valid = this->isValid();
}

void Trapeze::print() {
    valid ? std::cout << "Valid " : std::cout << "Invalid!!! ";
    std::cout << "trapeze: " << a << b << c << d << '\n';
}

double Trapeze::square() {
    if (!valid) {
        std::cout << "Unable to calculate invalid Trapeze's square, returned 0!\n";
        return 0;
    }

    return sqr;
}



bool Trapeze::isValid() {
    Point v1 = {b.x - a.x, b.y - a.y};
    Point v2 = {c.x - b.x, c.y - b.y};
    Point v3 = {d.x - c.x, d.y - c.y};
    Point v4 = {a.x - d.x, a.y - d.y};

    struct two_vectors {
        Point v1, v2;
        bool is_collinear = false;
    };

    two_vectors duet1 {v1, v3}, duet2 {v2, v4};

    duet1.is_collinear = (v1.x == 0 && v3.x ==0) || (v1.y == 0 && v3.y == 0) || ((v1.x / v3.x) == (v1.y / v3.y));
    duet2.is_collinear = (v2.x == 0 && v4.x ==0) || (v2.y == 0 && v4.y == 0) || ((v2.x / v4.x) == (v2.y / v4.y));

    double ab = a.dist(b);
    double bc = b.dist(c);
    double cd = c.dist(d);
    double da = d.dist(a);

    if (duet1.is_collinear || duet2.is_collinear) {
        double p1 = (ab + bc + a.dist(c)) / 2;
        double p2 = (da + cd + a.dist(c)) / 2;

        sqr = sqrt(p1 * (p1 - ab) * (p1 - bc) * (p1 - a.dist(c)))
                + sqrt(p2 * (p2 - da) * (p2 - cd) * (p2 - a.dist(c)));

        if (sqr > 0)
            return true;

        std::cout << "Trapeze invalid\n";
        return false;
    }

    std::cout << "Trapeze invalid\n";
    return false;
}

int Trapeze::VertexesNumber() {
    return 4;
}
