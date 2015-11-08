#ifndef PREDICTIVEMODELSERVICE_H
#define PREDICTIVEMODELSERVICE_H

#include <QObject>
#include <models/featuredefinition.h>
#include <models/predictivemodel.h>
#include <models/scopentry.h>

class PredictiveModelService : public QObject{
    Q_OBJECT

public:
    enum DISTANCETYPE {EUCLIDEAN, COSINE};

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
                     float *profile = NULL);

    float *getProfile(Matrix<float> *representativeFeatures = NULL,
                         SCOPEntry *entry = NULL,
                         FeatureDefinition *featDef = NULL);
    float *getProfile(Matrix<float> *representativeFeatures = NULL,
                         Matrix<float> *localFeatures = NULL);

    void scaleProfiles(PredictiveModel *model = NULL, float *scaleValues = NULL);

    void saveModel(PredictiveModel *model = NULL, QString file = "");

    DISTANCETYPE getDistance() const;
    void setDistance(const DISTANCETYPE &distance);

    SCOPEntry *getEntry() const;
    void setEntry(SCOPEntry *entry);

    PredictiveModel *getModel() const;
    void setModel(PredictiveModel *model);

    QString classTag() const;

    PredictiveModel *loadModel(QString pathModel);

signals:
    void entryClassified(QString entryName);

private slots:
    void runClassification();

protected:
    void loadFeatureDefinition(QTextStream *istream, PredictiveModel *model);

    void loadRepresentativeFeatures(QTextStream *istream, PredictiveModel *model);

    void loadProfiles(QTextStream *istream, PredictiveModel *model);

    float euclideanDistance(float *profile1,
                    float *profile2,
                    unsigned profileSize) const;

    float cosineSimilarity(float *profile1,
                           float *profile2,
                           unsigned profileSize) const;
    void scaleProfile(float *profile, float *scaleValues, unsigned profileLength);

    DISTANCETYPE _distance;
    SCOPEntry *_entry;
    PredictiveModel *_model;
    QString _classTag;

};

#endif // PREDICTIVEMODELSERVICE_H
