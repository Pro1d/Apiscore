//
// Created by pgg on 10/08/17.
//

#include "Score.h"

void Score::addValued(ValuedEntity::Ptr const &e)
{
    notesAndRests_.push_back(e);
}