#include "datasetcontroller.h"

#include <models/dataset.h>
#include <QDebug>

DatasetController::DatasetController(QObject *parent) : QObject(parent),
_service(){
    _datasetToProcess = NULL;

    _pathToEnts = "";
    _pathToId = "";

    _generateSample = false;
    _sampleSize = 0;

    _sizeFiltered = false;
    _minProtSize = 0;
    _maxProtSize = 0;

    _runByScopClass = false;
    _classLevel = SCOPEntry::FAMILY;

    connect(&_service, SIGNAL(notifyTask(QString)),
            this, SLOT(serviceTasksReported(QString)));
    connect(&_service, SIGNAL(notifyProgress(int)),
            this, SLOT(serviceProgresReported(int)));
}


void DatasetController::setDatasetToProcess(const Dataset *value)
{
    _datasetToProcess = value;
}

void DatasetController::setEntPath(QString path){
    _pathToEnts = path;
}

void DatasetController::setIdPath(QString path){
    _pathToId = path;
}

void DatasetController::setSampleSize(unsigned size){

    if(0 < size) {
        _sampleSize = size;
        _generateSample = true;
    } else {
        _sampleSize = 0;
        _generateSample = false;
    }

}

void DatasetController::setFilterSize(unsigned min, unsigned max){
    _minProtSize = min;
    _maxProtSize = max;
    _sizeFiltered = true;
}

void DatasetController::setScopClassFilter(SCOPEntry::SCOPCLASSLEVEL scopLevel){
    _classLevel = scopLevel;
    _runByScopClass = true;
}

QList <SCOPEntry *> DatasetController::getVisualRepresentation(
        const Dataset * dataset){
    if( dataset->hasSample() ){
        return _service.sampledProteins(*dataset);
    }else {
        return _service.proteins(*dataset);
    }
}

void DatasetController::createDataset()
{
    Dataset * generated = new Dataset;

    if(_pathToId.isEmpty()) {
        _service.loadDirectoryIntoDataset(generated, _pathToEnts);
    }else {
        _service.loadAstralFileIntoDataset(generated, _pathToId, _pathToEnts);
    }

    if(_generateSample) generateSampleFromDs(generated);

    emit datasetComplete(generated);
}

void DatasetController::loadVisualRepresentation(){
    QList<SCOPEntry*> listaProts = this->getVisualRepresentation(_datasetToProcess);
    emit visualRepComplete(listaProts);
}

void DatasetController::serviceProgresReported(int progress){
    emit notifyProgress(progress);
}

void DatasetController::serviceTasksReported(QString taskName){
    emit notifyTask(taskName);
}

void DatasetController::generateSampleFromDs(Dataset *ds) {
    if(_runByScopClass || _sizeFiltered){
        if(_sizeFiltered) _service.setSizeFilter(_minProtSize, _maxProtSize);
        if(_runByScopClass) _service.setClassFilter(_classLevel);
        _service.sampleByCriteria(ds, _sampleSize);
    } else {
        _service.sampleDataset(ds, _sampleSize);
    }
}
