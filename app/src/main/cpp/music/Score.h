//
// Created by pgg on 10/08/17.
//

#ifndef APISCORE_MUSICSCORE_H
#define APISCORE_MUSICSCORE_H


#include "Pitch.h"
#include "Note.h"
#include "Tempo.h"
#include "TimeSignature.h"
#include <memory>
#include <vector>

class Score {
public:
    typedef std::shared_ptr<Score> Ptr;

    void addValued(ValuedEntity::Ptr const& e);
    TempoBMP getTempoBPM() { return bpm_; }
private:
    std::vector<ValuedEntity::Ptr> notesAndRests_;
    TempoBMP bpm_;
    //TimeSignature timeSignature_;
};


#endif //APISCORE_MUSICSCORE_H
