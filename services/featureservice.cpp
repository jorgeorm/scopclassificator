﻿#include "featureservice.h"
#include "models/utils/matrix.h"

#include <QDateTime>
#include <QFile>
#include <QFileInfo>
#include <QStringBuilder>

FeatureService::FeatureService() {
}


////In case I need to add more ways to get the local features depending on the featuredefinition.
Matrix<float> *FeatureService::localFeaturesMatrix(FeatureDefinition *featureConfig, SCOPEntry *entry){
    return calculateLocalFeatures(featureConfig, entry);
}

Matrix<float> *FeatureService::normalizeMatrix(const Matrix<float> *matrix, double norm){

    double _norm = norm;
    double meassure;
    if(norm == 0){
        _norm = matrix->maxVal();
    }

    Matrix<float> *normalized = new Matrix<float>(matrix->rows(), matrix->cols());

    for(unsigned i = 0; i < matrix->rows(); ++i){
        for(unsigned j = 0; j < matrix->cols(); ++j){
            meassure = matrix->operator ()(i, j);
            normalized->setValue(i, j, meassure / _norm);
        }
    }

    return normalized;
}

void FeatureService::saveCalculatedMatrix(Matrix<float> *matrix, QString fileName){
    QFile *file;
    QTextStream *inputStream;

    file = new QFile(fileName);
    if (file->open(QIODevice::WriteOnly)){

        inputStream = new QTextStream(file);
        *inputStream<< " ," ;
        for(unsigned j = 0; j < matrix->cols(); ++j){

            *inputStream<< "Col" << j;
            if (j < matrix->cols() - 1) *inputStream << ", ";
        }

        *inputStream << endl;

        for(unsigned i = 0; i < matrix->rows(); ++i){

            *inputStream<< "LFeature" << i << ", ";
            for(unsigned j = 0; j < matrix->cols(); ++j){
                *inputStream<<matrix->operator ()(i, j);
                if (j < matrix->cols() - 1) *inputStream << ", ";
            }
            if (i < matrix->rows() - 1) *inputStream << endl;
        }

        file->close();
    }

    if (file != NULL) delete file;
    if (inputStream != NULL) delete inputStream;
}

Matrix<float> *FeatureService::loadCalculatedMatrix(QString filePath){
    Matrix<float> *matrix = NULL;

    QFile loadFile(filePath);

    if (loadFile.open(QIODevice::ReadOnly)){

        QTextStream dataStream(&loadFile);
        QTextStream istream(&loadFile);

        QString line = istream.readLine();
        QStringList values = line.split(",");
        QString value;

        int nRows = 0;

        dataStream.readLine();

        qDebug() << "Header Line: " << line;

        while (dataStream.atEnd()){
            ++nRows;
        }

        matrix = new Matrix<float> (nRows, values.size() - 1);

        nRows = 0;
        while (! istream.atEnd()){
            line = istream.readLine();
            values = line.split(",");

            for(int i = 1; values.size(); ++i){
                value = values.at(i);
                matrix->operator ()(nRows, i-1) = value.toFloat();
            }

            ++nRows;
        }

        loadFile.close();
    }

    return matrix;

}

QString FeatureService::concatenateFeatureFiles(const QStringList &files){
    int iFile, jLine;
    iFile = 0;
    jLine = 0;

    QDate myDate = QDate::currentDate();
    QString outputPath = "/tmp/features_" % myDate.toString("yyyy-MM-dd") % ".csv";
    QString line;
    QStringList values;
    QFile *featuresFile = NULL;
    QFileInfo *fInfo = NULL;
    QFile outputFile(outputPath);


    if (! outputFile.open(QIODevice::WriteOnly)) {
        return "";
    }

    QTextStream ostream(&outputFile);

    foreach (QString pathFile, files) {

        featuresFile  = new QFile(pathFile);
        fInfo = new QFileInfo(pathFile);

        if (fInfo->exists() && featuresFile->open(QIODevice::ReadOnly)){
            QTextStream istream(featuresFile);
            jLine = 0;
            while(! istream.atEnd()){
                line = istream.readLine();
                values = line.split(",");

                if (iFile > 0 && jLine == 0) {
                    line = istream.readLine();
                    values = line.split(",");
                }

                for (int iValue = 0; iValue < values.size(); ++iValue){
                    if (iValue == 0) {
                        QString medoid = values.at(iValue);
                        ostream << "\"" << fInfo->baseName() <<
                                   "_" <<
                                   medoid.remove(QString("\"")) << "\"";
                    }
                    else ostream << values.at(iValue);
                    if (iValue < values.size() -1) ostream <<", ";
                }
                ostream << endl;
                ++jLine;
            }

            featuresFile->close();
        }

        if (featuresFile != NULL) {
            delete featuresFile;
            featuresFile = NULL;
        }

        if (fInfo != NULL) {
            delete fInfo;
            fInfo = NULL;
        }

        ++iFile;
    }

    outputFile.close();

    return outputPath;
}

