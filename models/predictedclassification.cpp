#include "predictedclassification.h"

PredictedClassification::PredictedClassification(){
    _entry = NULL;
}

PredictedClassification::~PredictedClassification(){
    if (_entry != NULL)
        delete _entry;
}

SCOPEntry *PredictedClassification::entry() const{
    return _entry;
}

void PredictedClassification::setEntry(SCOPEntry *entry){
    _entry = entry;
}

QString PredictedClassification::classification() const{
    return _classification;
}

void PredictedClassification::setClassification(const QString &classification){
    if (classification == _entry->scss())
        _assertion = true;
    _classification = classification;

//    qDebug() << _entry->sid() <<", " <<
//                _classification << ", " <<
//                _entry->scss();
}

