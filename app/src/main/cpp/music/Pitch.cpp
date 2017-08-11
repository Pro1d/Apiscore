//
// Created by pgg on 06/08/17.
//

#include "Pitch.h"

const std::vector<std::string> Pitch::semitoneNames = SEMITONE_NAMES;
const std::regex Pitch::noteAndOctaveNameRegex(SEMITONE_NAME_REGEX OCTAVE_NAME_REGEX);
const std::regex Pitch::noteNameRegex(SEMITONE_NAME_REGEX);

Pitch::Pitch(int id) :
        noteId_(id)
{

}

Pitch::Pitch(int note, int octave) :
        noteId_(note + octave * SEMITONES_PER_SCALE)
{

}

Pitch::Pitch(std::string const &nameWithOctave) :
    noteId_(getIdFromNoteNameWithOctave(nameWithOctave))
{

}

int Pitch::getId()
{
    return noteId_;
}

Pitch& Pitch::operator++()
{
    ++noteId_;
    return *this;
}

Pitch& Pitch::operator--()
{
    --noteId_;
    return *this;
}

Pitch const& Pitch::operator+=(int i)
{
    noteId_ += i;
    return *this;
}

Pitch const& Pitch::operator+(int i)
{
    return Pitch(noteId_ + i);
}

int Pitch::getNote()
{
    int semitones = noteId_ % SEMITONES_PER_SCALE;

    return semitones < 0 ? semitones + SEMITONES_PER_SCALE : semitones;
}

std::string Pitch::getNoteName()
{
    return Pitch::semitoneNames[getNote()];
}

int Pitch::getOctave()
{
    if(noteId_ < 0)
        return ((noteId_+1)/SEMITONES_PER_SCALE - 1);
    return noteId_ / SEMITONES_PER_SCALE;
}

std::string Pitch::getOctaveName()
{
    std::stringstream ss;
    ss << getOctave();
    return ss.str();
}

std::string Pitch::getNoteNameWithOctave()
{
    return getNoteName()+getOctaveName();
}

double Pitch::getFrequency()
{
    return A4_FREQUENCY * pow(2.0, (double) (noteId_-A4_ID) / SEMITONES_PER_SCALE);
}

int Pitch::getIdFromNoteNameWithOctave(std::string const &name)
{
    if(Pitch::isValidNoteNameWithOctave(name)) {
        std::size_t nLength = (name[1] == '#' ? 2 : 1);
        auto it = std::find(Pitch::semitoneNames.begin(), Pitch::semitoneNames.end(),
                            name.substr(0, nLength));

        int note = std::distance(Pitch::semitoneNames.begin(), it);

        int octave = 0;
        std::stringstream ss(name.substr(nLength));
        ss >> octave;

        return Pitch(note, octave).noteId_;
    }
    return 0; // error
}

bool Pitch::isValidNoteNameWithOctave(std::string const &name)
{
    return std::regex_match(name, Pitch::noteAndOctaveNameRegex);
}

bool Pitch::isValidNoteName(std::string const& name)
{
    return std::regex_match(name, Pitch::noteNameRegex);
}