//
// Created by pgg on 04/08/17.
//

#include <cmath>
#include <sstream>
#include "SpectrumDefinition.h"
#include "../MusicNote.h"


SpectrumDefinition::SpectrumDefinition(int start, unsigned int length) :
        size(length),
        frequency(size),
        name(size)
{
    MusicNote note(start);

    for(int i = 0; i < length; i++) {
        frequency[i] = note.getFrequency();
        name[i] = note.getNoteNameWithOctave();
    }
}