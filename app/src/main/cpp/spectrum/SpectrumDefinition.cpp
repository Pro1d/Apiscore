//
// Created by pgg on 04/08/17.
//

#include <cmath>
#include <sstream>
#include "SpectrumDefinition.h"
#include "../music/Pitch.h"


SpectrumDefinition::SpectrumDefinition(int start, unsigned int length) :
        size(length),
        firstNote(start),
        frequency(size),
        name(size)
{
    Pitch note(firstNote);

    for(int i = 0; i < length; i++, ++note) {
        frequency[i] = note.getFrequency();
        name[i] = note.getNoteNameWithOctave();
    }
}