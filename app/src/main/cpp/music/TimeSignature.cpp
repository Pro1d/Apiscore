//
// Created by pgg on 10/08/17.
//

#include "TimeSignature.h"

TimeSignature::TimeSignature(unsigned int beatCount, Value const &beatValue) :
    beatCount_(beatCount),
    beatValue_(beatValue)
{

}

TimeSignature::TimeSignature(unsigned int beatCount, unsigned int beatNumber) :
    beatCount_(beatCount),
    beatValue_(beatNumber)
{

}

UnsignedFixedPoint TimeSignature::getBarValue()
{
    return beatValue_ * beatCount_;
}