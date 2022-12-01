//
// Created by zbv11 on 04-Oct-22.
//

#include "mmath.h"

int mmath::sign(int x) {
    return (x > 0) - (x < 0);
}

bool mmath::SidedCompare(float x, float y, bool greaterThan) {
    return greaterThan == (x > y);
}

