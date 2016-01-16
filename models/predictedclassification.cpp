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

QString PredictedClassification::nearestNeighborScss() const
{
    return _nearestNeighborScss;
}

void PredictedClassification::setNearestNeighborScss(const QString &nearestNeighborScss)
{
    _nearestNeighborScss = nearestNeighborScss;
}

QString PredictedClassification::nearestNeighbor() const{
    return _nearestNeighbor;
}

void PredictedClassification::setNearestNeighbor(const QString &classification){
    _nearestNeighbor = classification;
}
