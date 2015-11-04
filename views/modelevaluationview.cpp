#include "modelevaluationview.h"
#include "ui_modelevaluationview.h"

ModelEvaluationView::ModelEvaluationView(QWidget *parent) :
    QGroupBox(parent),
    ui(new Ui::ModelEvaluationView),
    _controller(){

    ui->setupUi(this);
    this->setVisible(false);
    ui->progress_qpb->setVisible(false);

    connect(&_controller, SIGNAL(dataLoaded()),
            this, SLOT(onTestDataLoaded()));
}

ModelEvaluationView::~ModelEvaluationView(){
    delete ui;
}

Dataset *ModelEvaluationView::dataset() const
{
    return _controller.dataset();
}

void ModelEvaluationView::setDataset(Dataset *dataset){
    if (dataset == NULL) return;
    if (dataset->hasSample()){
        ui->pathEntities_qle->setText(dataset->pathProteins());
    }
    _controller.setDataset(dataset);
}

PredictiveModel *ModelEvaluationView::predictiveModel() const{
    return _controller.predictiveModel();
}

void ModelEvaluationView::setPredictiveModel(PredictiveModel *predictiveModel){
    if (predictiveModel == NULL) return;
    _controller.setPredictiveModel( predictiveModel);
}

void ModelEvaluationView::on_loadEntities_qpb_clicked(){
    QString pathEntities = ui->pathEntities_qle->text();
    _controller.dataset()->setPathProteins(pathEntities);
    _controller.loadData();
}

void ModelEvaluationView::on_evaluate_qpb_clicked(){
    _controller.evaluateClassifier();
}

void ModelEvaluationView::on_save_qpb_clicked(){
    QString outputFilePath = "";
    _controller.saveModel(outputFilePath);
}

void ModelEvaluationView::onTestDataLoaded(){
    QList<PredictedClassification *> classifications = _controller.classifications();
}
