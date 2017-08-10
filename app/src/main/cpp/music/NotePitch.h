//
// Created by pgg on 06/08/17.
//

#ifndef APISCORE_MUSIC_H
#define APISCORE_MUSIC_H

#include <regex>
#include <vector>
#include <string>

/*
 * Refenrece note:
 *      Id = 0
 *      scale = 0
 *      note = C
 *      full name = "C0"
 */

#define A4_FREQUENCY            440 // Hz
#define A4_ID                   (4*SEMITONES_PER_SCALE + 9)
#define SEMITONES_PER_SCALE     12
#define NOTES_PER_SCALE         7
#define SEMITONE_NAMES          {"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"}
#define REFERENCE_NAME          "C0"
#define SEMITONE_NAME_REGEX     "[A-G]#?"
#define OCTAVE_NAME_REGEX       "-?[1-9]*[0-9]"
// TODO SEMITONE_NAME_REGEX     "[A-G][#b]*"

class NotePitch {
public:
    enum SemitoneId : int {C, Cs, Db=Cs, D, Ds, Eb=Ds, E, F, Fs, Gb=Fs, G, Gs, Ab=Gs, A, As, Bb=As, B};

    NotePitch(int id);
    NotePitch(int note, int octave);
    NotePitch(std::string const& nameWithOctave);

    int getId();
    NotePitch& operator++();
    NotePitch& operator--();

    int getNote();
    std::string getNoteName();

    int getOctave();
    std::string getOctaveName();

    std::string getNoteNameWithOctave();

    double getFrequency();

    static bool isValidNoteNameWithOctave(std::string const &name);
    static bool isValidNoteName(std::string const& name);

private:
    int noteId_;

    static const std::vector<std::string> semitoneNames;
    static const std::regex noteAndOctaveNameRegex;
    static const std::regex noteNameRegex;

    /** @return note id or 0 if name is invalid */
    static int getIdFromNoteNameWithOctave(std::string const &name);
};



#endif //APISCORE_MUSIC_H
