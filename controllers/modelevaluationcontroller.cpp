#include "modelevaluationcontroller.h"

#include <services/datasetservice.h>
#include <services/predictivemodelservice.h>
#include <services/scopentryservice.h>
#include <QFile>
#include <QThread>

ModelEvaluationController::ModelEvaluationController(QObject *parent) : QObject(parent)
{
    _dataset = NULL;
    _predictiveModel = NULL;
    _classificationsCompleted = 0;
}

Dataset *ModelEvaluationController::dataset() const
{
    return _dataset;
}

void ModelEvaluationController::setDataset(Dataset *dataset)
{
    _dataset = dataset;
}

PredictiveModel *ModelEvaluationController::predictiveModel() const
{
    return _predictiveModel;
}

void ModelEvaluationController::setPredictiveModel(PredictiveModel *predictiveModel)
{
    _predictiveModel = predictiveModel;
}

void ModelEvaluationController::loadData(){
    PredictedClassification *classification = NULL;
    DatasetService dsLoader;

    QList<SCOPEntry *> entries = dsLoader.proteinsMetadata(*_dataset);
    foreach (SCOPEntry *entry, entries) {
        classification = new PredictedClassification();
        classification->setEntry(entry);
        _classifications.insert(entry->sid(), classification);
    }
    emit dataLoaded();
}

void ModelEvaluationController::saveEvaluation(QString path){
    QFile ofile(path);

    if(ofile.open(QIODevice::WriteOnly)){
        QTextStream ostream(&ofile);
        ostream << "POTEIN, SCOP-CLASSIFICATION, NEAREST_NEIGHBOR, SCOP-CLASSIFICATION" << endl;

        foreach (PredictedClassification *classification, classifications()) {
            ostream << classification->entry()->sid() << ", ";
            ostream << classification->entry()->scss()<< ", ";
            ostream << classification->nearestNeighbor()<< ", ";
            ostream << classification->nearestNeighborScss()<< endl;
        }

        ofile.close();
    }

    emit evaluationStored();
}

//void ModelEvaluationController::evaluateClassifier(){
//    PredictiveModelService evaluator;
//    QString classTag = "";
//    SCOPEntry *entryMetadata = NULL;
//    SCOPEntry *entryFull = NULL;
//    SCOPEntryService scopLoader;

//    if (_predictiveModel == NULL) {
//        qDebug() << "The model is empty: ";
//        return;
//    }
//    foreach (PredictedClassification *classification,
//             _classifications) {
//        entryMetadata = classification->entry();
//        entryFull = scopLoader.loadEntFile(entryMetadata->sourceFile());
//        classTag = evaluator.classify(_predictiveModel,
//                                      entryFull);
//        delete entryFull;
//        classification->setClassification(classTag);
//    }

//    emit classifierEvaluated();
//}

void ModelEvaluationController::evaluateClassifier(){
    if (_predictiveModel == NULL) {
        qDebug() << "The model is empty: ";
        return;
    }
    runClassificationTasks();
}

void ModelEvaluationController::notifyProgress(unsigned count, unsigned total){
    double progress = (float)count / (float) total;
    emit notifyProgress((int) floor(progress*100));
}

void ModelEvaluationController::runClassifications()
{
    PredictiveModelService *classifier;
    QThread *classifier_thread;
    SCOPEntry *entryMetadata;
    SCOPEntry *entry;
    SCOPEntryService scopLoader;

    while(_threadCount < _maxClassifierThreads &&
          _classificationsStarted < (unsigned)_classifications.size()){
        entryMetadata = _classifications.values().at(_classificationsStarted)->entry();

        classifier_thread = new QThread();
        classifier = new PredictiveModelService();
        classifier->moveToThread(classifier_thread);
        classifier->setDistance(_distanceType);
        entry = scopLoader.loadEntFile(entryMetadata->sourceFile());
        classifier->setEntry(entry);
        classifier->setModel(_predictiveModel);

        connect(classifier, SIGNAL(entryClassified(QString)),
                this, SLOT(onEntryClassified(QString)));
        connect(classifier_thread, SIGNAL(started()),
                classifier, SLOT(runClassification()));
        classifier_thread->start();
        _classifiers.insert(entryMetadata->sid(), classifier);
        _classifiersThreads.insert(entryMetadata->sid(), classifier_thread);

        ++_classificationsStarted;
        ++_threadCount;
    }
}

unsigned ModelEvaluationController::maxClassifierThreads() const
{
    return _maxClassifierThreads;
}

void ModelEvaluationController::setMaxClassifierThreads(const unsigned &maxClassifierThreads)
{
    _maxClassifierThreads = maxClassifierThreads;
}

QString ModelEvaluationController::classTag() const
{
    return _classTag;
}

void ModelEvaluationController::runClassificationTasks(){
    
    _threadCount = 0;
    _classificationsStarted = 0;
    _classificationsCompleted = 0;
    
    runClassifications();
}

PredictiveModelService::DISTANCETYPE ModelEvaluationController::distanceType() const{
    return _distanceType;
}

void ModelEvaluationController::setDistanceType(const PredictiveModelService::DISTANCETYPE &distanceType){
    _distanceType = distanceType;
}

void ModelEvaluationController::onEntryClassified(QString entryName){
    ++_classificationsCompleted;
    if(_classifiers.contains(entryName) ||
            _classifiersThreads.contains(entryName)){
        PredictiveModelService *classifier;
        QThread *classifier_thread;

        classifier = _classifiers.take(entryName);
        _classifications.value(entryName)->setNearestNeighbor(classifier->nearestNeighbor());
        _classifications.value(entryName)->setNearestNeighborScss(classifier->nearestNeighborSCSS());

        classifier_thread = _classifiersThreads.take(entryName);
        classifier_thread->quit();
        classifier_thread->wait();
        delete classifier_thread;
        delete classifier;
        -- _threadCount;
        classifier = NULL;
        classifier_thread = NULL;
    }

    runClassifications();

    notifyProgress(_classificationsCompleted, _classifications.size());

    if(_classificationsCompleted == (unsigned)_classifications.size())
        emit classifierEvaluated();
}

QList<PredictedClassification *> ModelEvaluationController::classifications() const {
    return _classifications.values();
}

