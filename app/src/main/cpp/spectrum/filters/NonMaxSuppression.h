//
// Created by pgg on 04/08/17.
//

#ifndef APISCORE_NONMAXSUPRESSION_H
#define APISCORE_NONMAXSUPRESSION_H


#include "../SpectrumFilter.h"

class NonMaxSuppression : public SpectrumFilter {
public:
    void filter(Spectrum & out);
};


#endif //APISCORE_NONMAXSUPRESSION_H
