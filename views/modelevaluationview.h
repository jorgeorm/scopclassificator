#ifndef MODELEVALUATIONVIEW_H
#define MODELEVALUATIONVIEW_H

#include <QGroupBox>

#include <models/dataset.h>
#include <models/predictivemodel.h>

#include <controllers/modelevaluationcontroller.h>

namespace Ui {
class ModelEvaluationView;
}

class ModelEvaluationView : public QGroupBox
{
    Q_OBJECT

public:
    explicit ModelEvaluationView(QWidget *parent = 0);
    virtual ~ModelEvaluationView();


    Dataset *dataset() const;
    void setDataset(Dataset *dataset);

    PredictiveModel *predictiveModel() const;
    void setPredictiveModel(PredictiveModel *predictiveModel);

private slots:
    void on_loadEntities_qpb_clicked();

    void on_evaluate_qpb_clicked();

    void on_save_qpb_clicked();

    void onTestDataLoaded();

private:
    Ui::ModelEvaluationView *ui;
    ModelEvaluationController _controller;
};

#endif // MODELEVALUATIONVIEW_H
