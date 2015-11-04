#ifndef MODELEVALUATIONCONTROLLER_H
#define MODELEVALUATIONCONTROLLER_H

#include <QObject>

#include <models/dataset.h>
#include <models/predictedclassification.h>
#include <models/predictivemodel.h>

class ModelEvaluationController : public QObject
{
    Q_OBJECT
public:
    explicit ModelEvaluationController(QObject *parent = 0);

    Dataset *dataset() const;
    void setDataset(Dataset *dataset);

    PredictiveModel *predictiveModel() const;
    void setPredictiveModel(PredictiveModel *predictiveModel);

    QList<PredictedClassification *> classifications() const;
    void setClassifications(const QList<PredictedClassification *> &classifications);

signals:
    void dataLoaded();
    void evaluationStored();
    void classifierEvaluated();
public slots:
    void loadData();
    void saveModel(QString path);
    void evaluateClassifier();

private:
    Dataset *_dataset;
    PredictiveModel *_predictiveModel;
    QList<PredictedClassification*> _classifications;
};

#endif // MODELEVALUATIONCONTROLLER_H
