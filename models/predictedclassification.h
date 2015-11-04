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

    QString classification() const;
    void setClassification(const QString &classification);

private:
    SCOPEntry *_entry;
    QString _classification;
    bool _assertion;
};

#endif // PREDICTEDCLASSIFICATION_H
