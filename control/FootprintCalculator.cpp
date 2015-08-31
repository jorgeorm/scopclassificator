#include "FootprintCalculator.h"

#include <utils/arrayutils.h>//Para crear y borrar punteros de arrays o matrices
#include <stdio.h>
#include <math.h>
#include <float.h>


using namespace std;

FootprintCalculator::FootprintCalculator(QObject *parent) :
    QObject(parent){
    i_tipoTecnica = -1;
    dmat_features_medoides = NULL;
    dmat_footprints_output = NULL;
    bptr_salidaOcupada = NULL;
    i_numFeatures = 0;
    i_numMedoides = 0;
    i_idFootprint = -1;
    dmat_matrizProteina = NULL;

    idHiloFoot = -1;
}


void FootprintCalculator::borrarMedoides(){
    ArrayUtils::deleteMatriz(dmat_features_medoides, i_numMedoides);
    dmat_features_medoides = NULL;
    i_numMedoides = 0;
}

FootprintCalculator::~FootprintCalculator(){
    if(i_tamMatrizProt != 0){

        ArrayUtils::deleteMatriz(dmat_matrizProteina, i_tamMatrizProt);
        i_tamMatrizProt = 0;
        dmat_matrizProteina = NULL;
    }

}

//Seters
void FootprintCalculator::setFeaturesMedoides(double **dmat_matrizMedoides,
                                              int numRows, int numCols){
    this->dmat_features_medoides = dmat_matrizMedoides;
    this->i_numMedoides = numRows;
    this->i_numFeatures = numCols;
}

void FootprintCalculator::setMatrizOriginal(double ** dmat_matrizProt, int numRow, int numCols){

    if(this->dmat_matrizProteina== NULL){
        this->dmat_matrizProteina = ArrayUtils::dp_initMatriz(numRow, numCols);
    }else{
        ArrayUtils::deleteMatriz(this->dmat_matrizProteina, numRow);
        this->dmat_matrizProteina = NULL;
        this->dmat_matrizProteina = ArrayUtils::dp_initMatriz(numRow, numCols);
    }

    //Copia profunda de la matriz
    for(int idRow = 0; idRow < numRow; ++idRow){
        for(int idCol = 0; idCol < numCols; ++idCol){
            this->dmat_matrizProteina[idRow][idCol] = dmat_matrizProt[idRow][idCol];
        }
    }

    this->i_tamMatrizProt = numRow;
}

void FootprintCalculator::setTamSubmat(int tamSmat){
    i_tamSubmatFeature = tamSmat;
}

void FootprintCalculator::setFootprints(double **dmat_footprints){
    this->dmat_footprints_output = dmat_footprints;
}

void FootprintCalculator::setIdFootprint(int idFootprint){
    this->i_idFootprint = idFootprint;
}


//Geters
int FootprintCalculator::getIdFootprint(){
    return i_idFootprint;
}

int FootprintCalculator::getNumFeatures(){
    return i_numFeatures;
}

int FootprintCalculator::getNumMedoides(){
    return i_numMedoides;
}

int FootprintCalculator::getTipoTecnica(){
    return i_tipoTecnica;
}

int FootprintCalculator::getNumSubmatrices(){
    return i_tamMatrizProt;
}

void FootprintCalculator::setNombreHilo(QString nomHilo){
    this->qstr_nomHilo = nomHilo;
}

