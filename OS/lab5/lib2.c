//
// Created by aldes on 28.11.2021.
//

#include "lib1.h"
#include <math.h>

double integrateSin(double leftBound, double rightBound, double step) {
    double sinNumeric = 0;

    double left = leftBound;
    double right = left + step;
    while (right <= rightBound) {
        sinNumeric += (sin(left) + sin(right)) / 2 * step;
        left = right;
        right += step;
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