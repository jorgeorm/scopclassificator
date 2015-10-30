#include "modeltrainingcontroller.h"

#include <services/algorithmservice.h>
#include <services/datasetservice.h>
#include <services/featureservice.h>
#include <QStringBuilder>

#include <QDateTime>
#include <QFileInfo>
#include <QThread>

ModelTrainingController::ModelTrainingController(QObject *parent) : QObject(parent){
    _cancelSearch = false;
    _threadCount = 0;
    _processedEntries = 0;
    _startedEntries = 0;
}

ModelTrainingController::~ModelTrainingController(){

    QThread * thread;
    foreach (thread, _runningTasks) {
        qDebug() << "Deleted thread";
        thread->quit();
        thread->wait();
    }

    if (! _runningServices.isEmpty()){
        qDeleteAll(_runningServices.values());
    }
    _runningServices.clear();
    if (! _runningTasks.isEmpty()){
        qDeleteAll(_runningTasks.values());
    }
    _runningTasks.clear();
    if (! _entries.isEmpty()){
        qDeleteAll(_entries);
    }

}

QString ModelTrainingController::clusteringTech() const{
    return _clusteringTech;
}

void ModelTrainingController::setClusteringTech(const QString &clusteringTech){
    _clusteringTech = clusteringTech;
}

QString ModelTrainingController::specificParams() const
{
    return _specificParams;
}

void ModelTrainingController::setSpecificParams(const QString &specificParams)
{
    _specificParams = specificParams;
}

void ModelTrainingController::configureClusteringTask(
        SCOPEntry *entry,
        QThread *taskThread,
        ClusteringService *clusteringTask) {

    AlgorithmService coder;
    QString preAlgorithm, algorithm, postAlgorithm, localOutputFile;

    localOutputFile = "/tmp/localClusters_" % entry->sid() % ".csv";

    preAlgorithm = coder.preAlgorithm(_clusteringTech);

    algorithm = coder.algorithm(_clusteringTech) %
            QString::number(_localClusters)%
            ", " % _specificParams % ");";
    postAlgorithm = coder.postAlgorithm(_clusteringTech, localOutputFile);

    clusteringTask->moveToThread(taskThread);
    clusteringTask->setName (entry->sid());
    clusteringTask->setEntry(entry);
    clusteringTask->setFeatDef(_featureDef);

    clusteringTask->setPreAlgorithm(preAlgorithm);
    clusteringTask->setAlgorithm(algorithm);
    clusteringTask->setPostAlgorithm(postAlgorithm);

    connect(clusteringTask, SIGNAL(commonFeaturesFound(QString)),
            this, SLOT(onCommonFeaturesFound(QString)));
    connect(clusteringTask, SIGNAL(errorOnClustering(QString)),
            this, SLOT(onClusteringError(QString)));
    connect(taskThread, SIGNAL(started()),
            clusteringTask, SLOT(findCommonFeatures()));
}

void ModelTrainingController::findModel()
{
    DatasetService ds;
    _processedEntries = 0;
    _startedEntries = 0;
    _threadCount = 0;
    _localFeatureFiles.clear();


    if (! _entries.isEmpty()) qDeleteAll(_entries);

    _entries = _dataset->hasSample() ? ds.sampledProteins(*_dataset) : ds.proteins(*_dataset);

    runLocalCommonSearchs();
    //TODO: Remove this to avoid processing if the file exists:
    if(_threadCount == 0) {
        runGlobalCommonSearch();
        emitProgress();
        emit modelFound();
    }

}

void ModelTrainingController::cancelFindingTasks(){

    _cancelSearch = true;

    QString entryOnProcess;
    foreach (entryOnProcess, _runningServices.keys()) {
        freeResourcesForEntry(entryOnProcess);
    }

    emit modelSearchCancelled();
}

void ModelTrainingController::runLocalCommonSearchs()
{
    ClusteringService *clusteringTask;
    QThread *taskThread;
    SCOPEntry *entry;

    while (_threadCount < _maxThreads &&
         _startedEntries < _entries.size ()){

        entry = _entries.at (_startedEntries);

//        TODO: Remove this lines, they avoid processing if file exists
        QString outputFile = "/tmp/localClusters_" %
                entry->sid() % ".csv";
        QFileInfo info (outputFile);
        if (info.exists()) {
            _localFeatureFiles << outputFile;
            ++_startedEntries;
            ++_processedEntries;
            emitProgress();
            continue;
        }
        // End of lines to be removed

        taskThread = new QThread (this);
        clusteringTask = new ClusteringService ();
        configureClusteringTask (entry, taskThread, clusteringTask);
        taskThread->start ();

        _runningServices.insert (entry->sid(), clusteringTask);
        _runningTasks.insert (entry->sid(), taskThread);

        ++_startedEntries;
        ++_threadCount;
    }
}