//slot público
void FootprintCalculator::calcularFootprintMedoides(){

    double * footprintTmp = ArrayUtils::dp_initArray(i_numMedoides);

    int i_numFeatsxCol = i_tamMatrizProt - i_tamSubmatFeature+ 1;
    int i_numFeatsxRow = i_tamMatrizProt - i_tamSubmatFeature + 1;
    int i_tamFeat = i_tamSubmatFeature * i_tamSubmatFeature;

    double * featureTmp = new double [ i_tamFeat ] ;
    int idFeatElement = 0;
    int contFeature = 0;

    //Itero sobre las posibles submatrices
    for(int initRow = 0; initRow < i_numFeatsxRow; ++initRow){
        for(int initCol = 0; initCol < i_numFeatsxCol; ++initCol){

             idFeatElement = 0;
            //Extraigo submatriz
            for(int idRow = 0; idRow < i_tamSubmatFeature; idRow++){
                for(int idCol = 0; idCol < i_tamSubmatFeature; idCol++){
//                        darr_datasetArray[idItem] = matriz[idRow+initRow][idCol+initCol];
                    featureTmp[idFeatElement] =  dmat_matrizProteina[idRow+initRow][idCol+initCol];;
                    ++idFeatElement;

                }
            }

            ++contFeature;
//            std::cout<<"(II) FootprintCalculator["<<qPrintable(qstr_nomHilo);
//            std::cout<<"](calcularFootprintMedoides): Obtuvo feature "<<contFeature<<std::endl;

            double d_distEuc = 0;
            double d_minDistEuc = DBL_MAX;
            int i_idFeatCercano = -1;

            //Comparo la submatriz con los features comunes.
            for( int idFeatComun = 0; idFeatComun <  i_numMedoides; ++idFeatComun){

                //Calculo distancia euclidiana
                d_distEuc = 0;

                for( int idCol = 0; idCol <  i_numFeatures; ++idCol){
                    //Cuadrado de diferencia entre puntos
                    d_distEuc += pow( ( featureTmp [idCol] - dmat_features_medoides [ idFeatComun ][ idCol ] ), 2.0);
                }
                //Raiz de sumatoria de cuadrados de diferencias
                d_distEuc = sqrt(d_distEuc);

                //Almaceno la mínima distancia y el feature que la registró
                if(d_distEuc < d_minDistEuc){
                    d_minDistEuc = d_distEuc;
                    i_idFeatCercano = idFeatComun;
                }
            }



//            std::cout<<"(II) FootprintCalculator["<<qPrintable(qstr_nomHilo);
//            std::cout<<"](calcularFootprintMedoides): Asigné feature "<<contFeature<<"."<<std::endl;
            //Cuento el feature encontrado
            if(i_idFeatCercano > -1 && i_idFeatCercano < i_numMedoides){
                footprintTmp [ i_idFeatCercano ] ++;
            }else {
                std::cout<<"(EE) FootrpintCalculator (calcularFootprintMedoides): Valor no valido para el idFeatComun: "<<i_idFeatCercano<<std::endl;
            }

        }
    }

    delete [] featureTmp;

    //Verifico que la salida esté libre
    while(*bptr_salidaOcupada){
        std::cout<<"(II) FootprintCalculator["<<qPrintable(qstr_nomHilo);
        std::cout<<"](calcularFootprintMedoides): Sleep semaforo "<<qPrintable(qstr_nomHilo)<<std::endl;
        sleep(0.25);
    }
    //Salvo el footprint
    *bptr_salidaOcupada = true;
    std::cout<<"(II) FootprintCalculator["<<qPrintable(qstr_nomHilo);
    std::cout<<"](calcularFootprintMedoides): Seteó el semaforo a true"<<std::endl;

    for (int idMedoid = 0; idMedoid < i_numMedoides; idMedoid++){
//        cout<<"(II) Footprint("<<qPrintable(qstr_nomHilo);
//        cout<<")["<<idMedoid<<"]: "<<footprintTmp[idMedoid]<<endl;
        dmat_footprints_output[i_idFootprint][idMedoid] = footprintTmp[idMedoid];
    }

    *bptr_salidaOcupada = false;
    std::cout<<"(II) FootprintCalculator["<<qPrintable(qstr_nomHilo);
    std::cout<<"](calcularFootprintMedoides): Seteó el semaforo a false"<<std::endl;

    delete [] footprintTmp;
    footprintTmp = NULL;

    emit proteinaFootprinted( qstr_nomHilo );
    emit footprinted( idHiloFoot );

}

void FootprintCalculator::setSemaforo(bool &flagSemaforo){
    bptr_salidaOcupada = &flagSemaforo;
}

void FootprintCalculator::setIdHilo(int idHilo){
    idHiloFoot = idHilo;
}
