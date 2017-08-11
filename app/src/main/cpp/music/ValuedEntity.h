//
// Created by pgg on 11/08/17.
//

#ifndef APISCORE_VALUEDENTITY_H
#define APISCORE_VALUEDENTITY_H


#include <memory>
#include "Value.h"

class ValuedEntity {
public:
    typedef std::shared_ptr<ValuedEntity> Ptr;

    ValuedEntity(Value const& v) : value_(v) {}

    Value const& getValue() const { return value_; }
    void setValue(Value const& v) { value_ = v; }

protected:
    Value value_;
};


#endif //APISCORE_VALUEDENTITY_H
