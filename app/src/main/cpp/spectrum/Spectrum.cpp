//
// Created by pgg on 04/08/17.
//

#include <numeric>
#include <algorithm>
#include <android/log.h>
#include "Spectrum.h"

Spectrum::Spectrum()
{

}
Spectrum::Spectrum(int size) :
        std::vector<float>(size, 0)
{

}

float Spectrum::getMean() const
{
    return empty() ? 0.0f : std::accumulate(begin(), end(), 0.0f) / size();
}

std::pair<float, float> Spectrum::getMeanAndVariance() const
{
    float mean = getMean();
    float sum = 0.0;
    for(float const& v : *this)
        sum += (v - mean) * (v - mean);

    float var = empty() ? 0.0 : sum / size();

    return {mean, var};
}

void Spectrum::printStat()
{
    auto biggest = std::max_element(begin(), end());
    int ibiggest = std::distance(begin(), biggest);
    auto mv = getMeanAndVariance();
    __android_log_print(ANDROID_LOG_VERBOSE, "Spectrum", "size:%d mean:%f stddev:%f max:%f imax:%d", (int) size(), mv.first, sqrt(mv.second), *biggest, ibiggest);
}