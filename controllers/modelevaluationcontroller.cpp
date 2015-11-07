#include "modelevaluationcontroller.h"

#include <services/datasetservice.h>
#include <services/predictivemodelservice.h>
#include <services/scopentryservice.h>

ModelEvaluationController::ModelEvaluationController(QObject *parent) : QObject(parent)
{
    _dataset = NULL;
    _predictiveModel = NULL;
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
        _classifications.append(classification);
    }
    emit dataLoaded();
}

void ModelEvaluationController::saveModel(QString path){

    emit evaluationStored();
}

void ModelEvaluationController::evaluateClassifier(){
    PredictiveModelService evaluator;
    QString classTag = "";
    SCOPEntry *entryMetadata = NULL;
    SCOPEntry *entryFull = NULL;
    SCOPEntryService scopLoader;

    if (_predictiveModel == NULL) {
        qDebug() << "The model is empty: ";
        return;
    }

    foreach (PredictedClassification *classification,
             _classifications) {
        entryMetadata = classification->entry();
        entryFull = scopLoader.loadEntFile(entryMetadata->sourceFile());
        classTag = evaluator.classify(_predictiveModel,
                                      entryFull);
        delete entryFull;
        classification->setClassification(classTag);
    }

    emit classifierEvaluated();
}

QList<PredictedClassification *> ModelEvaluationController::classifications() const
{
    return _classifications;
}

void ModelEvaluationController::setClassifications(const QList<PredictedClassification *> &classifications)
{
    _classifications = classifications;
}

