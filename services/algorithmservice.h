#ifndef ALGORITHMSERVICE_H
#define ALGORITHMSERVICE_H

#include <QMap>
#include <QString>



class AlgorithmService
{
public:
    AlgorithmService();

    QString algorithm(QString algName);
    QString preAlgorithm(QString algName);
    QString postAlgorithm(QString algName, QString outputFile);

    QMap<QString, QString> _preAlgorithms, _algorithms, _postAlgorithms;
};

#endif // ALGORITHMSERVICE_H
