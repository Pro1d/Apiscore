//
// Created by pgg on 10/08/17.
//

#ifndef APISCORE_NOTEVALUE_H
#define APISCORE_NOTEVALUE_H

#include <cstdint>

#define FIXED_POINT_NOTE_VALUE     12

enum NamedNoteValues : uint16_t {
    SixteenthNote   = 1 << (-4 + FIXED_POINT_NOTE_VALUE),
    EighthNote      = 1 << (-3 + FIXED_POINT_NOTE_VALUE),
    QuarterNote     = 1 << (-2 + FIXED_POINT_NOTE_VALUE),
    HalfNote        = 1 << (-1 + FIXED_POINT_NOTE_VALUE),
    WholeNote       = 1 << (+0 + FIXED_POINT_NOTE_VALUE)
};

class NoteValue {
public:
    NoteValue(uint16_t value);
    NoteValue dot();
    NoteValue const& operator+=(NoteValue const& b);
private:
    uint16_t value_;
};


#endif //APISCORE_NOTEVALUE_H
