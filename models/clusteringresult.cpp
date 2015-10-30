#include "clusteringresult.h"

ClusteringResult::ClusteringResult()
{

}

Matrix<float> *ClusteringResult::originalData() const
{
    return _originalData;
}

void ClusteringResult::setOriginalData(Matrix<float> *originalData)
{
    _originalData = originalData;
}

Matrix<float> *ClusteringResult::clusters() const
{
    return _clusters;
}

void ClusteringResult::setClusters(Matrix<float> *clusters)
{
    _clusters = clusters;
}

unsigned *ClusteringResult::clusterMedoids() const
{
    return _clusterMedoids;
}

void ClusteringResult::setClusterMedoids(unsigned *clusterMedoids)
{
    _clusterMedoids = clusterMedoids;
}

QString ClusteringResult::algorithm() const
{
    return _algorithm;
}

void ClusteringResult::setAlgorithm(const QString &algorithm)
{
    _algorithm = algorithm;
}

bool ClusteringResult::hasMedoids() const
{
    return _hasMedoids;
}

void ClusteringResult::setHasMedoids(bool hasMedoids)
{
    _hasMedoids = hasMedoids;
}

