//
// Created by pgg on 10/08/17.
//

#include "NoteValue.h"

NoteValue::NoteValue(uint16_t value) :
    value_(value)
{

}

NoteValue NoteValue::dot()
{
    return NoteValue(value_ + value_ >> 1);
}

NoteValue const& NoteValue::operator+=(NoteValue const &b)
{
    value_ += b.value_;
    return value_;
}