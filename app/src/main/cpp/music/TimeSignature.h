//
// Created by pgg on 10/08/17.
//

#ifndef APISCORE_TIMESIGNATURE_H
#define APISCORE_TIMESIGNATURE_H


#include "Value.h"

class TimeSignature {
public:
    TimeSignature(unsigned int beatCount, unsigned int beatNumber);
    TimeSignature(unsigned int beatCount, UnsignedFixedPoint const& beatValue);

    UnsignedFixedPoint getBarValue();
private:
    unsigned int beatCount_;
    UnsignedFixedPoint beatValue_;
};


#endif //APISCORE_TIMESIGNATURE_H
