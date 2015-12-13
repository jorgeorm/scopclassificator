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

    /**
     * @brief profile Gets an specific profile using a key value
     * @param key Identifies a profile in the model
     * @return
     */
    float * profile(QString key) const;

    Matrix<float> *representativeFeatures() const;

    void setRepresentativeFeatures(Matrix<float> *representativeFeatures);
    /**
     * @brief addProfile Adds a new profile storing the scop_classification and the profile
     * @param key SCOP classification
     * @param profile profile built after finding representative models (features)
     */
    void addProfile(QString key, float* profile);
    /**
     * @brief addProfile Adds a new profile storing the scop_id, scop_classification and the profile.
     * @param sid SCOP id
     * @param scss SCOP classification
     * @param profile profile built after finding representative models (features)
     */
    void addProfile(QString sid, QString scss, float* profile);

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
    QMap<QString, QString> _classes;
    float * _columnScaleValues;
    Matrix<float> *_representativeFeatures;
    FeatureDefinition *_featureDefinition;
};


#endif // PREDICTIVEMODEL_H
