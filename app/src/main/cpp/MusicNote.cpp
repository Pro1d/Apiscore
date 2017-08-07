//
// Created by pgg on 06/08/17.
//

#include "MusicNote.h"

const std::vector<std::string> MusicNote::semitoneNames = SEMITONE_NAMES;
const std::regex MusicNote::noteAndOctaveNameRegex(SEMITONE_NAME_REGEX OCTAVE_NAME_REGEX);
const std::regex MusicNote::noteNameRegex(SEMITONE_NAME_REGEX);

MusicNote::MusicNote(int id) :
        noteId_(id)
{

}

MusicNote::MusicNote(int note, int octave) :
        noteId_(note + octave * SEMITONES_PER_SCALE)
{

}

MusicNote::MusicNote(std::string const &nameWithOctave) :
    noteId_(getIdFromNoteNameWithOctave(nameWithOctave))
{

}

int MusicNote::getId()
{
    return noteId_;
}

MusicNote& MusicNote::operator++()
{
    ++noteId_;
    return *this;
}

MusicNote& MusicNote::operator--()
{
    --noteId_;
    return *this;
}

int MusicNote::getNote()
{
    int semitones = noteId_ % SEMITONES_PER_SCALE;

    return semitones < 0 ? semitones + SEMITONES_PER_SCALE : semitones;
}

std::string MusicNote::getNoteName()
{
    return MusicNote::semitoneNames[getNote()];
}

int MusicNote::getOctave()
{
    if(noteId_ < 0)
        return ((noteId_+1)/SEMITONES_PER_SCALE - 1);
    return noteId_ / SEMITONES_PER_SCALE;
}

std::string MusicNote::getOctaveName()
{
    std::stringstream ss;
    ss << getOctave();
    return ss.str();
}

std::string MusicNote::getNoteNameWithOctave()
{
    return getNoteName()+getOctaveName();
}

double MusicNote::getFrequency()
{
    return A4_FREQUENCY * pow(2.0, (double) (noteId_-A4_ID) / SEMITONES_PER_SCALE);
}

int MusicNote::getIdFromNoteNameWithOctave(std::string const &name)
{
    if(MusicNote::isValidNoteNameWithOctave(name)) {
        std::size_t nLength = (name[1] == '#' ? 2 : 1);
        auto it = std::find(MusicNote::semitoneNames.begin(), MusicNote::semitoneNames.end(),
                            name.substr(0, nLength));

        int note = std::distance(MusicNote::semitoneNames.begin(), it);

        int octave = 0;
        std::stringstream ss(name.substr(nLength));
        ss >> octave;

        return MusicNote(note, octave).noteId_;
    }
    return 0; // error
}

bool MusicNote::isValidNoteNameWithOctave(std::string const &name)
{
    return std::regex_match(name, MusicNote::noteAndOctaveNameRegex);
}

bool MusicNote::isValidNoteName(std::string const& name)
{
    return std::regex_match(name, MusicNote::noteNameRegex);
}