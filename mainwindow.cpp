#include "mainwindow.h"
#include "ui_mainwindow.h"

// Animaciones
#include <QDebug>
#include <QGraphicsOpacityEffect> /*Animates the change on the uis*/
#include <QPropertyAnimation> /*Type of animations*/
#include <views/datasetview.h>
#include <QMessageBox>

void MainWindow::initParams()
{
    ui->setupUi(this);
    ui->variableWidget->setVisible(false);
    ui->clb_defineCharact->setChecked(true);
    animationDuration = 250;
    statusBar()->showMessage(tr("Listo"), 3000);

    _datasetView = NULL;
    _obtainedDataset = NULL;
    _obtainedFeaturesConf = NULL;
    _obtainedClassificationModel = NULL;
    _obtainedModelEvaluation = NULL;


    // Init other UI forms used
    _datasetView = new DatasetView (this);
    _featureView = new FeatureDefinitionView (this);
    _modelEvaluationView = new ModelEvaluationView(this);
    _modelTrainingView = new ModelTrainingView(this);

    // Connect auxform signals with proper slots
    connect(_datasetView, SIGNAL(eventNotified(QString)),
            this, SLOT(notifyEvent(QString)));
    connect(_datasetView, SIGNAL(stepCompleted()),
            this, SLOT(onDatasetObtained()));
    connect(_datasetView, SIGNAL(datasetDestroyed()),
            this, SLOT(onDatasetDestroyed()));

    connect(_featureView, SIGNAL(stepCompleted()),
            this, SLOT (onFeatureConfigObtained()));
    connect(_featureView, SIGNAL(notify(QString)),
            this, SLOT(notifyEvent(QString)));

    connect(_modelTrainingView, SIGNAL(stepCompleted()),
            this, SLOT(onClassificationModelObtained()));
    connect(_modelTrainingView, SIGNAL(notify(QString)),
            this, SLOT(notifyEvent(QString)));

    QLayout *variableLayout = ui->variableWidget->layout();

    if (variableLayout == NULL){
        variableLayout = new QVBoxLayout(ui->variableWidget);
    }

    variableLayout->setContentsMargins(0,9,0,0);

    // Add ui elements to the layout
    variableLayout->addWidget(_datasetView);
    variableLayout->addWidget(_featureView);
    variableLayout->addWidget(_modelTrainingView);
    variableLayout->addWidget(_modelEvaluationView);
}

void MainWindow::hideViews(){
    if(_featureView->isVisible()) _featureView->setVisible(false);
    if(_datasetView->isVisible()) _datasetView->setVisible(false);
    if(_modelEvaluationView->isVisible()) _modelEvaluationView->setVisible(false);
    if(_modelTrainingView->isVisible()) _modelTrainingView->setVisible(false);
}

//MainWindow::MainWindow(QWidget *parent) :
//    QMainWindow(parent), ui(new Ui::MainWindow){
//    _R = NULL;
//    initParams();
//}

MainWindow::MainWindow(RInside & R, QWidget *parent):
    QMainWindow(parent), ui(new Ui::MainWindow), _R(R) {

    initParams();

    if (_R.instancePtr() != NULL) _featureView->setRInstance(
                _R.instancePtr());


}

MainWindow::~MainWindow()
{
    delete ui;
    if (_obtainedDataset != NULL) delete _obtainedDataset;
    if (_obtainedFeaturesConf != NULL) delete _obtainedFeaturesConf;
    if (_obtainedClassificationModel != NULL) delete _obtainedClassificationModel;
    if (_obtainedModelEvaluation != NULL) delete _obtainedModelEvaluation;


    if(_datasetView != NULL) delete _datasetView;
    if(_featureView != NULL) delete _featureView;
    if(_modelTrainingView != NULL) delete _modelTrainingView;
    if(_modelEvaluationView != NULL) delete _modelEvaluationView;
}


void MainWindow::notifyEvent(QString event){
    statusBar()->showMessage(event, 10);
}

void MainWindow::resizeSidebar (int _width){
    ui->sidebarWidget->setEnabled(false);
    QPropertyAnimation *resizeAnimation = new QPropertyAnimation (ui->sidebarWidget, "size");
    resizeAnimation->setDuration (animationDuration);
    resizeAnimation->setStartValue (QSize (ui->sidebarWidget->width(), ui->sidebarWidget->height()));
    resizeAnimation->setEndValue (QSize (_width, ui->sidebarWidget->height()));

    connect (resizeAnimation,
             SIGNAL (finished ()),
             this,
             SLOT (onAnimationResizeFinish ()));

    resizeAnimation->start (QPropertyAnimation::DeleteWhenStopped);

}

void MainWindow::onAnimationResizeFinish (){
    ui->sidebarWidget->setEnabled(true);
    this->fadeInVariableWidget();
}

void MainWindow::fadeInVariableWidget(bool fade_in){

    _fadeDirection = fade_in;

    QGraphicsOpacityEffect* fade_effect = new QGraphicsOpacityEffect(ui->variableWidget);
    ui->variableWidget->setGraphicsEffect(fade_effect);
    QPropertyAnimation *animation = new QPropertyAnimation(fade_effect, "opacity");
    animation->setEasingCurve(QEasingCurve::InOutQuad);
    animation->setDuration(animationDuration);
    animation->setStartValue(0.01);
    animation->setEndValue(1.0);

    if (! fade_in) {
        animation->setDirection (QAbstractAnimation::Backward);
    }

    connect(animation,
            SIGNAL(finished()),
            this,
            SLOT(onAnimationFadeInFinish()));

    ui->variableWidget->setVisible(true);

    animation->start(QPropertyAnimation::DeleteWhenStopped);
}

