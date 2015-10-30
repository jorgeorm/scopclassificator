#ifndef MODELTRAININGCONTROLLER_H
#define MODELTRAININGCONTROLLER_H

#include <QObject>

#include <models/utils/matrix.h>

#include <services/clusteringservice.h>

#include <models/dataset.h>
#include <models/featuredefinition.h>

class ModelTrainingController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(unsigned maxThreads READ maxThreads WRITE setMaxThreads NOTIFY maxThreadsChanged)
    Q_PROPERTY(unsigned localClusters READ localClusters WRITE setLocalClusters NOTIFY localClustersChanged)
    Q_PROPERTY(unsigned globalClusters READ globalClusters WRITE setGlobalClusters NOTIFY globalClustersChanged)
    Q_PROPERTY(Dataset* dataset READ dataset WRITE setDataset NOTIFY datasetChanged)

public:
    explicit ModelTrainingController(QObject *parent = 0);
    virtual ~ModelTrainingController();

    QString clusteringTech() const;
    void setClusteringTech(const QString &clusteringTech);

    QString specificParams() const;
    void setSpecificParams(const QString &specificParams);

    unsigned maxThreads() const;
    void setMaxThreads(const unsigned &maxThreads);

    unsigned localClusters() const;
    void setLocalClusters(const unsigned &localClusters);

    unsigned globalClusters() const;
    void setGlobalClusters(const unsigned &globalClusters);

    Dataset *dataset() const;
    void setDataset(Dataset *dataset);

    FeatureDefinition *featureDef() const;
    void setFeatureDef(FeatureDefinition *featureDef);

signals:
    /**
     * @brief modelFound Notifies UI about model founded
     */
    void modelFound();
    void maxThreadsChanged();
    void modelSearchCancelled();
    void localClustersChanged();
    void globalClustersChanged();
    void datasetChanged();

    /**
     * @brief notifyProgress Notifies UI about progress made
     * @param progress
     */
    void notifyProgress(int progress);

    /**
     * @brief clusteringError Notifies UI about error
     * @param script
     */
    void clusteringError(QString script);
protected slots:
    void onCommonFeaturesFound(QString entryName);

public slots:
    void findModel();
    void cancelFindingTasks();
    void saveModel(QString filePath);
    void onClusteringError(QString scriptPath);

protected:
    void emitProgress();

    /**
     * @brief createPredictiveModel Generates the predictive model
     */
    void createPredictiveModel();

    /**
     * @brief configureClusteringTask Creates Clustering procesors
     * @param entry
     * @param taskThread
     * @param clusteringTask
     */
    void configureClusteringTask(SCOPEntry *entry,
                                 QThread *taskThread,
                                 ClusteringService *clusteringTask);

    /**
     * @brief runLocalCommonSearchs Creates clustering processes if there are pending entries to be processed
     */
    void runLocalCommonSearchs();

    /**
     * @brief runGlobalCommonSearch Creates final clustering process
     */
    void runGlobalCommonSearch();

    /**
     * @brief freeResourcesForEntry Stop and deletes pointer related to an entry name
     * @param entryName
     */
    void freeResourcesForEntry(QString entryName);

    /**
     * @brief addDetailsToOutput Parses the amount of features and featuresize to the matrix file
     * @param entryName
     */
    void addDetailsToOutput(QString entryName);

private:
    QString _clusteringTech,_specificParams, _pathDataFrame, _pathGlobalFeatures;
    unsigned _maxThreads, _localClusters,
    _globalClusters, _startedEntries,
    _processedEntries, _threadCount;

    QMap<QString, ClusteringService *> _runningServices;
    QMap<QString, QThread *> _runningTasks;

    QList<SCOPEntry *> _entries;

    Dataset *_dataset;
    FeatureDefinition *_featureDef;
    QStringList _localFeatureFiles;

    QList<unsigned> _globalMedoids;
    Matrix<float> *_localCommonFeatures, *_globalCommonFeatures;
    bool _cancelSearch;
};

#endif // MODELTRAININGCONTROLLER_H
