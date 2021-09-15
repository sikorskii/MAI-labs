//
// Created by djent on 09.09.2021.
//

#ifndef LAB1_POINT_H
#define LAB1_POINT_H

#include <iostream>

class Point {
public:
    Point();
    Point(double x, double y);

    double dist(Point other) const;

    friend std::istream& operator>>(std::istream& is, Point& p);
    friend std::ostream& operator<<(std::ostream& os, Point& p);

    double x;
    double y;
};


#endif //LAB1_POINT_H
