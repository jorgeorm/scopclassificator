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
    ~ModelEvaluationView();


    Dataset *dataset() const;
    void setDataset(Dataset *dataset);

    PredictiveModel *predictiveModel() const;
    void setPredictiveModel(PredictiveModel *predictiveModel);

private:
    Ui::ModelEvaluationView *ui;
    ModelEvaluationController _controller;
};

#endif // MODELEVALUATIONVIEW_H
