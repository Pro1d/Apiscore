//
// Created by pgg on 10/08/17.
//

#include "Value.h"

Value::Value(UnsignedFixedPoint v) :
    value_(v)
{

}

Value::Value(unsigned int N) :
    value_(1.0)
{
    value_ = value_ / N;
}

Value Value::dot()
{
    return Value(value_ * UnsignedFixedPoint(1.5));
    // return Value(value_ + value_ >> 1);
}

Value const& Value::operator*(unsigned int v)
{
    return Value(value_ * v);
}