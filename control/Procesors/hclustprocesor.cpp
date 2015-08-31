#include "hclustprocesor.h"
#include <QDir>

QString HClustProcesor::qstr_pathDirTrabajo = QString( "%1/SCOPP_Files/hclust").arg( QDir::homePath() );
QString HClustProcesor::qstr_maquetaComando = "hclust_res <- %1(points=%2)";
QString HClustProcesor::qstr_resultadoClustering="";
QString HClustProcesor::qstr_nomVarMedoids = "";

//Constructores y Destructores
HClustProcesor::HClustProcesor(QObject *parent):
    FeaturesProcesor(parent){

    this->initAtributos();
}

void HClustProcesor::initAtributos(){
    //Para multiproceso
    i_numMaxHilos = QThread::idealThreadCount();
    i_numProcsCorriendo = 0;
    b_flagGPU=false;
}


HClustProcesor::~HClustProcesor(){

}

//Setters y Getters
void HClustProcesor::setFlagGPU(bool b_gpu){
    b_flagGPU = b_gpu;
}

void HClustProcesor::setMaxHilos(int i_maxHilos){
    i_numMaxHilos = i_maxHilos;
}


//Metodos core
void HClustProcesor::procesar(){

}

void HClustProcesor::calcularFootprints(){

}
