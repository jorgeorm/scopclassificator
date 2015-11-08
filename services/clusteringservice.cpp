#include "clusteringservice.h"
#include "featureservice.h"
#include <QFile>
#include <QFileInfo>
#include <QStringBuilder>

ClusteringService::ClusteringService (QObject *parent):
QObject(parent) {
    _useEntry = false;
    _useFile = false;
    _useMatrix = false;
    _preAlgorithm = "";
    _algorithm = "";
    _postAlgorithm = "";
    _matrix = NULL;

}

ClusteringService::~ClusteringService(){
    if (_matrix != NULL) {
        delete _matrix;
        _matrix = NULL;
    }

}

QString ClusteringService::name() const{
    return _name;
}

void ClusteringService::setName(const QString &name){
    _name = name;
}

SCOPEntry *ClusteringService::entry() const{
    return _entry;
}

void ClusteringService::resetDataFlags()
{
    _useEntry = false;
    _useFile = false;
    _useMatrix = false;
}

void ClusteringService::setEntry(SCOPEntry *entry){
    resetDataFlags();
    if (entry != NULL) _useEntry = true;
    _entry = entry;
    _name = entry->sid();
}

Matrix<float> *ClusteringService::matrix() const{
    return _matrix;
}

void ClusteringService::setMatrix(Matrix<float> *matrix){
    resetDataFlags();
    if (matrix != NULL) {
        _matrix = matrix;
        _useMatrix = true;
    }
}

QString ClusteringService::preAlgorithm() const{
    return _preAlgorithm;
}

void ClusteringService::setPreAlgorithm(const QString &preAlgorithm){
    _preAlgorithm = preAlgorithm;
}

QString ClusteringService::postAlgorithm() const{
    return _postAlgorithm;
}

void ClusteringService::setPostAlgorithm(const QString &postAlgorithm){
    _postAlgorithm = postAlgorithm;
}

QString ClusteringService::algorithm() const{
    return _algorithm;
}

void ClusteringService::setAlgorithm(const QString &algorithm){
    _algorithm = algorithm;
}

void ClusteringService::findCommonFeatures(){
    FeatureService featureCalc;
    Matrix<float> *features;
    QString dataTmpFile;

    if (_useEntry || _useMatrix) {
        if (_useEntry) {

            features = featureCalc.localFeaturesMatrix(_featDef,_entry);
            _matrix = featureCalc.scaleMatrixByNumber(features);

            delete features;
            features = NULL;
        }

        dataTmpFile = "/tmp/matrix_" % _name % ".csv";
        featureCalc.saveCalculatedMatrix(_matrix, dataTmpFile);
    } else if (_useFile) {
        dataTmpFile = _pathDataFile;
    }

    QString rscriptTmp = parseRscript(dataTmpFile);

    QString rScript = "Rscript " %
            rscriptTmp %
            "> /dev/null";
    int res = system(rScript.toStdString().c_str());

    if (res != 0) {
        emit errorOnClustering(rscriptTmp);
    }

    emit commonFeaturesFound(_name);
}

QString ClusteringService::pathDataFile() const
{
    return _pathDataFile;
}

void ClusteringService::setPathDataFile(const QString &pathDataFile)
{
    _pathDataFile = pathDataFile;
    resetDataFlags();
    _useFile = true;
}

QString ClusteringService::parseRscript(QString dataPath){
    QString metric;

    switch (_featDef->method()){
    case FeatureDefinition::Angle:
        metric = "angl";
        break;
    case FeatureDefinition::Distance:
        metric = "dist";
        break;
    case FeatureDefinition::Mixture:
        metric = "mix";
        break;
    case FeatureDefinition::Undefined:
        qDebug() << "(EE) Feature definition no definida";
        exit(0);
        break;
    }

    QString rHead = "#! /usr/bin/Rscript --vanilla";

    QString scriptPath = "/tmp/clust_" % _name % "_" % metric % ".R";

    QFile rFile (scriptPath);
    if (rFile.open (QIODevice::WriteOnly)){

        QTextStream istream (&rFile);

        istream << rHead << endl;
        istream << "x = read.table (file = \"" % dataPath % "\", row.names = 1, header = TRUE, sep = \",\")" << endl;
        if (! _preAlgorithm.isEmpty ()) istream << _preAlgorithm << endl;
        if (! _algorithm.isEmpty ()) istream << _algorithm << endl;
        if (! _postAlgorithm.isEmpty ()) istream << _postAlgorithm << endl;
        rFile.close ();
        return scriptPath;
    }
    return "";
}

FeatureDefinition *ClusteringService::featDef() const
{
    return _featDef;
}

void ClusteringService::setFeatDef(FeatureDefinition *featDef)
{
    _featDef = featDef;
}

