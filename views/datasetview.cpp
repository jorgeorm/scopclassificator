#include "datasetview.h"
#include "ui_datasetview.h"
#include <QDebug>
#include <QGraphicsOpacityEffect>
#include <QParallelAnimationGroup>
#include <QPropertyAnimation>
#include <QFileDialog> //To select files and folders
#include <QMessageBox> //To display errors
#include <models/dataset.h>
#include <views/viewModels/scopentrytablemodel.h>

DatasetView::DatasetView (QWidget *parent) :
    QGroupBox(parent),
    ui(new Ui::DatasetView),
    _controller(),
    _threadController()
{
    ui->setupUi(this);
    this->setVisible(false);

    _animationTime = 250;
    _defaultSearchPath = QDir::homePath();
    _datasetVisualRep = NULL;
    _dataset_p = NULL;


    connect(&_controller,
            SIGNAL(notifyTask(QString)),
            this,
            SLOT(taskNotified(QString)));
    connect(&_controller,
            SIGNAL(notifyProgress(int)),
            this,
            SLOT(progressNotified(int)));

    _controller.moveToThread(&_threadController);
    connect(&_controller, SIGNAL(datasetComplete(Dataset*)),
            this, SLOT(datasetGenerated(Dataset*)));

    //To be able to queue the Entrylist
    typedef QList<SCOPEntry *> SCOPEntryList;
    qRegisterMetaType< SCOPEntryList >( "SCOPEntryList" );
    connect(&_controller, SIGNAL(visualRepComplete(QList<SCOPEntry*>)),
            this, SLOT(viewableDatasetlGenerated(QList<SCOPEntry*>)));

    this->ui->progressWidget->setVisible(false);
    ui->qpb4_finishSelection->setEnabled(false);
}

DatasetView::~DatasetView ()
{
    delete ui;
}

//double DatasetView::opacity () const
//{
//    return _opacity;
//}

//void DatasetView::setOpacity (double opacity)
//{
//    _opacity = opacity;
//}

int DatasetView::animationTime () const
{
    return _animationTime;
}

void DatasetView::setAnimationTime (int animationTime)
{
    _animationTime = animationTime;
}

QString DatasetView::defaultSearchPath() const
{
    return _defaultSearchPath;
}

void DatasetView::setDefaultSearchPath(const QString &defaultSearchPath)
{
    _defaultSearchPath = defaultSearchPath;
}

Dataset *DatasetView::dataset() const
{
    return _dataset_p;
}

void DatasetView::setDataset(Dataset *dataset_p)
{
    //Loads dataset to class
    _dataset_p = dataset_p;

    //ui stuff
    if (_dataset_p->hasSample()){
        this->ui->qle2_pathFolder->setText(_dataset_p->pathSampledProteins());
    }else {
        this->ui->qle2_pathFolder->setText(_dataset_p->pathProteins());
    }

    this->ui->qle1_pathId->setText("Path a archivo ID");
    this->ui->qgb2_sample->setChecked(false);
    this->ui->progressWidget->setVisible(true);
    this->ui->qpb_progressBar->setValue(0);
    this->ui->actionsFrame->setEnabled(false);
    this->ui->formVerticalFrame->setEnabled(false);
    displayDatasetInformation();
}

