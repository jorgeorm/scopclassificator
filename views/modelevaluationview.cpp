#include "modelevaluationview.h"
#include "ui_modelevaluationview.h"

ModelEvaluationView::ModelEvaluationView(QWidget *parent) :
    QGroupBox(parent),
    ui(new Ui::ModelEvaluationView),
    _controller(){

    ui->setupUi(this);
    this->setVisible(false);
    ui->progress_qpb->setVisible(false);
}

ModelEvaluationView::~ModelEvaluationView()
{
    delete ui;
}

Dataset *ModelEvaluationView::dataset() const
{
    return _controller.dataset();
}

void ModelEvaluationView::setDataset(Dataset *dataset)
{
    _controller.setDataset(dataset);
}

PredictiveModel *ModelEvaluationView::predictiveModel() const
{
    return _controller.predictiveModel();
}

void ModelEvaluationView::setPredictiveModel(PredictiveModel *predictiveModel)
{
    _controller.setPredictiveModel( predictiveModel);
}
