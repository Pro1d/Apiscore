//
// Created by pgg on 10/08/17.
//

#include "ScoreRecorder.h"

void ScoreRecorder::startRecording(double timestamp, SpectrumDefinition::Ptr const& spectrumDefinition, Score::Ptr const &score)
{
    startTimestamp_ = timestamp;
    spectrumDefinition_ = spectrumDefinition;
    musicScore_ = score;
    currentNote_ = -1;
    startTimestampCurrentNote_ = timestamp;
}

void ScoreRecorder::record(Spectrum const &spectrum)
{
    double t = spectrum.getAcquisitionStart();
    int note = selectNote(spectrum);

    if(note != currentNote_) {
        UnsignedFixedPoint start = timeToValue(startTimestampCurrentNote_, Value(16));
        UnsignedFixedPoint end = timeToValue(t, Value(16));

        ValuedEntity::Ptr e;
        if(note >= 0)
            e = std::make_shared<Note>(Pitch(spectrumDefinition_->firstNote + note), end - start);
        else
            e = std::make_shared(end - start);

        musicScore_->addValued(e);

        currentNote_ = note;
        startTimestampCurrentNote_ = t;
    }
}

UnsignedFixedPoint const& ScoreRecorder::timeToValue(double t, Value const& precision)
{
    double beatPeriod = (60 * 1000.0) / musicScore_->getTempoBPM(); // FIXME class Tempo, method getBeatPeriod()
    double beat = (t - startTimestamp_) / beatPeriod;
    return UnsignedFixedPoint(beat).round(precision.getUFP());
}

int ScoreRecorder::selectNote(Spectrum const &spectrum)
{
    auto m = std::max_element(spectrum.begin(), spectrum.end());
    if(*m > 0.01)
        return std::distance(spectrum.begin(), m);
    return -1;
}