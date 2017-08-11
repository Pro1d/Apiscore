//
// Created by pgg on 10/08/17.
//

#ifndef APISCORE_SCORERECORDER_H
#define APISCORE_SCORERECORDER_H


#include "music/Score.h"
#include "spectrum/Spectrum.h"

/**
 * From filtered dated spectrums to score with notes and rests
 */
class ScoreRecorder {
public:
    void startRecording(double timestamp, SpectrumDefinition::Ptr const& spectrumDefinition, Score::Ptr const& score);
    void record(Spectrum const &spectrum);
    Score::Ptr const& stopRecording();

    Score::Ptr const& getMusicScore() const { return musicScore_; }
    //void setMusicScore(Score::Ptr const& score) { musicScore_ = score; }
protected:
    int selectNote(Spectrum const& spectrum);
    UnsignedFixedPoint const& timeToValue(double t, Value const& precision);

private:

    double startTimestamp_;
    SpectrumDefinition::Ptr spectrumDefinition_;
    Score::Ptr musicScore_;
    int currentNote_;
    double startTimestampCurrentNote_;
};


#endif //APISCORE_SCORERECORDER_H
