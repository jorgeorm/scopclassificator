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

void PredictiveModel::addProfile(QString key, float *profile){
    _profiles.insertMulti(key, profile);
}

void PredictiveModel::addProfile(QString sid, QString scss, float *profile){
    addProfile(sid, profile);
    _classes.insert(sid, scss);
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

float *PredictiveModel::profile(QString key) const{
    if(_profiles.contains(key)){
        return _profiles.value(key);
    } else {
        return NULL;
    }
}
