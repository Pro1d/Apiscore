//
// Created by pgg on 04/08/17.
//

#include <cmath>
#include <sstream>
#include "SpectrumDefinition.h"
#include "../Music/NotePitch.h"


SpectrumDefinition::SpectrumDefinition(int start, unsigned int length) :
        size(length),
        frequency(size),
        name(size)
{
    NotePitch note(start);

    for(int i = 0; i < length; i++, ++note) {
        frequency[i] = note.getFrequency();
        name[i] = note.getNoteNameWithOctave();
    }
}