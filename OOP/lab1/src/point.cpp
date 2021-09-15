//
// Created by djent on 09.09.2021.
//

#include "../headers/point.h"

#include <cmath>

Point::Point() : x(0.0), y(0.0) {}
Point::Point(double x, double y) : x(x), y(y) {}


double Point::dist(Point other) const {
    double dx = (other.x - x);
    double dy = (other.y - y);
    return std::sqrt(dx*dx + dy*dy);
}

std::istream& operator>>(std::istream& is, Point& p) {
    is >> p.x >> p.y;
    return is;
}

std::ostream& operator<<(std::ostream& os, Point& p) {
    os << "(" << p.x << ", " << p.y << ")";
    return os;
}