void MainWindow::onAnimationFadeInFinish(){
    if(_fadeDirection) ui->variableWidget->setEnabled(true);
    else ui->variableWidget->setEnabled(false);

}

void MainWindow::onDatasetObtained(){

    //Obtain the data
    _obtainedDataset = _datasetView->dataset();

    // -- enabling next step
    ui->clb_selectDataset->setChecked(false);
    ui->clb_defineCharact->setEnabled(true);
    ui->clb_defineCharact->setChecked(true);
    ui->variableWidget->setEnabled(false);


    // -- removing and enabling uis
    fadeInVariableWidget(false);
    hideViews();
}

void MainWindow::onClassificationModelObtained(){
    _obtainedClassificationModel = _modelTrainingView->model();

    // Enables next step
    ui->clb_selectDataset->setChecked(false);
    ui->clb_defineCharact->setChecked(false);
    ui->clb_trainModel->setChecked(false);
    ui->clb_testModel->setEnabled(true);
    ui->clb_testModel->setChecked(true);

    // -- removing and enabling uis
    // fadeInVariableWidget(false);
    // hideViews();
}

void MainWindow::onDatasetDestroyed(){
    if(ui->clb_defineCharact->isEnabled()) ui->clb_defineCharact->setEnabled(false);

    if(ui->clb_trainModel->isEnabled()) ui->clb_trainModel->setEnabled(false);

    if(ui->clb_testModel->isEnabled()) ui->clb_testModel->setEnabled(false);

    _obtainedDataset = NULL;
}

void MainWindow::onFeatureConfigObtained(){
    _obtainedFeaturesConf = _featureView->featuresDefinition();

    // -- enabling next step
    ui->clb_selectDataset->setChecked(false);
    ui->clb_defineCharact->setChecked(false);
    ui->clb_trainModel->setEnabled(true);
    ui->clb_trainModel->setChecked(true);
    ui->variableWidget->setEnabled(false);


    // -- removing and enabling uis
    fadeInVariableWidget(false);
    hideViews();
}

void MainWindow::cleanLayout(QLayout *variableLayout){
    QLayoutItem *wItem;

    while ((wItem = variableLayout->takeAt(0)) != NULL) {
        delete wItem->widget();
        delete wItem;
    }
}

void MainWindow::replaceVariableWidget(QWidget* _widget){
    QLayout *variableLayout = ui->variableWidget->layout();

    if (variableLayout != NULL){

        cleanLayout(variableLayout);
    }else {
        variableLayout = new QVBoxLayout(ui->variableWidget);
    }
    variableLayout->addWidget(_widget);
}

void MainWindow::on_clb_selectDataset_clicked(){
    if(_datasetView->isVisible()) return;

    if (_obtainedDataset != NULL){
        _datasetView->setDataset(_obtainedDataset);
    }

    hideViews();
    _datasetView->setVisible(true);

    if (ui->sidebarWidget->width() != ui->sidebarWidget->minimumWidth())
        resizeSidebar (ui->sidebarWidget->minimumWidth());
    else fadeInVariableWidget();

}

void MainWindow::on_clb_defineCharact_clicked(){
    if (_obtainedDataset != NULL) _featureView->setDataset(
                const_cast<const Dataset*>(_obtainedDataset));
    if (_featureView->isVisible()) return;
    hideViews();
    _featureView->setVisible(true);
    fadeInVariableWidget();
}

void MainWindow::on_clb_trainModel_clicked(){
    if (_modelTrainingView->isVisible()) return;
    if (_obtainedDataset == NULL ||
            _obtainedFeaturesConf == NULL){

        int ret = QMessageBox::warning(this, tr("Ooops!"),
                                       tr("Se requiere haber configurado un dataset y la definición de los features.\n"
                                          "Desea continuar?"),
                                       QMessageBox::Ok
                                       | QMessageBox::Cancel,
                                       QMessageBox::Ok);
        switch (ret) {
        case QMessageBox::Cancel:
            return;
            break;
        }
    }

    hideViews();

    if (_obtainedDataset != NULL)
        _modelTrainingView->setDataset(_obtainedDataset);
    if (_obtainedFeaturesConf != NULL)
        _modelTrainingView->setFeatureDef(_obtainedFeaturesConf);
    _modelTrainingView->setVisible(true);
    fadeInVariableWidget();
}

void MainWindow::on_clb_testModel_clicked(){
    if (_modelEvaluationView->isVisible()) return;
    if (_obtainedDataset == NULL ||
            _obtainedClassificationModel == NULL){

        int ret = QMessageBox::warning(this, tr("Ooops!"),
                                       tr("Se requiere haber configurado un dataset y un modelo cargado.\n"
                                          "Desea continuar?"),
                                       QMessageBox::Ok
                                       | QMessageBox::Cancel,
                                       QMessageBox::Ok);
        switch (ret) {
        case QMessageBox::Cancel:
            return;
            break;
        }
    }

    if (_obtainedDataset != NULL)
        _modelEvaluationView->setDataset(_obtainedDataset);
    if (_obtainedClassificationModel != NULL)
        _modelEvaluationView->setPredictiveModel(_obtainedClassificationModel);

    hideViews();
    _modelEvaluationView->setVisible(true);
    ui->variableWidget->setEnabled(true);

}

void MainWindow::onNotification(QString notification){
    statusBar()->showMessage(notification, 3000);
}

