#ifndef DATASETCONTROLLER_H
#define DATASETCONTROLLER_H

#include <QObject>

#include <services/datasetservice.h>

class DatasetController : public QObject
{
    Q_OBJECT
public:
    explicit DatasetController(QObject *parent = 0);
//    void setParamsToDSetCreation(
//            QString pathToEnts,
//            QString pathToId = "",
//            int sampleSize = 0,
//            bool sizeParams = false,
//            int minProtSize = -1,
//            int maxProtSize = -1,
//            bool classParams = false,
//            bool scop_class = false,
//            bool scop_fold = false,
//            bool scop_sfamily = false,
//            bool scop_family = false,
//            bool scop_species = false,);
    QList<SCOPEntry *> getVisualRepresentation(const Dataset *dataset);

    void setDatasetToProcess(const Dataset *value);
    void setEntPath(QString path = "");
    void setIdPath(QString path = "");
    void setSampleSize(unsigned size = 0);
    void setFilterSize(unsigned min = 0, unsigned max = UINT_MAX);
    void setScopClassFilter(SCOPEntry::SCOPCLASSLEVEL scopLevel);


public slots:
    void createDataset();
    void loadVisualRepresentation();
signals:
    void notifyProgress(int progress);
    void notifyTask(QString name);
    void datasetComplete (Dataset *dataset);
    void visualRepComplete (QList <SCOPEntry*> viewableData);

private slots:
    void serviceProgresReported(int progress);
    void serviceTasksReported(QString taskName);
 protected:
    void generateSampleFromDs(Dataset* ds);
//    void generateSample(
//            int minProtSize,
//            Dataset *newDataset,
//            int maxProtSize,
//            int sampleSize);

//    Dataset* createDataset(QString pathToEnts, int sampleSize, int minProtSize, int maxProtSize);
//    Dataset* createDataset(QString pathToId, QString pathToEnts, int sampleSize, int minProtSize, int maxProtSize);
private:
    DatasetService _service;

    QString _pathToEnts;
    QString _pathToId;

    bool _generateSample;
    unsigned _sampleSize;

    bool _sizeFiltered;
    unsigned _minProtSize;
    unsigned _maxProtSize;

    bool _runByScopClass;
    SCOPEntry::SCOPCLASSLEVEL _classLevel;

    const Dataset* _datasetToProcess;
};

#endif // DATASETCONTROLLER_H
