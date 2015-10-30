#include "dataset.h"

Dataset::Dataset(QObject *parent) : QObject(parent){
    _hasSample = false;
    _sampleSize = 0;
    _size = 0;
    _pathSampledProteins = "";
    _pathProteins = "";

}

QString Dataset::pathSampledProteins() const
{
    return _pathSampledProteins;
}

void Dataset::setPathSampledProteins(const QString &pathSampledProteins)
{
    _hasSample = true;
    _pathSampledProteins = pathSampledProteins;
}

int Dataset::sampleSize() const
{
    return _sampleSize;
}

void Dataset::setSampleSize(int sampleSize)
{
    _hasSample = true;
    _sampleSize = sampleSize;
}

bool Dataset::hasSample() const
{
    return _hasSample;
}

void Dataset::setHasSample(bool hasSample)
{
    _hasSample = hasSample;
}

QString Dataset::pathProteins() const
{
    return _pathProteins;
}

void Dataset::setPathProteins(const QString &pathProteins)
{
    _pathProteins = pathProteins;
}

int Dataset::size() const
{
    return _size;
}

void Dataset::setSize(int size)
{
    _size = size;
}