Matrix<float> *FeatureService::calculateLocalFeatures(FeatureDefinition *featureConfig, SCOPEntry *entry) {

    Matrix<float> *rawMetrics;
    Matrix<float> *featureMatrix;

    rawMetrics = rawMetricsMatrix(featureConfig, entry);

    unsigned featureSize = featureConfig->sqrtSize();
    unsigned computedFeatureSize = featureSize*featureSize;

    unsigned i_numFeatsxCol = rawMetrics->cols() - featureSize + 1;
    unsigned i_numFeatsxRow = rawMetrics->rows() - featureSize + 1;
    unsigned i_numFeats = i_numFeatsxCol * i_numFeatsxRow;

    unsigned idFeat = 0;
    unsigned idElement = 0;

    featureMatrix = new Matrix<float> (i_numFeats, computedFeatureSize);

    for(unsigned initRow = 0; initRow < i_numFeatsxRow; ++initRow){
        for(unsigned initCol = 0; initCol < i_numFeatsxCol; ++initCol){

            for(unsigned idRow = 0; idRow < featureSize; idRow++){
                for(unsigned idCol = 0; idCol < featureSize; idCol++){

                    featureMatrix->setValue(idFeat, idElement,
                            rawMetrics->operator ()(idRow+initRow, idCol+initCol));

                    ++idElement;
                }
            }

            idElement = 0;
            ++idFeat;
        }
    }

    delete rawMetrics;

    return featureMatrix;
}


Matrix<float> *FeatureService::rawMetricsMatrix(
        FeatureDefinition *featureConfig, SCOPEntry *entry) {

    Matrix<float> *rawMetrics = NULL;

    if(featureConfig->method() == FeatureDefinition::Mixture){
        qDebug() << " -- Gonna Calculate a mixted matrix";
        Matrix<float> *distances, *angles, *normalizedAngles, *normalizedDistances;

        if (featureConfig->treshold() != 0) {
            distances = calculateMetricsMatrix(
                    FeatureDefinition::Distance,
                    entry,
                    featureConfig->treshold());
        }
        else {
            distances = calculateMetricsMatrix(
                    FeatureDefinition::Distance,
                    entry);
        }

        angles = calculateMetricsMatrix(
                    FeatureDefinition::Angle, entry);

        normalizedAngles = this->normalizeMatrix(angles);
        normalizedDistances = this->normalizeMatrix(distances);

        rawMetrics = mixMetricMatrices(normalizedDistances,
                                       normalizedAngles);

        delete distances;
        delete angles;
        delete normalizedAngles;
        delete normalizedDistances;
        distances = NULL;
        angles = NULL;

    }else {

        if (featureConfig->treshold() != 0){

            rawMetrics = calculateMetricsMatrix(
                        featureConfig->method(),
                        entry,
                        featureConfig->treshold());
        } else {

            rawMetrics = calculateMetricsMatrix(
                        featureConfig->method(),
                        entry);
        }
    }

    return rawMetrics;
}

Matrix<float> *FeatureService::calculateMetricsMatrix(
        FeatureDefinition::GeneratorMethod method, SCOPEntry *entry, double treshold){
    unsigned dim = entry->numResidues();

    Matrix<float> *metricsMatrix = new Matrix<float>(dim, dim);

    Residue *residueI, *residueJ;

    double meassure = 0;

    for(unsigned i = 0; i < dim; ++i){
        residueI = entry->residues().at(i);

        for(unsigned j = i; j < dim; ++j){

            if (i == j) continue;

            residueJ = entry->residues().at(j);

            switch (method){
            case FeatureDefinition::Distance:
                meassure = *residueI - *residueJ;
                if(meassure > treshold) meassure = treshold;

                break;
            case FeatureDefinition::Angle:
                meassure = *residueI % *residueJ;
                break;
            default:
                meassure = -1.0;
            }


            metricsMatrix->setValue(i, j, meassure);
            metricsMatrix->setValue(j, i, meassure);

        }
    }

    return metricsMatrix;
}

Matrix<float> *FeatureService::mixMetricMatrices(Matrix<float> *metric, Matrix<float> *auxMetric){
    Matrix<float> *mixed;

    unsigned newColSize = metric->cols() * 2;
    double distance, angle;

    qDebug() << "Mixed rows: " << metric->rows() << ", Mixed COlumns: " << newColSize;

    mixed = new Matrix<float>(metric->rows(), newColSize);

    for(unsigned i = 0; i < metric->rows(); i++){
        for(unsigned j = 0; j < newColSize; j++){
            distance = metric->operator ()(i, j);
            angle = auxMetric->operator ()(i, j);

            mixed->setValue(i, j * 2, distance);
            mixed->setValue(i, (j * 2) + 1, angle);
        }
    }


    qDebug() << "Just finished the mixing";
    return mixed;
}