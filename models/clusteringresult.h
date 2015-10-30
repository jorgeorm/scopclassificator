#ifndef CLUSTERINGRESULT_H
#define CLUSTERINGRESULT_H

#include <models/utils/matrix.h>



class ClusteringResult
{
public:
    ClusteringResult();

    Matrix<float> *originalData() const;
    void setOriginalData(Matrix<float> *originalData);

    Matrix<float> *clusters() const;
    void setClusters(Matrix<float> *clusters);

    unsigned *clusterMedoids() const;
    void setClusterMedoids(unsigned *clusterMedoids);

    QString algorithm() const;
    void setAlgorithm(const QString &algorithm);

    bool hasMedoids() const;
    void setHasMedoids(bool hasMedoids);

private:
    Matrix<float> *_originalData, *_clusters;
    unsigned *_clusterMedoids;
    QString _algorithm;
    bool _hasMedoids;
};

#endif // CLUSTERINGRESULT_H
