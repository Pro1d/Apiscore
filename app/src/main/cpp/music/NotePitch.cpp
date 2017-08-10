//
// Created by pgg on 06/08/17.
//

#include "NotePitch.h"

const std::vector<std::string> NotePitch::semitoneNames = SEMITONE_NAMES;
const std::regex NotePitch::noteAndOctaveNameRegex(SEMITONE_NAME_REGEX OCTAVE_NAME_REGEX);
const std::regex NotePitch::noteNameRegex(SEMITONE_NAME_REGEX);

NotePitch::NotePitch(int id) :
        noteId_(id)
{

}

NotePitch::NotePitch(int note, int octave) :
        noteId_(note + octave * SEMITONES_PER_SCALE)
{

}

NotePitch::NotePitch(std::string const &nameWithOctave) :
    noteId_(getIdFromNoteNameWithOctave(nameWithOctave))
{

}

int NotePitch::getId()
{
    return noteId_;
}

NotePitch& NotePitch::operator++()
{
    ++noteId_;
    return *this;
}

NotePitch& NotePitch::operator--()
{
    --noteId_;
    return *this;
}

int NotePitch::getNote()
{
    int semitones = noteId_ % SEMITONES_PER_SCALE;

    return semitones < 0 ? semitones + SEMITONES_PER_SCALE : semitones;
}

std::string NotePitch::getNoteName()
{
    return NotePitch::semitoneNames[getNote()];
}

int NotePitch::getOctave()
{
    if(noteId_ < 0)
        return ((noteId_+1)/SEMITONES_PER_SCALE - 1);
    return noteId_ / SEMITONES_PER_SCALE;
}

std::string NotePitch::getOctaveName()
{
    std::stringstream ss;
    ss << getOctave();
    return ss.str();
}

std::string NotePitch::getNoteNameWithOctave()
{
    return getNoteName()+getOctaveName();
}

double NotePitch::getFrequency()
{
    return A4_FREQUENCY * pow(2.0, (double) (noteId_-A4_ID) / SEMITONES_PER_SCALE);
}

int NotePitch::getIdFromNoteNameWithOctave(std::string const &name)
{
    if(NotePitch::isValidNoteNameWithOctave(name)) {
        std::size_t nLength = (name[1] == '#' ? 2 : 1);
        auto it = std::find(NotePitch::semitoneNames.begin(), NotePitch::semitoneNames.end(),
                            name.substr(0, nLength));

        int note = std::distance(NotePitch::semitoneNames.begin(), it);

        int octave = 0;
        std::stringstream ss(name.substr(nLength));
        ss >> octave;

        return NotePitch(note, octave).noteId_;
    }
    return 0; // error
}

bool NotePitch::isValidNoteNameWithOctave(std::string const &name)
{
    return std::regex_match(name, NotePitch::noteAndOctaveNameRegex);
}

bool NotePitch::isValidNoteName(std::string const& name)
{
    return std::regex_match(name, NotePitch::noteNameRegex);
}