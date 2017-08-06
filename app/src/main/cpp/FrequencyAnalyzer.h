//
// Created by pgg on 02/08/17.
//

#ifndef APISCORE_FREQUENCYANALYZER_H
#define APISCORE_FREQUENCYANALYZER_H


#include <asm/sigcontext.h>
#include <jni.h>
#include <string>
#include <vector>
#include "spectrum/SpectrumDefinition.h"
#include "spectrum/Spectrum.h"
#include "spectrum/SampledSpectrumDefinition.h"

class FrequencyAnalyzer {
public:
    FrequencyAnalyzer(int rate);
    void getSpectrumIntensity(std::vector<float> const& data, int size, SpectrumDefinition const& spec, Spectrum::Ptr & output);
    void getSpectrumIntensity(std::vector<float> const& data, int size, SampledSpectrumDefinition const& ss, Spectrum::Ptr & output);
private:
    int rate_;
};


#endif //APISCORE_FREQUENCYANALYZER_H
