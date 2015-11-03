#ifndef MODELEVALUATIONCONTROLLER_H
#define MODELEVALUATIONCONTROLLER_H

#include <QObject>

#include <models/dataset.h>
#include <models/modelevaluation.h>
#include <models/predictivemodel.h>

class ModelEvaluationController : public QObject
{
    Q_OBJECT
public:
    explicit ModelEvaluationController(QObject *parent = 0);
    virtual ~ModelEvaluationController();



    Dataset *dataset() const;
    void setDataset(Dataset *dataset);

    PredictiveModel *predictiveModel() const;
    void setPredictiveModel(PredictiveModel *predictiveModel);

signals:

public slots:

private:
    Dataset *_dataset;
    PredictiveModel *_predictiveModel;
    ModelEvaluation *_modelEvaluation;
};

#endif // MODELEVALUATIONCONTROLLER_H
