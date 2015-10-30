#include "clusteringalgorithmgeneratorservice.h"
#include <QStringBuilder>

ClusteringAlgorithmGeneratorService::ClusteringAlgorithmGeneratorService():
_baseAlgorithms(){
    _baseAlgorithms.insert("CLARA", "clara(x,");
    _preAlgorithm = "";
    _postAlgorithm = "";
}

QString ClusteringAlgorithmGeneratorService::getPreAlgorithm() const
{
    return _preAlgorithm;
}

void ClusteringAlgorithmGeneratorService::setPreAlgorithm(const QString &preAlgorithm)
{
    _preAlgorithm = preAlgorithm;
}

QString ClusteringAlgorithmGeneratorService::getPostAlgorithm() const
{
    return _postAlgorithm;
}

void ClusteringAlgorithmGeneratorService::setPostAlgorithm(const QString &postAlgorithm)
{
    _postAlgorithm = postAlgorithm;
}

QString ClusteringAlgorithmGeneratorService::getAlgorithm(QString algorithm, QString additionalParams)
{
    QString rClustAlg = "";
    if (_baseAlgorithms.contains(algorithm)){
        if (! _preAlgorithm.isEmpty()){
            rClustAlg = _preAlgorithm;
        }

        rClustAlg = rClustAlg % "\nresp <- " %
                _baseAlgorithms.value(algorithm)%
                additionalParams % ");";

        if (! _postAlgorithm.isEmpty()) {
            rClustAlg = rClustAlg % _postAlgorithm;
        }
    }
    return rClustAlg;
}