void ModelTrainingController::runGlobalCommonSearch(){

    AlgorithmService coder;
    ClusteringService clusteringTask;
    QString preAlgorithm, algorithm, postAlgorithm, localFeaturesFound;
    QDate myDate = QDate::currentDate();
    FeatureService featureConcatenator;

    _pathGlobalFeatures = "/tmp/globalClusters_" %
            myDate.toString("yyyy-MM-dd") % ".csv";

    localFeaturesFound = featureConcatenator.concatenateFeatureFiles(_localFeatureFiles);

    preAlgorithm = coder.preAlgorithm(_clusteringTech);

    algorithm = coder.algorithm(_clusteringTech) %
            QString::number(_globalClusters)%
            ", " % _specificParams % ");";
    postAlgorithm = coder.postAlgorithm(_clusteringTech, _pathGlobalFeatures);

    clusteringTask.setName (QString("Global"));
    clusteringTask.setPathDataFile(localFeaturesFound);
    clusteringTask.setFeatDef(_featureDef);

    clusteringTask.setPreAlgorithm(preAlgorithm);
    clusteringTask.setAlgorithm(algorithm);
    clusteringTask.setPostAlgorithm(postAlgorithm);

    connect(&clusteringTask, SIGNAL(errorOnClustering(QString)),
            this, SLOT(onClusteringError(QString)));

    clusteringTask.findCommonFeatures();
    ++_processedEntries;
    emitProgress();
}

void ModelTrainingController::freeResourcesForEntry(QString entryName){

    if(_runningServices.contains(entryName) || _runningTasks.contains(entryName)){

        ClusteringService *clusteringTask;
        QThread *taskThread;

        taskThread = _runningTasks.take(entryName);
        clusteringTask = _runningServices.take(entryName);
        taskThread->quit();
        taskThread->wait();
        delete taskThread;
        delete clusteringTask;
        --_threadCount;

        clusteringTask = NULL;
        taskThread = NULL;
    }

}

void ModelTrainingController::addDetailsToOutput(QString entryName){
    unsigned featureSize = _featureDef->sqrtSize();
    unsigned computedFeatureSize = featureSize*featureSize;
    QString outputFile, features;

    if (entryName == "Global"){
        features = QString::number(_globalClusters);
        QDate myDate = QDate::currentDate();
        outputFile = "/tmp/globalClusters_" %
                myDate.toString("yyyy-MM-dd") % ".csv";
    } else {
        features = QString::number(_localClusters);
        outputFile = "/tmp/localClusters_" %
                entryName % ".csv";
    }


    QString addRowsNColsCommand =
            "echo 'features: " %
            features %
            ", featureSize: " %
            QString::number(computedFeatureSize) %
            "' | cat - " % outputFile %
            " > temp && mv temp " % outputFile;

    int res = system(addRowsNColsCommand.toStdString().c_str());

    if (res != 0) {
        emit onClusteringError(entryName);
        qDebug() << "There was an error with this command: " << addRowsNColsCommand;
    }
}

void ModelTrainingController::onCommonFeaturesFound(QString entryName){

    ++_processedEntries;

    //addDetailsToOutput(entryName); // Adds metadata to the output of the clustering algorithm

    _localFeatureFiles.append("/tmp/localClusters_" % entryName % ".csv");

    freeResourcesForEntry(entryName);// Stop threads and free resources in regards of the entryname

    runLocalCommonSearchs();// Creates new clustering tasks if there are pending entries to be processed

    if (_processedEntries == (unsigned) _entries.size ()){
        runGlobalCommonSearch();
        qDebug() << "Just found the global features";
        emitProgress();

        createPredictiveModel();

        //TODO: Create model based on what was found use _pathGlobalFeatures to calculate model

        emit modelFound();
    }

    emitProgress();

}

void ModelTrainingController::saveModel(QString filePath){
    //TODO: Store model obtained
    qDebug() << "Gonna save the model obtained in: " << filePath;
}

void ModelTrainingController::onClusteringError(QString scriptPath){
    //TODO: Log error with the system logger
    qDebug() << "Found errors in script " << scriptPath;

    emit clusteringError(scriptPath);
}

void ModelTrainingController::emitProgress(){
    int total = _entries.size() + 1;
    int progress = (int) floor(((float) _processedEntries / (float) total) * 100.0);

    if (progress > 100) progress = 100;

    emit notifyProgress(progress);
}

FeatureDefinition *ModelTrainingController::featureDef() const
{
    return _featureDef;
}

void ModelTrainingController::setFeatureDef(FeatureDefinition *featureDef)
{
    _featureDef = featureDef;
}

Dataset *ModelTrainingController::dataset() const
{
    return _dataset;
}

void ModelTrainingController::setDataset(Dataset *dataset)
{
    _dataset = dataset;
}

unsigned ModelTrainingController::globalClusters() const
{
    return _globalClusters;
}

void ModelTrainingController::setGlobalClusters(const unsigned &globalClusters)
{
    _globalClusters = globalClusters;
}

unsigned ModelTrainingController::localClusters() const
{
    return _localClusters;
}

void ModelTrainingController::setLocalClusters(const unsigned &localClusters)
{
    _localClusters = localClusters;
}

unsigned ModelTrainingController::maxThreads() const
{
    return _maxThreads;
}

void ModelTrainingController::setMaxThreads(const unsigned &maxThreads)
{
    if (_maxThreads != maxThreads) {

        _maxThreads = maxThreads;
        emit maxThreadsChanged();
    }
}



