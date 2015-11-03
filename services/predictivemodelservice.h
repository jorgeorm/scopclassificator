#ifndef PREDICTIVEMODELSERVICE_H
#define PREDICTIVEMODELSERVICE_H

#include <models/featuredefinition.h>
#include <models/predictivemodel.h>
#include <models/scopentry.h>



class PredictiveModelService
{
public:
    PredictiveModelService();
    virtual ~PredictiveModelService();

    PredictiveModel *generateModel(QList<SCOPEntry*> entries,
                                   Matrix<float> *repFeatures,
                                   FeatureDefinition *featDef);

    QString classify(PredictiveModel *model = NULL,
                     SCOPEntry *entry = NULL);
    QString classify(PredictiveModel *model = NULL,
                     Matrix<float> *localFeatures = NULL);
    QString classify(PredictiveModel *model = NULL,
                     unsigned *profile = NULL);

    float *getProfile(Matrix<float> *representativeFeatures = NULL,
                         SCOPEntry *entry = NULL,
                         FeatureDefinition *featDef = NULL);
    float *getProfile(Matrix<float> *representativeFeatures = NULL,
                         Matrix<float> *localFeatures = NULL);

    void scaleProfiles(PredictiveModel *model = NULL, float *scaleValues = NULL);

    void saveModel(PredictiveModel *model = NULL, QString file = "");

protected:
    float cosineSimilarity(float *profile1,
                           float *profile2,
                           unsigned profileSize);

};

#endif // PREDICTIVEMODELSERVICE_H
