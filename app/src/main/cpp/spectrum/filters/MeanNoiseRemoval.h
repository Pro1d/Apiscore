//
// Created by pgg on 04/08/17.
//

#ifndef APISCORE_MEANNOISEREMOVAL_H
#define APISCORE_MEANNOISEREMOVAL_H


#include "../Spectrum.h"
#include "../SpectrumFilter.h"

class MeanNoiseRemoval : public SpectrumFilter {
public:
    MeanNoiseRemoval(double scale = 3.0);
    void setScale(double scale) { scale_ = scale; }
    void filter(Spectrum& out);
private:
    double scale_;
};


#endif //APISCORE_MEANNOISEREMOVAL_H
