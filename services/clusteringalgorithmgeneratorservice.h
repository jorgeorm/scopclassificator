#ifndef CLUSTERINGALGORITHMGENERATORSERVICE_H
#define CLUSTERINGALGORITHMGENERATORSERVICE_H

#include <QMap>
#include <QString>



class ClusteringAlgorithmGeneratorService
{
public:
    ClusteringAlgorithmGeneratorService();
    /**
     * @brief getAlgorithm
     * @param algorithm
     * @param additionalParams
     * @return
     */
    QString getAlgorithm(QString algorithm, QString additionalParams);

    QString getPreAlgorithm() const;
    void setPreAlgorithm(const QString &preAlgorithm);

    QString getPostAlgorithm() const;
    void setPostAlgorithm(const QString &postAlgorithm);

private:
    QMap<QString, QString> _baseAlgorithms;
    QString _preAlgorithm;
    QString _postAlgorithm;
};

#endif // CLUSTERINGALGORITHMGENERATORSERVICE_H
