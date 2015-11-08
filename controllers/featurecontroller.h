#ifndef FEATURECONTROLLER_H
#define FEATURECONTROLLER_H

#include <QFile>
#include <QObject>
#include <models/dataset.h>
#include <models/utils/matrix.h>
#include "models/featuredefinition.h"

class FeatureController : public QObject
{
    Q_OBJECT
public:
    explicit FeatureController(QObject *parent = 0);    

    FeatureDefinition *featureDefinition() const;
    void setFeatureDefinition(FeatureDefinition *featureDefinition);

    unsigned int graphicSampleSize() const;
    void setGraphicSampleSize(unsigned int graphicSampleSize);

    void setDataset(const Dataset *datasetTobeProcessed);

    QString svgFile() const;
    
signals:
    void featureGraphicsCompleted();
    void notifyTask(QString);
    void notifyProgress(unsigned int progress);

public slots:
    void generateFeatureGraphic();

protected:
    void filterSVGFile();
private:

    /**
     * @brief emitProgress
     * @param counter
     * @param total
     */
    void emitProgress(int counter, int total);

    const Dataset * _datasetTobeProcessed;
    FeatureDefinition *_featureDefinition;
    unsigned int _graphicSampleSize;

    QString _svgFile;
    QString _rTempFile;

};

#endif // FEATURECONTROLLER_H
