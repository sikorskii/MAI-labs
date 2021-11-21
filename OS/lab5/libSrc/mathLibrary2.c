//
// Created by aldes on 18.11.2021.
//
#include <math.h>

#include "mathHeader.h"

double integrateSin(double leftBound, double rightBound, double step) {
    double sinNumeric = 0;

    double left = leftBound;
    double right = left + step;
    double point = (left + right) / 2;
    while (right <= rightBound) {
        sinNumeric += (sin(point) * step);
        left = right;
        right += step;
        point = (left + right) / 2;
    }

    return sinNumeric;
}

double calculatePI(int seriesLength) {
    double PI = 1;

    for (double i = 1; i < seriesLength; ) {
        PI *= (2 * i / (2 * i - 1) * 2 * i / (2 * i + 1));
        i += 1;
    }

    return 2 * PI;
}