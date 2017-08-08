//
// Created by pgg on 02/08/17.
//

#ifndef APISCORE_FREQUENCYANALYZER_H
#define APISCORE_FREQUENCYANALYZER_H


#include <cmath>
#include <vector>
#include <cassert>
#include "spectrum/SpectrumDefinition.h"
#include "spectrum/Spectrum.h"
#include "spectrum/SampledSpectrumDefinition.h"

class FrequencyAnalyzer {
public:
    FrequencyAnalyzer(int rate);

    template <typename V>
    void getSpectrumIntensity(V const& data, int size, SpectrumDefinition const& spec, Spectrum::Ptr & output)
    {
        assert(output != nullptr);

        float tau = 2 * M_PI / rate_;
        for(int i = 0; i < spec.size; i++) {
            float sin_sum = 0, cos_sum = 0;
            float thetaStep = spec.frequency[i] * tau;
            for(int j = 0; j < size; j++) {
                float theta = j * thetaStep;
                sin_sum += data[j] * sin(theta);
                cos_sum += data[j] * cos(theta);
            }
            (*output)[i] = hypot(cos_sum, sin_sum) / size;
        }
    }

    template <typename V>
    void getSpectrumIntensity(V const& data, int size, SampledSpectrumDefinition const& ss, Spectrum::Ptr & output)
    {
        assert(output != nullptr);

        for(int f = 0; f < ss.size(); f++) {
            double cos_sum = 0, sin_sum = 0;
            for(int s = 0; s < size; s++) {
                const auto& cos_sin(ss[f][s]);
                cos_sum += data[s] * cos_sin.first;
                sin_sum += data[s] * cos_sin.second;
            }

            (*output)[f] = hypot(cos_sum, sin_sum) / size;
        }
    }
private:
    int rate_;
};


#endif //APISCORE_FREQUENCYANALYZER_H
