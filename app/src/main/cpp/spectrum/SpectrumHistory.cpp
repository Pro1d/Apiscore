//
// Created by pgg on 07/08/17.
//

#include "SpectrumHistory.h"

SpectrumHistory::SpectrumHistory(unsigned int maxSize) :
        maxSize_(maxSize),
        totalHistorySize_(0)
{

}

void SpectrumHistory::addSpectrum(Spectrum const &spectrum)
{
    if(spectrum.size() == maxSize_)
        history_.pop_back();

    history_.push_front(std::make_pair(spectrum, totalHistorySize_));
    totalHistorySize_++;
}