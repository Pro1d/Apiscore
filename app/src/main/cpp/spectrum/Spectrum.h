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

    void setAcquisitionStart(double t) { acquisitionStart_ = t; }
    void setAcquisitionEnd(double t) { acquisitionEnd_ = t; }
    double getAcquisitionDuration() { return acquisitionEnd_ - acquisitionStart_; }

    float getMean() const;
    std::pair<float, float> getMeanAndVariance() const;

    void printStat();

private:
    double acquisitionStart_;
    double acquisitionEnd_;
};


#endif //APISCORE_SPECTRUMINTENSITY_H
