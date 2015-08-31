#include "RParser.h"
#include <QDir>
#include <stdio.h>
#include <sstream>
#include <string>
#include <fstream>
#include <QTextStream>
#include "control/gestorconfiguracion.h"

//#include <modelo/FeaturesDataset.h>

const std::string RParser::fileDataset= "datasetTmp.csv";

/**
 * @brief featureCollection
 */
const std::string RParser::fileFeatures = "subMatrices.ptmp";


RParser::RParser()
{
}

//TODO: modificar para proteínas gigantes.
void RParser::parsearMatrizClienteR ( double ** matriz, int i_rows, int i_cols, int i_rowsSub,
                                      int i_colsSub,  RServClient * rscl_entorno, QString nomVar){
    QString tempPathDir = GestorConfiguracion::getInstancia().getConfig()->getQstr_pathTMP();
    QString archivoURL = "%1/%2.csv";
    archivoURL = archivoURL.arg(tempPathDir, nomVar);
    QFile archivoTmp(archivoURL);

    int i_tamFeat = i_rowsSub * i_colsSub;
    int i_numFeatsxCol = i_cols - i_colsSub + 1;
    int i_numFeatsxRow = i_rows - i_rowsSub + 1;
    int i_numFeats = i_numFeatsxCol * i_numFeatsxRow;
    int i_numElemEscrito = 0;
    int i_numFeatureEscrito = 0;

    if(archivoTmp.open(QFile::WriteOnly)){
        QTextStream qts_salidaTmp(&archivoTmp);

        for(int initRow = 0; initRow < i_numFeatsxRow; ++initRow){
            for(int initCol = 0; initCol < i_numFeatsxCol; ++initCol){

                for(int idRow = 0; idRow < i_rowsSub; idRow++){
                    for(int idCol = 0; idCol < i_colsSub; idCol++){
//                        darr_datasetArray[idItem] = matriz[idRow+initRow][idCol+initCol];
                        qts_salidaTmp<<matriz[idRow+initRow][idCol+initCol];
                        ++i_numElemEscrito;
                        if(i_numElemEscrito < i_tamFeat){
                            qts_salidaTmp<<";";
                        }
                    }
                }
                i_numElemEscrito = 0;
                ++i_numFeatureEscrito;
                if(i_numFeatureEscrito < i_numFeats){
                    qts_salidaTmp<<"\n";
                }

            }
        }

        qts_salidaTmp.flush();
        archivoTmp.flush();

        archivoTmp.close();

    }else {
        cerr<<"(EE) RParser(parsear...ClienteR): No se pudo crear archivo"<<endl;
    }

    QString cargaDset;

    if(i_rows >= 1200){
        cargaDset = QString("%1 <- read.csv.ffdf( file=\"%2\", sep=\";\", dec=\".\", header=FALSE);").arg(nomVar,archivoURL);

    }else{
        cargaDset = QString("%1 <- read.csv( file=\"%2\", sep=\";\", dec=\".\", header=FALSE);").arg(nomVar,archivoURL);

    }

    rscl_entorno->ejecutarComandoSimple( cargaDset );//< Borro el array ingresado para no ocupar memoria adicional

    if( remove( qPrintable(archivoURL) ) != 0 ){
        perror( "(EE) Rparser(RServClient): Error borrando archivo temporal." );
    }else{
        cout<<"(II) Rparser(RServClient): Archivo temporal borrado. "<<qPrintable(archivoURL)<<endl;
    }

}

char* RParser::parsearMatrices(double **matriz, int tamMat, int tamSmat, RInside &instanciaR){
    int tamFeature = tamSmat*tamSmat;
    int numFeatxDim = tamMat - tamSmat +1;
    int numFeatures = numFeatxDim*numFeatxDim;
    char * nomMat = "features";
//    QString pathTmp = QDir::tempPath();
//    FILE * dataset;
//    FILE *features;
//    QString pathDataset = QString("%1/%2").arg(pathTmp,QString(fileDataset.c_str()));
//    QString pathFeatures = QString("%1/%2").arg(pathTmp,QString(fileFeatures.c_str()));
//    dataset = fopen(pathDataset.toStdString().c_str(), "w");
//    features = fopen(pathFeatures.toStdString().c_str(), "w");
//    ofstream fstr_dataset(pathDataset.toStdString().c_str());

    cout<<"Voy a instanciar la matriz de R con: "<<numFeatures<<", "<<tamFeature<<endl;
//    NumericMatrix datasetFeatures(numFeatures, tamFeature);
//    cout<<"Instancié la matriz numérica de R"<<endl;
    int idItem = 0;
    int idSubMat = 0;
//    int lastFeature = numFeatures -1;
    NumericMatrix matDataset(numFeatures, tamFeature);
//    stringstream bufferLinea;

    //itero por cada uno de los features
    for(int initRow = 0; initRow < numFeatxDim; ++initRow){
        for(int initCol = 0; initCol < numFeatxDim; ++initCol){
            // copio el feature como una submatriz
//            fprintf(features, "%i\n",idSubMat);
//            idItem = 0;
//            bufferLinea.str("");

            for(int idRow = 0; idRow < tamSmat; idRow++){
                for(int idCol = 0; idCol < tamSmat; idCol++){
//                    fprintf(dataset, "%2.4f",matriz[idRow+initRow][idCol+initCol]);

//                    if(idItem == lastFeature){
//                        fprintf(dataset, "\n");
//                    } else {
//                        fprintf(dataset, ";");
//                    }
                    matDataset[idItem] = matriz[idRow+initRow][idCol+initCol];
                    idItem++;
                }
//                fprintf(features, "\n");
            }
//            fstr_dataset<<bufferLinea.str();
//            cout<<"\tCopié la submatriz "<<idSubMat<<": ";
//            cout<<"en "<<qPrintable(pathDataset)<<" y "<<qPrintable(pathFeatures)<<endl;
            idSubMat++;
        }
    }
//    fclose(dataset);
//    fclose(features);
//    fstr_dataset.close();
    instanciaR[nomMat] = matDataset;

//    cout<<"Escribí matrices a "<<qPrintable(pathDataset)<<" y "<<qPrintable(pathFeatures)<<endl;
    return nomMat;
}
