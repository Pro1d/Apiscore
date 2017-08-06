//
// Created by pgg on 02/08/17.
//
#include <cmath>
#include <sstream>
#include <cassert>
#include "FrequencyAnalyzer.h"
#include "spectrum/SampledSpectrumDefinition.h"


FrequencyAnalyzer::FrequencyAnalyzer(int rate) :
    rate_(rate)
{

}

void FrequencyAnalyzer::getSpectrumIntensity(std::vector<float> const& data, int size, SpectrumDefinition const& spec, Spectrum::Ptr & output)
{
    assert(output != nullptr);

    float tau = 2 * M_PI / rate_;
    for(int i = 0; i < spec.size; i++) {
        float ssum = 0, csum = 0;
        float thetaStep = spec.frequency[i] * tau;
        for(int j = 0; j < size; j++) {
            float theta = j * thetaStep;
            ssum += data[j] * sin(theta);
            csum += data[j] * cos(theta);
        }
        (*output)[i] = sqrt(ssum*ssum+csum*csum) / size;
    }
}

void FrequencyAnalyzer::getSpectrumIntensity(std::vector<float> const& data, int size, SampledSpectrumDefinition const& ss, Spectrum::Ptr & output)
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
