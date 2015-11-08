#include "modelevaluationview.h"
#include "ui_modelevaluationview.h"
#include <QFileDialog>

ModelEvaluationView::ModelEvaluationView(QWidget *parent) :
    QGroupBox(parent),
    ui(new Ui::ModelEvaluationView),
    _controller(),
    _threadController(){

    ui->setupUi(this);
    this->setVisible(false);
    ui->pathEntities_qle->setReadOnly(true);
    ui->progress_qpb->setVisible(false);

    _controller.moveToThread(&_threadController);
    connect(&_controller, SIGNAL(dataLoaded()),
            this, SLOT(onTestDataLoaded()));
    connect(&_controller, SIGNAL(notifyProgress(int)),
            this, SLOT(onProgressNotified(int)));
    connect(&_controller, SIGNAL(classifierEvaluated()),
            this, SLOT(onModelEvaluated()));
    _controller.setMaxClassifierThreads(1);
    _controller.setDistanceType(PredictiveModelService::EUCLIDEAN);

}

ModelEvaluationView::~ModelEvaluationView(){
    delete ui;
    _threadController.quit();
    _threadController.wait();
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
    _threadController.quit();
    _threadController.wait();
    connect(&_threadController, SIGNAL(started()),
            &_controller, SLOT(evaluateClassifier()));

    ui->progress_qpb->setValue(0);
    ui->evaluate_qpb->setEnabled(false);
    ui->save_qpb->setEnabled(false);
    ui->loadEntities_qpb->setEnabled(false);

    _threadController.start();
}

void ModelEvaluationView::onTestDataLoaded(){
    QList<PredictedClassification *> classifications = _controller.classifications();
}

void ModelEvaluationView::onProgressNotified(int progress){
    if (! ui->progress_qpb->isVisible()) ui->progress_qpb->setVisible(true);
    ui->progress_qpb->setValue(progress);
}

void ModelEvaluationView::onModelEvaluated(){
    disconnect(&_threadController, SIGNAL(started()),
               &_controller, SLOT(evaluateClassifier()));

    ui->evaluate_qpb->setEnabled(true);
    ui->save_qpb->setEnabled(true);
    ui->loadEntities_qpb->setEnabled(true);
}

void ModelEvaluationView::on_save_qpb_clicked(){
    QString filetypes("SCOP Evaluation (*.SCOPEval);;");

    QString outputFilePath = QFileDialog::getSaveFileName(this,
                                                          "Guardar resultados de evaluación",
                                                          QDir::homePath(),
                                                          filetypes);  //selected file type
    _controller.saveEvaluation(outputFilePath);
}

void ModelEvaluationView::on_pathEntities_qtb_clicked(){
    QString pathTestData = QFileDialog::getExistingDirectory ( this, "Escoger folder de archivos de prueba");
    ui->pathEntities_qle->setText(pathTestData);
}

void ModelEvaluationView::on_simultClass_qsb_valueChanged(int maxThreads){
    _controller.setMaxClassifierThreads((unsigned)maxThreads);
}


void ModelEvaluationView::on_distance_cbx_currentIndexChanged(int index){
    PredictiveModelService::DISTANCETYPE distance;
    switch (index){
    case 0:
        distance = PredictiveModelService::EUCLIDEAN;
        break;
    case 1:
        distance = PredictiveModelService::COSINE;
        break;
    }

    _controller.setDistanceType(distance);
}
