//
// Created by pgg on 06/08/17.
//

#ifndef APISCORE_MUSIC_H
#define APISCORE_MUSIC_H

#define A4_FREQUENCY            440 // Hz
#define SEMITONES_PER_SCALE     12
#define NOTES_PER_SCALE         7
#define A4_ID                   (4*SEMITONES_PER_SCALE)
#define SEMITONE_NAMES          {"A", "A#", "B", "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#"}

template<typename T>
T semitonesFromNoteId(T note_id)
{
    T semitones = note_id % SEMITONES_PER_SCALE;

    return semitones < 0 ? (semitones + SEMITONES_PER_SCALE) % SEMITONES_PER_SCALE : semitones;
}

template<typename T>
T octaveFromINoted(T note_id)
{
    if(note_id < 0)
        return ((note_id+1)/SEMITONES_PER_SCALE - 1);
    return note_id / SEMITONES_PER_SCALE;

}

#endif //APISCORE_MUSIC_H
