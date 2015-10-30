#include "predictivemodel.h"

PredictiveModel::PredictiveModel()
{

}

void PredictiveModel::setProfiles(const QMap<QString, int *> &profiles)
{
    _profiles = profiles;
}

Matrix<float *> PredictiveModel::getRepresentativeFeatures() const
{
    return _representativeFeatures;
}

void PredictiveModel::setRepresentativeFeatures(const Matrix<float *> &representativeFeatures)
{
    _representativeFeatures = representativeFeatures;
}

QMap<QString, int *> PredictiveModel::getProfiles() const
{
    return _profiles;
}

