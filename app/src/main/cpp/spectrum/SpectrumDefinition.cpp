//
// Created by pgg on 04/08/17.
//

#include <cmath>
#include <sstream>
#include "SpectrumDefinition.h"
#include "../Music.h"


SpectrumDefinition::SpectrumDefinition(int start, unsigned int length) :
        size(length),
        frequency(size),
        name(size)
{
    std::vector<std::string> names = SEMITONE_NAMES;

    for(int i = 0; i < length; i++) {
        int note_id = i + start;
        frequency[i] = A4_FREQUENCY * pow(2.f, (float) (note_id-A4_ID) / SEMITONES_PER_SCALE);

        std::stringstream ss;
        ss << names[semitonesFromNoteId(note_id)];
        ss << octaveFromINoted(note_id+1);

        name[i] = ss.str();
    }
}