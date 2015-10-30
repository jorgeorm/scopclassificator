#include "modeltrainingview.h"
#include "ui_modeltrainingview.h"
#include <QThread>
#include <QFileDialog>
#include <views/clusteringSpecificViews/claraoptions.h>

ModelTrainingView::ModelTrainingView(QWidget *parent) :
    QGroupBox(parent),
    ui(new Ui::ModelTrainingView),
    _controller(),
    _controllerThread(){
//    connect(&_controllerThread, SIGNAL(finished()),
//            &_controller, SLOT(deleteLater()));

    ui->setupUi(this);
    this->setVisible(false);



    ClaraOptions *clara = new ClaraOptions(this);
    clara->setVisible(true);


    QLayout *lay = ui->algorithmConfig_qgbx->layout();

    if (lay == NULL){
        lay = new QVBoxLayout();
        ui->algorithmConfig_qgbx->setLayout(lay);

    }

    lay->addWidget(clara);
    _algorithmsSpecifics << clara;
    _currentOptions = clara;

    _controller.moveToThread(&_controllerThread);
    _controller.setClusteringTech("CLARA");
    _controller.setSpecificParams(clara->optionsAsString());
    _controller.setMaxThreads(ui->procesesNumber_qsbx->value());
    _controller.setLocalClusters(ui->localFeatures_qsb->value());
    _controller.setGlobalClusters(ui->globalFeatures_qsb->value());



    connect(&_controller, SIGNAL(modelFound()),
            this, SLOT(onModelFound()));
    connect(&_controller, SIGNAL(notifyProgress(int)),
            ui->progressBar, SLOT(setValue(int)));
    connect(&_controller, SIGNAL(modelSearchCancelled()),
            this, SLOT(onModelSearchCancelled()));
    connect(&_controllerThread, SIGNAL(started()),
            &_controller, SLOT(findModel()));

    connect(this, SIGNAL (featureDefChanged()), this, SLOT (onFeatureDefinitionChanged()));
    connect(this, SIGNAL (datasetChanged()), this, SLOT (onDatasetChanged()));

}

ModelTrainingView::~ModelTrainingView(){
    _controllerThread.quit();
    _controllerThread.wait();

    delete ui;
}

void ModelTrainingView::on_clustTech_qcbx_activated(int index){

    AlgorithmOptions *box;
    foreach(box, _algorithmsSpecifics){
        box->setVisible(false);
    }

    switch (index) {
    case 0:
        _currentOptions = _algorithmsSpecifics.at(0);
        break;
    case 1:
        _currentOptions = NULL;
        break;
    case 2:
        _currentOptions = NULL;
        break;
    case 3:
        _currentOptions = NULL;
        break;
    case 4:
        _currentOptions = NULL;
        break;
    case 5:
        _currentOptions = NULL;
        break;
    case 6:
        _currentOptions = NULL;
        break;
    default:
        _currentOptions = NULL;
        break;
    }

    _currentOptions->setVisible(true);
}

void ModelTrainingView::onFeatureDefinitionChanged(){

}

void ModelTrainingView::onDatasetChanged(){

}

void ModelTrainingView::onModelFound(){
    if (! ui->generate_btn->isEnabled()) ui->generate_btn->setEnabled(true);
    if (ui->stop_btn->isEnabled()) ui->stop_btn->setEnabled(false);
    if (ui->save_btn->isEnabled()) ui->save_btn->setEnabled(true);

    ui->progressBar->setValue(100);
}

FeatureDefinition *ModelTrainingView::featureDef() const
{
    return _controller.featureDef();
}

void ModelTrainingView::setFeatureDef(FeatureDefinition *featureDef)
{
    if (_controller.featureDef () != featureDef){
        _controller.setFeatureDef (featureDef);
        emit featureDefChanged ();
    }
}

Dataset *ModelTrainingView::dataset() const
{
    return _controller.dataset ();
}

void ModelTrainingView::setDataset(Dataset *dataset){

    if (_controller.dataset () != dataset) {

        _controller.setDataset (dataset);
        emit datasetChanged ();
    }
}

void ModelTrainingView::on_localFeatures_qsb_valueChanged(int arg1){
    _currentOptions->setClusters(std::max(arg1, ui->globalFeatures_qsb->value()));
    _controller.setLocalClusters (arg1);
}


void ModelTrainingView::on_procesesNumber_qsbx_valueChanged(int arg1){
    _controller.setMaxThreads (arg1);
}

void ModelTrainingView::on_generate_btn_clicked(){
    ui->generate_btn->setEnabled(false);
    if (! ui->stop_btn->isEnabled()) ui->stop_btn->setEnabled(true);
    if (ui->save_btn->isEnabled()) ui->save_btn->setEnabled(false);

    _controller.setSpecificParams(_currentOptions->optionsAsString());
    _controllerThread.start();
}

void ModelTrainingView::on_stop_btn_clicked(){
    _controller.cancelFindingTasks();
}

void ModelTrainingView::on_save_btn_clicked(){
    QString fileName = QFileDialog::getSaveFileName(this, tr("Guardar"),
                               "/home/jorge/modelObtained.scopmodel",
                               tr("Modelos (*.scopmodel)"));
    _controller.saveModel(fileName);
}

void ModelTrainingView::on_globalFeatures_qsb_valueChanged(int arg1){
    _currentOptions->setClusters (std::max (arg1, ui->globalFeatures_qsb->value ()));
    _controller.setGlobalClusters (arg1);
}

void ModelTrainingView::onModelSearchCancelled(){
    _controllerThread.quit();
    _controllerThread.wait();
}
