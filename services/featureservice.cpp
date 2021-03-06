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


Matrix<float> *FeatureService::scaleMatrixByNumber(const Matrix<float> *matrix, double norm){

    double _norm = norm;
    double meassure;
    if(norm == 0){
        _norm = matrix->maxVal();
    }

    Matrix<float> *normalized = new Matrix<float>(matrix->rows(), matrix->cols());

    for (unsigned i = 0; i < matrix->rows(); ++i){
        for (unsigned j = 0; j < matrix->cols(); ++j){
            meassure = matrix->operator ()(i, j);
            normalized->setValue(i, j, meassure / _norm);
        }
    }
    normalized->setScaled(true);

    return normalized;
}

void FeatureService::saveCalculatedMatrix(Matrix<float> *matrix, QString fileName){
    QFile *file;
    QTextStream *inputStream;

    file = new QFile(fileName);
    if (file->open(QIODevice::WriteOnly)){

        inputStream = new QTextStream(file);
        *inputStream<< " ," ;
        for (unsigned j = 0; j < matrix->cols(); ++j){

            *inputStream<< "Col" << j;
            if (j < matrix->cols() - 1) *inputStream << ", ";
        }

        *inputStream << endl;

        for (unsigned i = 0; i < matrix->rows(); ++i){

            *inputStream<< "LFeature" << i << ", ";
            for (unsigned j = 0; j < matrix->cols(); ++j){
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

        QTextStream istream(&loadFile);
        QString line = istream.readLine();
        QStringList values = line.split(",");
        QString value;

        int nRows = 0;
        while (! istream.atEnd()){
            line = istream.readLine();
            ++nRows;
        }
        istream.seek(0);
        istream.readLine();

        unsigned cols = values.size() - 1;
        matrix = new Matrix<float> (nRows, cols);

        nRows = 0;
        while (! istream.atEnd()){
            line = istream.readLine();
            values = line.split(",");

            for (int i = 1; i < values.size(); ++i){
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

float FeatureService::euclideanDistance(float *feat1, float *feat2, unsigned featSize){
    float distance = 0;
    for (unsigned i = 0; i < featSize; ++i) {
        distance += pow((feat1[i] - feat2[i]), 2.0);
    }
    return sqrt(distance);
}

Matrix<float> *FeatureService::calculateLocalFeatures(FeatureDefinition *featureConfig, SCOPEntry *entry) {

    Matrix<float> *rawMetrics;
    Matrix<float> *tmpMetrics = NULL;
    Matrix<float> *auxRawMetrics = NULL;
    Matrix<float> *featureMatrix;
    Matrix<float> *filteredFeatMatrix;
    const FeatureDefinition::GeneratorMethod method = featureConfig->method();

    //Checks if I'm gonna generate a mixture of distances and angles or just a simple one.
    switch (method){
    case FeatureDefinition::Distance:
    case FeatureDefinition::Angle:
        rawMetrics = rawMetricsMatrix(method,
                                      entry,
                                      featureConfig->treshold());
        break;
    case FeatureDefinition::Mixture:
        //If is a mixed matrix both matrixs should be scaled to a factor of apperture and distance
        rawMetrics = rawMetricsMatrix(FeatureDefinition::Distance,
                                      entry,
                                      featureConfig->treshold());
        tmpMetrics = scaleMatrixByNumber(rawMetrics, featureConfig->treshold());
        delete rawMetrics;
        rawMetrics = NULL;
        rawMetrics = tmpMetrics;

        auxRawMetrics = NULL;
        auxRawMetrics = rawMetricsMatrix(FeatureDefinition::Angle,
                                         entry);
        tmpMetrics = scaleMatrixByNumber(auxRawMetrics);
        delete auxRawMetrics;
        auxRawMetrics = NULL;
        auxRawMetrics = tmpMetrics;
        tmpMetrics = NULL;

        break;
    default:
        qCritical() << "There is no known implementation of the selected "
                    << "generation method, current method is: " << method;
        return NULL;
    }



    unsigned featureSize = featureConfig->sqrtSize();
    unsigned computedFeatureSize = featureSize*featureSize;

    //In case is a mixture I have to make touples for the feature.
    //m(i,j) = if j is odd d(i, j) else a(i,j)
    if (method == FeatureDefinition::Mixture) computedFeatureSize *= 2;

    unsigned i_numFeatsxCol = rawMetrics->cols() - featureSize + 1;
    unsigned i_numFeatsxRow = rawMetrics->rows() - featureSize + 1;
    unsigned i_numFeats = i_numFeatsxCol * i_numFeatsxRow;

    unsigned idFeat = 0;
    unsigned idElement = 0;

    bool containsRelevantData = false;
    float* tmpFeature = NULL;
    float tmpValue;
    tmpFeature = new float[computedFeatureSize];

    featureMatrix = new Matrix<float> (i_numFeats, computedFeatureSize);
    unsigned auxId = 0;

    for (unsigned initRow = 0; initRow < i_numFeatsxRow; ++initRow){
        for (unsigned initCol = 0; initCol < i_numFeatsxCol; ++initCol){

            for (unsigned idRow = 0; idRow < featureSize; idRow++){
                for (unsigned idCol = 0; idCol < featureSize; idCol++){
                    tmpValue = rawMetrics->valueAt(idRow+initRow, idCol+initCol);
                    if(featureConfig->treshold() != 0 &&
                            tmpValue < featureConfig->treshold()){
                        containsRelevantData = true;
                    }
                    if (method == FeatureDefinition::Mixture){

                        auxId = idElement*2;
                        tmpFeature[auxId] = tmpValue;
                        tmpValue = auxRawMetrics->valueAt(idRow+initRow, idCol+initCol);
                        auxId = (idElement*2)+1;
                        tmpFeature[auxId] = tmpValue;

                    }else{

                        tmpFeature[idElement] = tmpValue;
                    }

                    ++idElement;
                }
            }

            //TODO: Take out the counter of idFeat and the featureMatrix assignation once the other part is defined
            if(containsRelevantData ||
                    featureConfig->treshold() == 0){
                for (unsigned i = 0; i < computedFeatureSize; ++i){
                    featureMatrix->setValue(idFeat, i, tmpFeature[i]);
                    tmpFeature[i] = 0;
                }
                ++idFeat;
                containsRelevantData = false;

            }

            idElement = 0;
        }
    }


    //TODO: Remove the filter if the previous part gets a good definition to avoid fragments without information
    if(featureConfig->treshold() != 0){
        filteredFeatMatrix = new Matrix<float>(idFeat, computedFeatureSize);
        for (unsigned i = 0; i < idFeat; ++i){
            for (unsigned j = 0; j < computedFeatureSize; ++j){
                filteredFeatMatrix->setValue(i, j, featureMatrix->valueAt(i, j));
            }
        }
        delete featureMatrix;
        featureMatrix = NULL;
        featureMatrix = filteredFeatMatrix;
    }

    delete tmpFeature;
    delete rawMetrics;
    if (auxRawMetrics != NULL) delete auxRawMetrics;

    return featureMatrix;
}


Matrix<float> *FeatureService::rawMetricsMatrix(
        const FeatureDefinition::GeneratorMethod method,
        SCOPEntry *entry,
        const double treshold) {

    Matrix<float> *rawMetrics = NULL;

    if (treshold != 0){
        rawMetrics = calculateMetricsMatrix(method,
                                            entry,
                                            treshold);
    } else {
        rawMetrics = calculateMetricsMatrix(method,
                                            entry);
    }

    return rawMetrics;
}

Matrix<float> *FeatureService::calculateMetricsMatrix(
        FeatureDefinition::GeneratorMethod method, SCOPEntry *entry, const double treshold){
    unsigned dim = entry->numResidues();

    Matrix<float> *metricsMatrix = new Matrix<float>(dim, dim);

    Residue *residueI, *residueJ;

    double meassure = 0;

    for (unsigned i = 0; i < dim; ++i){
        residueI = entry->residues().at(i);

        for (unsigned j = i; j < dim; ++j){

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

    for (unsigned i = 0; i < metric->rows(); i++){
        for (unsigned j = 0; j < newColSize; j++){
            distance = metric->operator ()(i, j);
            angle = auxMetric->operator ()(i, j);

            mixed->setValue(i, j * 2, distance);
            mixed->setValue(i, (j * 2) + 1, angle);
        }
    }


    qDebug() << "Just finished the mixing";
    return mixed;
}
