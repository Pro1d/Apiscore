//
// Created by pgg on 07/08/17.
//

#ifndef APISCORE_SPECTRUMHISTORY_H
#define APISCORE_SPECTRUMHISTORY_H


#include <deque>
#include <memory>
#include "Spectrum.h"

class SpectrumHistory {
public:
    typedef std::shared_ptr<SpectrumHistory> Ptr;

    SpectrumHistory(unsigned int maxSize);
    void addSpectrum(Spectrum const& spectrum);
private:
    std::deque<std::pair<Spectrum, unsigned long int>> history_;
    unsigned int maxSize_;
    unsigned long int totalHistorySize_;
};


#endif //APISCORE_SPECTRUMHISTORY_H
