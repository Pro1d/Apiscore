//
// Created by pgg on 04/08/17.
//

#ifndef APISCORE_SAMPLEDSPECTRUM_H
#define APISCORE_SAMPLEDSPECTRUM_H


#include "SpectrumDefinition.h"

class SampledSpectrumDefinition : public std::vector<std::vector<std::pair<double, double>>> {
public:
    SampledSpectrumDefinition(SpectrumDefinition const& spec, int samplesCount, int samplingRate);
};


#endif //APISCORE_SAMPLEDSPECTRUM_H
