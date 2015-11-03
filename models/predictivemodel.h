#ifndef PREDICTIVEMODEL_H
#define PREDICTIVEMODEL_H

#include "featuredefinition.h"

#include <QMap>
#include <QObject>

#include <models/utils/matrix.h>

class PredictiveModel{
public:
    PredictiveModel();
    virtual ~PredictiveModel();

    void setProfiles(const QMap<QString, float *> &profiles);
    QMap<QString, float *> profiles() const;
    float * profile(QString classTag) const;


    Matrix<float> *representativeFeatures() const;
    void setRepresentativeFeatures(Matrix<float> *representativeFeatures);
    void addProfile(QString classTag, float* profile);

    FeatureDefinition *featureDefinition() const;
    void setFeatureDefinition(FeatureDefinition *featureDefinition);
    unsigned profileLength() const;
    void setProfileScaleFactors(float *scalesByFeat);
    float *scaleValuesByFeat() const;
    /**
     * @brief hasScaledProfile Determines if profile has been scaled
     * @return
     */
    bool hasScaledProfile() const;


private:
    QMap<QString, float *> _profiles;
    float * _columnScaleValues;
    Matrix<float> *_representativeFeatures;
    FeatureDefinition *_featureDefinition;
};


#endif // PREDICTIVEMODEL_H
