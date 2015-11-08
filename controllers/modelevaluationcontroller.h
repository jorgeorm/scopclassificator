#ifndef MODELEVALUATIONCONTROLLER_H
#define MODELEVALUATIONCONTROLLER_H

#include <QObject>

#include <models/dataset.h>
#include <models/predictedclassification.h>
#include <models/predictivemodel.h>

#include <services/predictivemodelservice.h>

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

    PredictiveModelService::DISTANCETYPE distanceType() const;
    void setDistanceType(const PredictiveModelService::DISTANCETYPE &distanceType);

    QString classTag() const;

    unsigned maxClassifierThreads() const;
    void setMaxClassifierThreads(const unsigned &maxClassifierThreads);

private slots:
    void onEntryClassified(QString entryName);

signals:
    void dataLoaded();
    void evaluationStored();
    void classifierEvaluated();
    void notifyProgress(int);

public slots:
    void loadData();
    void saveEvaluation(QString path);
    void evaluateClassifier();

protected:
    void notifyProgress(unsigned count, unsigned total);
    void runClassificationTasks();

private:
    void runClassifications();

    Dataset *_dataset;
    PredictiveModel *_predictiveModel;
    QMap<QString, PredictedClassification*> _classifications;
    unsigned _classifiedEntries, _classificationsStarted, _threadCount, _maxClassifierThreads, _classificationsCompleted;
    QMap<QString, QThread*> _classifiersThreads;
    QMap<QString, PredictiveModelService*> _classifiers;
    PredictiveModelService::DISTANCETYPE _distanceType;
    QString _classTag;
};

#endif // MODELEVALUATIONCONTROLLER_H
