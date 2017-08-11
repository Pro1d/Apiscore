//
// Created by pgg on 10/08/17.
//

#ifndef APISCORE_NOTEVALUE_H
#define APISCORE_NOTEVALUE_H

#include <cstdint>
#include "../FixedPoint.h"
/*
enum NamedNoteValues : uint16_t {
    SixteenthNote   = 1 << (-4 + UnsignedFixedPoint.point()), // french: "double croche"
    EighthNote      = 1 << (-3 + FIXED_POINT_NOTE_VALUE), // french: "croche"
    QuarterNote     = 1 << (-2 + FIXED_POINT_NOTE_VALUE), // french: "noire"
    HalfNote        = 1 << (-1 + FIXED_POINT_NOTE_VALUE), // french: "blanche"
    WholeNote       = 1 << (+0 + FIXED_POINT_NOTE_VALUE)  // french: "ronde"
};*/

class Value {
public:
    Value(UnsignedFixedPoint v);
    /**
     * set value to 1/N
     * @param N must be a power of 2
     */
    Value(unsigned int N);
    Value dot();
    Value const& operator*(unsigned int v);

    UnsignedFixedPoint const& getUFP() const { return value_; }

private:
    UnsignedFixedPoint value_;
};


#endif //APISCORE_NOTEVALUE_H
