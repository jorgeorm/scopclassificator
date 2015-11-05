#include "featuredefinitionview.h"
#include "ui_featuredefinitionview.h"
#include <QDebug>
#include <services/datasetservice.h>

FeatureDefinition::GeneratorMethod FeatureDefinitionView::selectedMethod()
{
    FeatureDefinition::GeneratorMethod gen;

    int currentIndex = ui->featMethod_qcb->currentIndex();
    ui->taskProgress_qpb->setVisible(false);

    switch (currentIndex){
    case 0:
        gen = FeatureDefinition::Distance;
        break;
    case 1:
        gen = FeatureDefinition::Angle;
        break;
    case 2:
        gen = FeatureDefinition::Mixture;
        break;
    default:
        gen = FeatureDefinition::Undefined;
    }

    return gen;
}

void FeatureDefinitionView::syncFeatureDefinition()
{
    _featureDef->setMethod (selectedMethod ());
    _featureDef->setSqrtSize (ui->featureSize_qsb->value());
    if(_featureDef->method() == FeatureDefinition::Distance)
        _featureDef->setTreshold (ui->featLimit_qsb->value());

}

FeatureDefinitionView::FeatureDefinitionView(QWidget *parent) :
    QGroupBox(parent),
    ui(new Ui::FeatureDefinitionView),
    _controller(),
    _threadController() {

    _RInstance = NULL;

    ui->setupUi(this);
    this->setVisible(false);

    ui->showFeat_qpb->setEnabled(false);

    _featureDef = new FeatureDefinition;
    syncFeatureDefinition();

    _controller.moveToThread(&_threadController);
    connect(&_threadController, SIGNAL(started()),
            &_controller, SLOT(generateFeatureGraphic()));

    // Controller signals
    connect(&_controller, SIGNAL(featureGraphicsCompleted()),
            &_threadController, SLOT(quit()));
    connect(&_controller, SIGNAL(featureGraphicsCompleted()),
            this, SLOT(paintFeatureGraphics()));
    connect(&_controller, SIGNAL(notifyTask(QString)),
            this, SLOT(taskNotified(QString)));
    connect(&_controller, SIGNAL(notifyProgress(unsigned int)),
            this, SLOT(displayProgress(unsigned int)));

    //For graphics
    _svg = new QSvgWidget;
    _svg->setVisible(false);
    QLayout * svgFrameLayout = ui->sample_qfrm->layout();

    if (svgFrameLayout == NULL){
        svgFrameLayout = new QVBoxLayout(ui->sample_qfrm);
    }

    svgFrameLayout->addWidget(_svg);
}

FeatureDefinitionView::~FeatureDefinitionView()
{
    delete ui;
}

void FeatureDefinitionView::on_showFeat_qpb_clicked(){
    syncFeatureDefinition();
    _controller.setFeatureDefinition(_featureDef);
    _controller.setDataset(_dataset);

    ui->feature_qwgt->setEnabled(false);
    ui->actions_qwdg->setEnabled(false);

    _threadController.start();
}

void FeatureDefinitionView::on_finish_qpb_clicked(){
    syncFeatureDefinition();
    emit stepCompleted();
}

FeatureDefinition *FeatureDefinitionView::featuresDefinition() const
{
    return _featureDef;
}

void FeatureDefinitionView::setFeaturesDefinition(FeatureDefinition *featureDef)
{
    _featureDef = featureDef;
}

void FeatureDefinitionView::setRInstance( RInside *RInstance)
{
    _RInstance = RInstance;

    if(_RInstance != NULL){
        ui->showFeat_qpb->setEnabled(true);
    } else {
        ui->showFeat_qpb->setEnabled(false);
    }
}

void FeatureDefinitionView::on_featureSize_qsb_valueChanged(int newVal)
{
    _featureDef->setSqrtSize(newVal);
    ui->computedFeatSize_qle->setText(QString::number(newVal*newVal));
}

void FeatureDefinitionView::paintFeatureGraphics(){
    QString svg = _controller.svgFile();

    ui->feature_qwgt->setEnabled(true);
    ui->actions_qwdg->setEnabled(true);

    _svg->load(svg);
    if(! _svg->isVisible()) _svg->setVisible(true);

    _threadController.terminate();
}

void FeatureDefinitionView::taskNotified(QString task){
    emit notify(task);
}

void FeatureDefinitionView::displayProgress(unsigned int progress){
    if(! ui->taskProgress_qpb->isVisible())
        ui->taskProgress_qpb->setVisible(true);
    ui->taskProgress_qpb->setValue((int) progress);
}

void FeatureDefinitionView::setDataset(const Dataset *dataset)
{
    _dataset = dataset;
}

void FeatureDefinitionView::on_featMethod_qcb_activated(const QString &arg1){
    if (! arg1.contains("Distancia")){
        this->ui->featLimit_qsb->setVisible(false);
        this->ui->featLimit_qlbl->setVisible(false);
    } else {
        this->ui->featLimit_qlbl->setVisible(true);
        this->ui->featLimit_qsb->setVisible(true);
    }
}
