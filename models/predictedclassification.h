#ifndef PREDICTEDCLASSIFICATION_H
#define PREDICTEDCLASSIFICATION_H

#include "scopentry.h"



class PredictedClassification
{
public:
    PredictedClassification();
    virtual ~PredictedClassification();

    SCOPEntry *entry() const;
    void setEntry(SCOPEntry *entry);

    QString nearestNeighbor() const;
    void setNearestNeighbor(const QString &classification);

    QString nearestNeighborScss() const;
    void setNearestNeighborScss(const QString &nearestNeighborScss);

private:
    SCOPEntry *_entry;
    QString _nearestNeighbor;
    QString _nearestNeighborScss;
};

#endif // PREDICTEDCLASSIFICATION_H
