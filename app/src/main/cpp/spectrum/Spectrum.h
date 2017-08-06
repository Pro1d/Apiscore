//
// Created by pgg on 04/08/17.
//

#ifndef APISCORE_SPECTRUMINTENSITY_H
#define APISCORE_SPECTRUMINTENSITY_H

#include <vector>
#include <memory>
#include "SpectrumDefinition.h"

class Spectrum : public std::vector<float> {
public:
    typedef std::shared_ptr<Spectrum> Ptr;

    Spectrum();
    Spectrum(int size);

    float getMean() const;
    std::pair<float, float> getMeanAndVariance() const;

    void printStat();

private:
};


#endif //APISCORE_SPECTRUMINTENSITY_H
