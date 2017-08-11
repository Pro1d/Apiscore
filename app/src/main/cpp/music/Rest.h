//
// Created by pgg on 11/08/17.
//

#ifndef APISCORE_MUSICREST_H
#define APISCORE_MUSICREST_H


#include "ValuedEntity.h"

class Rest : public ValuedEntity {
public:
    Rest(Value const& v) : ValuedEntity(v) {}
};


#endif //APISCORE_MUSICREST_H
