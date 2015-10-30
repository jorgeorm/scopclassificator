#ifndef DATASET_H
#define DATASET_H

#include "scopentry.h"

#include <QObject>

class Dataset : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString pathSampledProteins READ pathSampledProteins WRITE setPathSampledProteins NOTIFY pathSampledProteinsChanged)
    Q_PROPERTY(int sampleSize READ sampleSize WRITE setSampleSize NOTIFY sampleSizeChanged)
    Q_PROPERTY(bool hasSample READ hasSample WRITE setHasSample NOTIFY hasSampleChanged)
    Q_PROPERTY(QString pathProteins READ pathProteins WRITE setPathProteins NOTIFY pathProteinsChanged)
public:
    explicit Dataset(QObject *parent = 0);

    QString pathSampledProteins() const;
    void setPathSampledProteins(const QString &pathSampledProteins);

    int sampleSize() const;
    void setSampleSize(int sampleSize);

    bool hasSample() const;
    void setHasSample(bool hasSample);

    QString pathProteins() const;
    void setPathProteins(const QString &pathProteins);

    int size() const;
    void setSize(int size);

signals:
    void pathSampledProteinsChanged();
    void sampleSizeChanged();
    void hasSampleChanged();
    void pathProteinsChanged();
private:
    QString _pathSampledProteins;
    int _sampleSize, _size;
    bool _hasSample;
    QString _pathProteins;
};

#endif // DATASET_H
