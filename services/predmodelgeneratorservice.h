#ifndef PREDMODELGENERATORSERVICE_H
#define PREDMODELGENERATORSERVICE_H

#include <models/predictivemodel.h>
#include <models/scopentry.h>



class PredModelGeneratorService
{
public:
    PredModelGeneratorService();

    PredictiveModel *generateModel(QList<SCOPEntry*> entries, Matrix<float> *repFeatures);
    QString classify(PredictiveModel *model, int *profile);
    int *getProfile(Matrix<float> *repFeatures, SCOPEntry *entry);

};

#endif // PREDMODELGENERATORSERVICE_H
