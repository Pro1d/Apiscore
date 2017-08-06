//
// Created by pgg on 04/08/17.
//

#ifndef APISCORE_SPECTRUMINTENSITYFILTER_H
#define APISCORE_SPECTRUMINTENSITYFILTER_H


#include "Spectrum.h"

class SpectrumFilter {
public:
    void setInput(Spectrum::Ptr & input) { this->input_ = input; }
    virtual void filter(Spectrum & out) = 0;
protected:
    Spectrum::Ptr input_;
};


#endif //APISCORE_SPECTRUMINTENSITYFILTER_H
