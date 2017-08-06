//
// Created by pgg on 04/08/17.
//

#ifndef APISCORE_SPECTRUM_H
#define APISCORE_SPECTRUM_H

#include <vector>
#include <string>

#define A4_FREQUENCY    440 // Hz

struct SpectrumDefinition {
    SpectrumDefinition(int startFromA3, int endFromA3);

    const int size;
    std::vector<float> frequency;
    std::vector<std::string> name;

};


#endif //APISCORE_SPECTRUM_H
