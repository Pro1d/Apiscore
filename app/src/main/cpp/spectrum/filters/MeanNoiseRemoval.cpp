//
// Created by pgg on 04/08/17.
//

#include <cmath>
#include "MeanNoiseRemoval.h"

MeanNoiseRemoval::MeanNoiseRemoval(double scale) :
    scale_(scale)
{

}

void MeanNoiseRemoval::filter(Spectrum &out)
{
    Spectrum const& input(*input_);

    out.resize(input.size());

    auto meanAndVar = input.getMeanAndVariance();
    double mean = meanAndVar.first;
    //double stddev = sqrt(meanAndVar.second);
    double offset = mean * (1 + scale_);

    for(int i = 0; i < input.size(); i++) {
        out[i] = std::max(0.0, input[i] - offset) / (1 - offset);
    }
}