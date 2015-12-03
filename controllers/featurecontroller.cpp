#include "featurecontroller.h"
#include <QStringBuilder>

#include <services/datasetservice.h>
#include <services/featuregraphicsservice.h>
#include <services/featureservice.h>
#include <services/scopentryservice.h>

#include <QFile>

FeatureController::FeatureController(QObject *parent) : QObject(parent){
    _graphicSampleSize = 20;
    _datasetTobeProcessed = NULL;

    _rTempFile = "";
    _svgFile = "";

}

void FeatureController::generateFeatureGraphic(){
    DatasetService dsHandler;
    FeatureService dataGenerator;
    FeatureGraphicsService plotter;

    QStringList fileLists;
    QString pathEntFiels;

    if(_datasetTobeProcessed == NULL) return;

    if (_datasetTobeProcessed->hasSample()){
        pathEntFiels = _datasetTobeProcessed->pathSampledProteins();
    }else{
        pathEntFiels = _datasetTobeProcessed->pathProteins();
    }


    QList<SCOPEntry *> proteins;

    if(_datasetTobeProcessed->hasSample()){
        proteins = dsHandler.sampledProteins(*_datasetTobeProcessed);
    } else {
        proteins = dsHandler.proteins(*_datasetTobeProcessed);
    }

    Matrix <float> *data, *dataAux, *tmpMatrix;
    tmpMatrix = NULL;
    dataAux = NULL;
    SCOPEntry *prot;
    unsigned fileCounter;
    fileCounter = 0;

    QString dataTmpFile;
    FeatureDefinition::GeneratorMethod method = _featureDefinition->method();


    foreach (prot, proteins) {


        // Checks if I'm generating a mixed matrix

        switch (method) {
        case FeatureDefinition::Angle:
        case FeatureDefinition::Distance:
            data = dataGenerator.rawMetricsMatrix(method,
                                                  prot,
                                                  _featureDefinition->treshold());
            _svgFile = plotter.generateHeatmap(prot->sid(), data);
            break;
        case FeatureDefinition::Mixture:
            data = dataGenerator.rawMetricsMatrix(FeatureDefinition::Distance,
                                                  prot,
                                                  _featureDefinition->treshold());
            tmpMatrix = dataGenerator.scaleMatrixByNumber(data, _featureDefinition->treshold());
            delete data;
            data = tmpMatrix;

            dataAux = dataGenerator.rawMetricsMatrix(FeatureDefinition::Angle,
                                                     prot);
            tmpMatrix = dataGenerator.scaleMatrixByNumber(dataAux);
            delete dataAux;
            dataAux = tmpMatrix;

            tmpMatrix = NULL;
            _svgFile = plotter.generateMixedHeatmap(prot->sid(), data, dataAux);
            break;
        default:
            qCritical() <<"This generation method is not supported by graphics";
            return;
            break;
        }


        delete data;
        data = NULL;
        if (dataAux != NULL) delete dataAux;
        dataAux = NULL;
        emitProgress(++fileCounter,proteins.size());
    }
    qDeleteAll(proteins);



// With sampling
//QStringList filesToProcess = dsHandler.scanFolderForEntFiles(pathEntFiels);

//    QMap<QString, QString> filesToGraphic = dsHandler.getSampleFromFileList(filesToProcess, _graphicSampleSize);

//    qDebug() << "Sample size for graphics: " << _graphicSampleSize;
//    SCOPEntry * rawData;
//    unsigned fileCounter;
//    Matrix <float> *dataPreprocesed;

//    fileCounter = 0;
//    QString entryName;

//    foreach (entryName, filesToGraphic.keys()) {

////    TODO: Uncomment this when iterating
//        qDebug()  << "Gonna Load the ent file for: " << entryName;
//        rawData = dataLoader.loadEntFile (filesToGraphic.value (entryName));

////        rawData = dataLoader.loadEntFile (filesToGraphic.values().first());
//        qDebug()  << "Gonna generate the data for: " << entryName;

//        dataPreprocesed = dataGenerator.rawMetricsMatrix(
//                    _featureDefinition, rawData);

//        qDebug()  << "Gonna write svg for: " << entryName;

//        _svgFile = plotter.generateHeatmap(entryName, dataPreprocesed);

//        qDebug() << "File located in: " << _svgFile;
//        qDebug() << "Just deleted the file matrix";
//        qDebug() << "Counter: " << fileCounter;

//        fileCounter++;
//        emitProgress(fileCounter, _graphicSampleSize);

//        delete dataPreprocesed;
//        delete rawData;
//    }

    emit featureGraphicsCompleted();
}


void FeatureController::emitProgress(int counter, int total){
    double percentage = (double)counter / (double)total;
    percentage *= 100.0;

    emit notifyProgress((int) floor(percentage));
}

QString FeatureController::svgFile() const{
    return _svgFile;
}

void FeatureController::setDataset(const Dataset *datasetTobeProcessed){
    _datasetTobeProcessed = datasetTobeProcessed;
}

unsigned int FeatureController::graphicSampleSize() const{
    return _graphicSampleSize;
}

void FeatureController::setGraphicSampleSize(unsigned int graphicSampleSize){
    _graphicSampleSize = graphicSampleSize;
}

FeatureDefinition *FeatureController::featureDefinition() const{
    return _featureDefinition;
}

void FeatureController::setFeatureDefinition(FeatureDefinition *featureDefinition){
    _featureDefinition = featureDefinition;
}
