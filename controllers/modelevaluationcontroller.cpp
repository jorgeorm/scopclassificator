#include "modelevaluationcontroller.h"

ModelEvaluationController::ModelEvaluationController(QObject *parent) : QObject(parent)
{
    _dataset = NULL;
    _predictiveModel = NULL;
    _modelEvaluation = NULL;
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

