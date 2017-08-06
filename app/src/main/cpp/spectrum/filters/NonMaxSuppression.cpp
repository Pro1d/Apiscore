//
// Created by pgg on 04/08/17.
//

#include "NonMaxSuppression.h"

void NonMaxSuppression::filter(Spectrum &out)
{
    Spectrum const& input(*input_);

    out.resize(input.size());

    for(int i = 0; i < input.size(); i++) {
        bool gt_prev = i > 0 ? input[i] > input[i-1] : false;
        bool gt_next = i < input.size()-1 ? input[i] > input[i+1] : false;

        out[i] = gt_prev && gt_next ? input[i] : 0.0;
    }
}