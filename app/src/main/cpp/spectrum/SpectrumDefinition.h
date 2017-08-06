//
// Created by pgg on 04/08/17.
//

#ifndef APISCORE_SPECTRUM_H
#define APISCORE_SPECTRUM_H

#include <vector>
#include <string>


struct SpectrumDefinition {
    SpectrumDefinition(int start, unsigned int length);

    const unsigned int size;
    std::vector<float> frequency;
    std::vector<std::string> name;

};


#endif //APISCORE_SPECTRUM_H
