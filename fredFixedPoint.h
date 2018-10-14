#ifndef FREDFIXEDPOINT_H
#define FREDFIXEDPOINT_H
#include "fredMathUtil.h"

//a fixed point value that is saved using an int type
template<typename basetype, int decimals> class FixedPoint {
    basetype internalValue = 0;
public:
    FixedPoint(double value) :
            internalValue(value * powerOf10<decimals, uint32_t>()) {
    }

    double getValue() {
        return internalValue / (double) powerOf10<decimals, uint32_t>();
    }

    void setValue(double value) {
        internalValue = value * powerOf10<decimals, uint32_t>();
    }
};

#endif
