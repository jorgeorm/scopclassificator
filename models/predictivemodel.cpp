#include "predictivemodel.h"

PredictiveModel::PredictiveModel(){
    _columnScaleValues = NULL;
    _representativeFeatures = NULL;
    _featureDefinition = NULL;
}

PredictiveModel::~PredictiveModel(){
    if (_columnScaleValues != NULL) delete _columnScaleValues;
    if (_representativeFeatures != NULL)  delete _representativeFeatures;
    if (_featureDefinition != NULL) delete _featureDefinition;
}

void PredictiveModel::setProfiles(const QMap<QString, float *> &profiles){
    _profiles = profiles;
}

Matrix<float> *PredictiveModel::representativeFeatures() const{
    return _representativeFeatures;
}

void PredictiveModel::setRepresentativeFeatures(Matrix<float > *representativeFeatures){
    _representativeFeatures = representativeFeatures;
}

void PredictiveModel::addProfile(QString classTag, float *profile){
    _profiles.insert(classTag, profile);
}

FeatureDefinition *PredictiveModel::featureDefinition() const{
    return _featureDefinition;
}

void PredictiveModel::setFeatureDefinition(FeatureDefinition *featureDefinition){
    _featureDefinition = featureDefinition;
}

unsigned PredictiveModel::profileLength() const{
    return _representativeFeatures->rows();
}

void PredictiveModel::setProfileScaleFactors(float *scalesByFeat = NULL){
    if (scalesByFeat != NULL) _columnScaleValues = scalesByFeat;
}

float *PredictiveModel::scaleValuesByFeat() const{
    return _columnScaleValues;
}

bool PredictiveModel::hasScaledProfile() const{
    return _columnScaleValues != NULL ? true : false;
}

QMap<QString, float *> PredictiveModel::profiles() const{
    return _profiles;
}

float *PredictiveModel::profile(QString classTag) const{
    if(_profiles.contains(classTag)){
        return _profiles.value(classTag);
    } else {
        return NULL;
    }
}