void DatasetView::animatePathIdCheck(int _state){
    ui->qtb1_pathId->setEnabled (false);
    ui->qle1_pathId->setEnabled (false);
    ui->qtb1_pathId->setVisible (true);
    ui->qle1_pathId->setVisible (true);

    QAbstractAnimation::Direction direction;

    if (Qt::Checked == _state){
        direction = QAbstractAnimation::Backward;
    }else if (Qt::Unchecked == _state){
        direction = QAbstractAnimation::Forward;
    }else {
    }

    QPropertyAnimation *animQlePathId = new QPropertyAnimation (
                ui->qle1_pathId,"size");
    animQlePathId->setEasingCurve (QEasingCurve::InOutQuad);
    animQlePathId->setDuration (_animationTime);
    animQlePathId->setStartValue (
                QSize (ui->qle1_pathId->width (),
                       ui->qle2_pathFolder->height ()));
    animQlePathId->setEndValue (
                QSize (ui->qle1_pathId->width (), 1));

    QPropertyAnimation *animQtbPathId = new QPropertyAnimation (
                ui->qtb1_pathId, "size");
    animQtbPathId->setEasingCurve (QEasingCurve::InOutQuad);
    animQtbPathId->setDuration (_animationTime);
    animQtbPathId->setStartValue (
                QSize (ui->qtb1_pathId->width (),
                       ui->qtb2_pathFolder->height ()));
    animQtbPathId->setEndValue (
                QSize (ui->qtb1_pathId->width (), 1));

    QParallelAnimationGroup *animGroupPathId = new QParallelAnimationGroup;
    animGroupPathId->setDirection(direction);

    animGroupPathId->addAnimation (animQtbPathId);
    animGroupPathId->addAnimation (animQlePathId);

    connect(animGroupPathId,
            SIGNAL(finished ()),
            this,
            SLOT(onAnimationPathIdFinished ()));

    animGroupPathId->start(QPropertyAnimation::DeleteWhenStopped);
}

void DatasetView::on_qcb1_pathId_stateChanged(int _state){
    animatePathIdCheck(_state);

//    Método seguro
//    bool pathIdVisible = true;
//    if (Qt::Checked == _state){
//        pathIdVisible = true;
//    }else if (Qt::Unchecked == _state){
//        pathIdVisible = false;
//    }else {
//    }

//    ui->qtb1_pathId->setEnabled (pathIdVisible);
//    ui->qle1_pathId->setEnabled (pathIdVisible);
//    ui->qtb1_pathId->setVisible (pathIdVisible);
//    ui->qle1_pathId->setVisible (pathIdVisible);
}

void DatasetView::onAnimationPathIdFinished(){
    ui->qtb1_pathId->setEnabled (ui->qcb1_pathId->isChecked ());
    ui->qle1_pathId->setEnabled (ui->qcb1_pathId->isChecked ());
    ui->qtb1_pathId->setVisible (ui->qcb1_pathId->isChecked ());
    ui->qle1_pathId->setVisible (ui->qcb1_pathId->isChecked ());
}

void DatasetView::on_qtb1_pathId_clicked(){
    QString pathToFileId = "";

    pathToFileId = QFileDialog::getOpenFileName(this,
                                                tr("Abrir archivo ID filtrado"),
                                                _defaultSearchPath,
                                                tr("Archivos Id (*.id *.fa);;"
                                                   "Todos (*)"));

    if (! pathToFileId.isEmpty()){
        ui->qle1_pathId->setText(pathToFileId);
    }
}

void DatasetView::on_qtb2_pathFolder_clicked(){
    QString pathToFolderToScan = "";

    pathToFolderToScan = QFileDialog::getExistingDirectory (this,
                                                tr("Abrir archivo ID filtrado"),
                                                _defaultSearchPath);

    if (! pathToFolderToScan.isEmpty()){
        ui->qle2_pathFolder->setText(pathToFolderToScan);
    }
}

void DatasetView::on_qpb4_finishSelection_clicked(){
    emit eventNotified("Dataset listo");
    emit stepCompleted();
}

void DatasetView::on_qpb1_loadDataset_clicked(){
    QString pathEnts = ui->qle2_pathFolder->text();
    if( pathEnts.isEmpty() ||
            pathEnts == "Path a Folder con .ent") {

        QMessageBox::warning(this,
                             tr("Dataset"),
                             "Seleccionar al menos ubicación de archivos .ent"
                             );

        return;
    }

    if( _dataset_p != NULL ){
        delete _dataset_p;
        _dataset_p = NULL;
    }

    this->ui->progressWidget->setVisible(true);
    this->ui->qpb_progressBar->setValue(0);
    this->ui->actionsFrame->setEnabled(false);
    this->ui->formVerticalFrame->setEnabled(false);

    configControllerByUi();

    connect(&_threadController, SIGNAL(started()),
            &_controller, SLOT(createDataset()));

    _threadController.start();
    emit eventNotified("Generando Dataset");

}

