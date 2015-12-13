#include "featuredefinition.h"

FeatureDefinition::FeatureDefinition(QObject *parent) : QObject(parent)
{
    _treshold = 0;
}

FeatureDefinition::GeneratorMethod FeatureDefinition::method() const
{
    return _method;
}

void FeatureDefinition::setMethod(const GeneratorMethod &method)
{
    if (_method != method){
        _method = method;
        emit methodChanged();
    }
}

int FeatureDefinition::sqrtSize() const
{
    return _sqrtSize;
}

void FeatureDefinition::setSqrtSize(int sqrtSize)
{
    _sqrtSize = sqrtSize;
}

double FeatureDefinition::treshold() const
{
    return _treshold;
}

void FeatureDefinition::setTreshold(double treshold)
{
    _treshold = treshold;
}

int FeatureDefinition::calculatedSize() const{
    int size = 0;

    switch (_method) {
    case Distance:
    case Angle:
        size = _sqrtSize*_sqrtSize;
        break;
    case Mixture:
        size = _sqrtSize*_sqrtSize*2;
        break;
    }

    return size;
}

