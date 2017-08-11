//
// Created by pgg on 10/08/17.
//

#ifndef APISCORE_MUSICNOTE_H
#define APISCORE_MUSICNOTE_H


#include "Pitch.h"
#include "Value.h"
#include "ValuedEntity.h"

class Note : public ValuedEntity {
public:
    Note(Pitch const& pitch, Value const& value): ValuedEntity(value), pitch_(pitch) {}
    Pitch const& getPitch() const { return pitch_; }
    void setPitch(Pitch const& pitch) { pitch_ = pitch; }
private:
    Pitch pitch_;
};


#endif //APISCORE_MUSICNOTE_H