void DatasetView::configControllerByUi(){

    _controller.setEntPath(ui->qle2_pathFolder->text());

    if(ui->qcb1_pathId->isChecked()){
        _controller.setIdPath(ui->qle1_pathId->text());
    }

    if(ui->qgb2_sample->isChecked()){
        _controller.setSampleSize(ui->qsb_size->value());
        if(ui->qgb1_filterSample->isChecked()){
            if(ui->qgb1_size->isChecked()) _controller.setFilterSize(ui->qsb_minProtSize->value(),
                                                                     ui->qsb_maxProtSize->value());
            if(ui->qgb2_scopClass->isChecked()) {
                SCOPEntry::SCOPCLASSLEVEL level;
                if(ui->class_check->isChecked()){
                    level = SCOPEntry::CLASS;
                } else if(ui->fold_check->isChecked()){
                    level = SCOPEntry::FOLD;
                } else if(ui->superFam_check->isChecked()){
                    level = SCOPEntry::SUPER_FAMILY;
                } else if(ui->family_check->isChecked()){
                    level = SCOPEntry::FAMILY;
                }
                _controller.setScopClassFilter(level);
            }
        }
    }

}

void DatasetView::removeDataFromTableView()
{
    if (_datasetVisualRep != NULL){
        _datasetVisualRep->clearData();
        delete _datasetVisualRep;
        _datasetVisualRep = NULL;
        ui->qtv_proteins->setModel(NULL);
        ui->qtv_proteins->reset();
    }

}

void DatasetView::displayDatasetInformation(){
    removeDataFromTableView();

    emit eventNotified("Mostrando dataset");
    this->ui->qpb_progressBar->setValue(0);

    _controller.setDatasetToProcess(_dataset_p);

    connect(&_threadController, SIGNAL(started()),
            &_controller, SLOT(loadVisualRepresentation()));

    _threadController.start();

}

void DatasetView::datasetGenerated(Dataset *dataset){
    ui->qpb4_finishSelection->setEnabled(true);
    _threadController.quit();
    disconnect(&_threadController, SIGNAL(started()),
            &_controller, SLOT(createDataset()));

    emit eventNotified("Dataset obtenido");
    this->ui->qpb_progressBar->setValue(100);

    _dataset_p = dataset;
    displayDatasetInformation ();
}

void DatasetView::viewableDatasetlGenerated(QList<SCOPEntry *> visualRep){

    _threadController.quit();
    disconnect(&_threadController, SIGNAL(started()),
            &_controller, SLOT(loadVisualRepresentation()));

    emit eventNotified("Información de dataset obtenida");
    this->ui->qpb_progressBar->setValue(100);

    this->ui->actionsFrame->setEnabled(true);
    this->ui->formVerticalFrame->setEnabled(true);


    _datasetVisualRep = new SCOPEntryTableModel(0);

    _datasetVisualRep->setData (visualRep);

    ui->qtv_proteins->setModel(_datasetVisualRep);
}

void DatasetView::taskNotified(QString taskName){
    emit eventNotified( taskName );
}

void DatasetView::progressNotified(int progress){
    this->ui->qpb_progressBar->setValue(progress);
}

void DatasetView::on_qpb3_resetDataset_clicked(){
    ui->qpb_progressBar->setValue(0);
    this->removeDataFromTableView();

    if(_dataset_p != NULL){
        delete _dataset_p;
        _dataset_p = NULL;
    }

    emit eventNotified("Dataset reseteado");
    emit datasetDestroyed();
}

