//
// Created by pgg on 04/08/17.
//

#include <cmath>
#include <sstream>
#include "SpectrumDefinition.h"


SpectrumDefinition::SpectrumDefinition(int startFromA4, int endFromA4) :
        size(endFromA4 - startFromA4),
        frequency(size),
        name(size)
{
    std::vector<std::string> names = {"A", "A#", "B", "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#"};

    for(int i = startFromA4; i < endFromA4; i++) {
        frequency[i - startFromA4] = A4_FREQUENCY * pow(2.f, i / 12.f);
        std::stringstream ss;
        int v = i+4*12;
        if(v < 0)
            ss << names[(v % 12 + 12) % 12] << ((v+1)/12 - 1);
        else
            ss << names[v % 12] << (v / 12);
        name[i - startFromA4] = ss.str();
    }
}