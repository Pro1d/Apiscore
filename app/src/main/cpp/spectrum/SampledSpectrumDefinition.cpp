//
// Created by pgg on 04/08/17.
//

#include <cmath>
#include "SampledSpectrumDefinition.h"

using std::vector;
using std::pair;

SampledSpectrumDefinition::SampledSpectrumDefinition(SpectrumDefinition const& spec, int samplesCount, int samplingRate) :
    vector<vector<pair<double, double>>>(spec.size, vector<pair<double, double>>(samplesCount))
{
    double theta_E = 2 * M_PI / samplingRate;

    for(int f = 0; f < spec.size; f++) {
        double thetaStep = spec.frequency[f] * theta_E;

        for(int s = 0; s < samplesCount; s++) {
            double theta = s * thetaStep;
            (*this)[f][s].first = cos(theta);
            (*this)[f][s].second = sin(theta);
        }
    }
}