#include "FeaturesProcesor.h"
#include <QFile>
#include <QTextStream>
#include <utils/arrayutils.h>
#include "control/FootprintCalculator.h"
#include <QDir>
#include <QDateTime>
#include <fstream>

//Para gestión de configuración
#include "control/gestorconfiguracion.h" //Accede a los parametros de configuración
#include "modelo/configuracion.h" // Modelo de la configuración


FeaturesProcesor::FeaturesProcesor(QObject *parent):
    QObject(parent){

    pdbh_manejadorProts = new PDBHandler();

    initAtributos();

}

void FeaturesProcesor::initAtributos(){

    qstr_pathDirTrabajo = GestorConfiguracion::getInstancia().getConfig()->getQstr_pathTMP();
    //Valido que los directorios de trabajo y temporales existan
    QDir dirTrabajo(qstr_pathDirTrabajo);
    if(!dirTrabajo.exists()){
        dirTrabajo.mkpath(qstr_pathDirTrabajo);
    }

    QString pathTmp = QString("%1/tmp").arg(qstr_pathDirTrabajo);
    QDir tmpDir( pathTmp );
    if(!tmpDir.exists()){
        tmpDir.mkdir( pathTmp );
    }

    i_numClustersGeneral = 0;
    i_numFootprints = 0;
    i_numClustersProteina = 0;
    i_tamFeature = 0;
    i_tamSubmatriz = 0;
    d_medidaMax = 0;
    qstr_tipoMatriz = "";
    dmat_footprintsModelo = NULL;
    qmap_poolProteinas = NULL;
    i_numProcsSimult = 0;
    b_semaforoFootprint = false;
    dmat_caracteristicasComunes = NULL;


}

void FeaturesProcesor::leerArchivoCaracticasComunes(){

    ifstream ifst_archivoMedoides;
    ifst_archivoMedoides.open(qPrintable(qstr_pathArchivoMedoides));
    string bufferLinea;


    if(ifst_archivoMedoides.is_open()){

        int idRow = 0;
        int idCol = 0;

        //Borro e instancio la matriz de las caracterictisticas
        if(dmat_caracteristicasComunes != NULL){
            ArrayUtils::deleteMatriz(dmat_caracteristicasComunes, i_numClustersGeneral);
        }
        dmat_caracteristicasComunes = ArrayUtils::dp_initMatriz(i_numClustersGeneral, i_tamFeature);

        cout<<"Inicializó la matriz de caracteristicas comunes"<<endl;

        //Empiezo a leer el archivo de características comunes
        while(getline( ifst_archivoMedoides, bufferLinea ) ){
            QString parserLinea = QString::fromUtf8(bufferLinea.c_str());

            bool esNombreProt = parserLinea.contains(tr(":"));
            //Verifico si la linea es descripción de proteínas
            if( esNombreProt ){
                QString nombreProt = parserLinea.split(tr(":")).at(1);
                listaProteinasBase.push_back(nombreProt);
            }else{
                //Ingreso valores a la matriz de características communes
                //Siendo las filas el elemento representativo del cluster y las columnas sus variables
                QStringList rowMatriz = parserLinea.split(";");

                QStringList::iterator iteradorNumeros;

                double valTmp = 0;

                for(iteradorNumeros = rowMatriz.begin();
                    iteradorNumeros != rowMatriz.end();
                    ++iteradorNumeros){
                    valTmp = (*iteradorNumeros).toDouble();
                    dmat_caracteristicasComunes[idRow][idCol] = valTmp;
                    ++idCol;

                }

                idCol = 0;
                ++idRow;
            }

        }

//        cout<<"(II) FeaturesProcesor(leerArchivo..Comunes): Terminó de leer el archivo"<<endl;
//        cout<<"(II) FeaturesProcesor(leerArchivo..Comunes): Numero de proteínas base: ";
//        cout<<listaProteinasBase.size()<<endl;
//        cout<<"(II) FeaturesProcesor(leerArchivo..Comunes): Elemento en 0,0: ";
//        cout<<dmat_caracteristicasComunes[0][0]<<endl;
//        cout<<"(II) FeaturesProcesor(leerArchivo..Comunes): Elemento en "<< i_numClustersGeneral -1 <<","<<i_tamFeature -1 <<":";
//        cout<<dmat_caracteristicasComunes[i_numClustersGeneral-1][i_tamFeature-1]<<endl;

    }else {
        cout<<"(EE) FeaturesProcesor((leerArchivo..Comunes): No fue posible abrir archivo de características comunes en: ";
        cout<<qPrintable(qstr_pathDirTrabajo)<<endl;
    }
}

FeaturesProcesor::FeaturesProcesor(QObject *parent, int i_numFeatsDom_, int i_numFeatsClass_, double d_medidaMax_, QString str_tipoMat_):
    QObject(parent){
    i_numClustersProteina = i_numFeatsDom_;
    i_numClustersGeneral = i_numFeatsClass_;
    d_medidaMax= d_medidaMax_;
    qstr_tipoMatriz = str_tipoMat_;

    dmat_footprintsModelo = NULL;
    i_numFootprints = 0;

    pdbh_manejadorProts = new PDBHandler();
}


FeaturesProcesor::~FeaturesProcesor(){
    delete pdbh_manejadorProts;
}

//Slot publico
void FeaturesProcesor::errorSlot(QString qstr_error){
    cout<<"(EE) FeaturesProcesor: "<<qPrintable(qstr_error)<<endl;
    exit(EXIT_FAILURE);
}

//Setters
void FeaturesProcesor::setNumFeatsDom(int i_numFeatsDom_){
    this->i_numClustersProteina = i_numFeatsDom_;
}

void FeaturesProcesor::setMaxMed(double max){
    d_medidaMax = max;
}

void FeaturesProcesor::setNumFeatsClase(int i_numFeatsClass_){
    this->i_numClustersGeneral = i_numFeatsClass_;
}

void FeaturesProcesor::setSubMatSize(int i_smatSize_){
    this->i_tamSubmatriz = i_smatSize_;
    this->i_tamFeature = i_smatSize_*i_smatSize_;
}

//  Para cargar medoides
void FeaturesProcesor::setPathArchivoMedoides( QString pathMedoides ){
    qstr_pathArchivoMedoides = pathMedoides;
}

void FeaturesProcesor::setTipoMat(QString qstr_tipoMatriz){
    this->qstr_tipoMatriz = qstr_tipoMatriz;
}

void FeaturesProcesor::setPoolProteinas(QMap<QString, QString> * qmap_poolProteinas_){
    qmap_poolProteinas = qmap_poolProteinas_;
}

//void FeaturesProcesor::setFeaturesEncontrados(double *dptr_features, int i_numFeatsC){
//    dptr_featuresEncontrados = dptr_features;
//    i_numClustersGeneral = i_numFeatsC;
//}

//Getters
int FeaturesProcesor:: getNumFeatsDom(){
    return i_numClustersProteina;
}

double FeaturesProcesor::getMaxMed(){
    return d_medidaMax;
}

int FeaturesProcesor::getNumFeatsClase(){
    return i_numClustersGeneral;
}

int FeaturesProcesor::getFeatSize(){
    return i_tamFeature;
}

QString FeaturesProcesor::getTipoMat(){
    return qstr_tipoMatriz;
}

QMap<QString, QString> * FeaturesProcesor::getPoolProteinas(){
    return this->qmap_poolProteinas;
}

//double * FeaturesProcesor::getFeaturesEncontrados(){
//    return dptr_featuresEncontrados;
//}

double ** FeaturesProcesor::parsearMatricesComoArrays(double **matriz, int i_rows, int i_cols, int i_rowsSub, int i_colsSub){

    int i_tamFeat = i_rowsSub * i_colsSub;
    int i_numFeatsxCol = i_cols - i_colsSub + 1;
    int i_numFeatsxRow = i_rows - i_rowsSub + 1;
    int i_numFeats = i_numFeatsxCol * i_numFeatsxRow;

//    Init matriz que retornaré
    double ** dmat_subMats = new double * [i_numFeats];
    for(int idRow = 0; idRow < i_numFeats; ++idRow){

        dmat_subMats [ idRow ] = new double [i_tamFeat];

        for (int idCol = 0; idCol < i_tamFeat; idCol ++){
            dmat_subMats [idRow] [idCol] = 0;
        }

    }

    int idItem = 0;
    int idFeature = 0;

    for(int initRow = 0; initRow < i_numFeatsxRow; ++initRow){
        for(int initCol = 0; initCol < i_numFeatsxCol; ++initCol){

            for(int idRow = 0; idRow < i_rowsSub; idRow++){
                for(int idCol = 0; idCol < i_colsSub; idCol++){
                    dmat_subMats [ idFeature ] [ idItem ] = matriz[idRow+initRow][idCol+initCol];
                    idItem++;
                }
            }

            idItem = 0;
            ++idFeature;
        }
    }

    return dmat_subMats;
}

void FeaturesProcesor::calcularFootprintsModelo(){

    key_footprint = 0;

    cout<<"(II) FeaturesProcesor(calcularFootprintsModelo): Entró a calcular footprints"<<endl;
    leerArchivoCaracticasComunes();
    cout<<"(II) FeaturesProcesor(calcularFootprintsModelo): Leyo archivo de medoides"<<endl;

    itProteinasBase = listaProteinasBase.begin();

    //cout<<"(II) FeaturesProcesor(calcularFootprintsModelo): Va a comenzar iteración "<<endl;

    if(dmat_footprintsModelo != NULL){
        ArrayUtils::deleteMatriz(dmat_footprintsModelo, listaProteinasBase.size());
    }

    dmat_footprintsModelo = ArrayUtils::dp_initMatriz(listaProteinasBase.size(), i_numClustersGeneral);

    for(int idHilo = 0;
        idHilo < i_numMaxHilos
        && itProteinasBase != listaProteinasBase.end();
        ++idHilo){

        QThread * qthreadTmp = new QThread();
        threadList.push_back(qthreadTmp);
        asignarHiloFootprinter(idHilo);

    }

}

void FeaturesProcesor::salvarModelo(){

    //TODO: Terminar de salvar el modelo obtenido
    QDir qdir_workDir(qstr_pathDirTrabajo);

    bool b_dirExist = qdir_workDir.exists();

    if( !b_dirExist ){
        b_dirExist = qdir_workDir.mkpath( qstr_pathDirTrabajo );
    }

    if( b_dirExist ){
        QString qstr_pathArchivoModelo= tr("%1/modelo%2.csv").arg( qstr_pathDirTrabajo, QDateTime::currentDateTime().toString("dd-MM-yyyy-hh-mm") );
        ofstream flujoSalida;

        flujoSalida.open( qPrintable(qstr_pathArchivoModelo ) );

        if ( flujoSalida.is_open() ){

            for (int idModelo = 0; idModelo < listaProteinasBase.size(); ++idModelo) {

                flujoSalida<<qPrintable(listaProteinasBase[idModelo])<<":";

                for (int idFeature = 0; idFeature < i_numClustersGeneral; ++idFeature) {

                    flujoSalida<<dmat_footprintsModelo[idModelo][idFeature];

                    if( idFeature < i_numClustersGeneral -1 ){
                        flujoSalida<<";";
                    }
                }
                if(idModelo < listaProteinasBase.size() -1 ) {
                    endl(flujoSalida);
                }
            }

            flujoSalida.flush();
            flujoSalida.close();

            emit calculeFootprintsModelo( qstr_pathArchivoModelo );

        }else {
            cout<<"(EE) FeatureProcesor(salvarModelo): El archivo donde se guardan los modelos no se pudo abrir"<<endl;
        }
    } else {
        cout<<"(EE) FeatureProcesor(salvarModelo): No se pudo crear el foder de trabajo"<<endl;
    }
}

void FeaturesProcesor::liberarHiloFootprinter(int idHilo){
    cout<<"(II) FeatureProcesor (liberarHiloFootprinter) : El hilo "<<idHilo<<" terminó de calcular footprint"<<endl;
    QThread * hiloFinalizado = threadList[idHilo];
    if( hiloFinalizado->isRunning() ){
        hiloFinalizado->quit();
        while( !hiloFinalizado->isFinished() ){
            //Loop para garantizar que espera hasta que terimna
        }
    }

    delete hiloFinalizado;
    threadList[ idHilo ] = NULL;
    int i_keyHilo = idHilo;

    // Si ya acabó de procesar todas las proteínas con footprints
    if( --i_numProcsSimult == 0){
        salvarModelo();
    }else {// Verifique si cambió el número de hilos
        //Mientras haya proteínas por procesar y el número de procesos sea menor que el número máximo de hilos
        while( itProteinasBase != listaProteinasBase.end() && i_numProcsSimult < i_numMaxHilos){

            //Verifico si no conozco un hilo para reutilizar.
            if( i_keyHilo < 0 ){
                for (unsigned int idHiloTmp = 0; idHiloTmp < threadList.size(); ++idHiloTmp) {
                    if( threadList[idHiloTmp] == NULL ){
                        i_keyHilo = idHiloTmp;
                    }
                }

            }

            QThread * nuevoHilo = new QThread();
            //Si no encontré espacio en la lista de hilos
            if( i_keyHilo < 0 ){

                i_keyHilo = threadList.size();
                threadList.push_back( new QThread() );

            }else {
                threadList [ i_keyHilo ] = nuevoHilo;

            }

            asignarHiloFootprinter( i_keyHilo );

            i_keyHilo = -1;
        }

    }

}

void FeaturesProcesor::proteinaFootprinted(QString nomProt){
    cout<<"(II) FeatureProcesor(proteinaFootprinted): Finalicé "<<qPrintable(nomProt)<<endl;
    emit proceseProteina( nomProt );
}

void FeaturesProcesor::configurarFPC(QThread *pQThread, FootprintCalculator *pFootprinter){

    QString nomProt = *itProteinasBase;

    while ( !qmap_poolProteinas->contains( nomProt ) ){
        cout<<"(II) FeaturesProcesor( configurarFPC ): La proteína "<<qPrintable(nomProt)<<" no existe en el conjunto"<<endl;
        ++itProteinasBase;
        if( itProteinasBase == listaProteinasBase.end() ){
            return;
        }
        nomProt = *itProteinasBase;
    }

    cout<<"(II) FeaturesProcesor( configurarFPC ): Nombre de proteína a calcular perfil "<<qPrintable(nomProt)<<endl;

    double ** matrizProt = NULL;
    int tamMatrizProt = 0;
    //Identifico el tipo de matriz
    int i_tipoMat = 0;
    if(qstr_tipoMatriz.compare(tr("Distancias")) == 0){
        i_tipoMat = pdbh_manejadorProts->DIST;
    }else if(qstr_tipoMatriz.compare(tr("Angulos")) == 0){
        i_tipoMat = pdbh_manejadorProts->ANGS;
    }


    QString qstr_pathProteina = qmap_poolProteinas->value( nomProt );
    pdbh_manejadorProts->leerPDB( qPrintable( qstr_pathProteina ) );
    pdbh_manejadorProts->mapearaMatriz(i_tipoMat, d_medidaMax);
    matrizProt = pdbh_manejadorProts->getMatriz(i_tipoMat);
    tamMatrizProt = pdbh_manejadorProts->getTamMatriz();

//    cout<<"(II) FeaturesProcesor (configurarFPC): matriz[0][0] = "<<matrizProt[0][0]<<endl;

    //Seteo lo necesario para que funciione
    pFootprinter->setMatrizOriginal(matrizProt, tamMatrizProt, tamMatrizProt);
    pFootprinter->setTamSubmat( i_tamSubmatriz );
    pFootprinter->setFeaturesMedoides(dmat_caracteristicasComunes,i_numClustersGeneral, i_tamFeature);

    pFootprinter->setIdFootprint( key_footprint );
    pFootprinter->setFootprints( dmat_footprintsModelo );
    pFootprinter->setSemaforo(b_semaforoFootprint);
    pFootprinter->setNombreHilo(nomProt);


    // Configuiro el hIlo
    pFootprinter->moveToThread(pQThread);
    connect(
                pQThread,    SIGNAL( started() ),
                pFootprinter, SLOT( calcularFootprintMedoides() ) );
    connect(
                pQThread,    SIGNAL(finished() ),
                pFootprinter, SLOT( deleteLater() ));
    connect(
                pFootprinter, SIGNAL( footprinted ( int ) ),
                this,             SLOT( liberarHiloFootprinter(int) ));

    connect(
                pFootprinter, SIGNAL( proteinaFootprinted ( QString ) ),
                this,             SLOT( proteinaFootprinted(QString) ));
}

void FeaturesProcesor::asignarHiloFootprinter(int idHilo){
    QThread * qthreadTmp = threadList[idHilo];
    FootprintCalculator * fpc_footprinte = new FootprintCalculator();

    fpc_footprinte->setIdHilo( idHilo );
    configurarFPC(qthreadTmp, fpc_footprinte);

    //Si terminé de revisar el pool de proteínas no haga nada.
    if( itProteinasBase != listaProteinasBase.end() ){
        qthreadTmp->start();

        ++i_numProcsSimult;
        ++itProteinasBase;
        ++key_footprint;
    }else {
        cout<<"(II) FeaturesProcesor(asignarFootprinter): Terminé con la lista de proteínas base."<<endl;
    }


}

void FeaturesProcesor::normalizarFootprintsXDimension(){

    //Instancio normas
    double normas[i_numClustersGeneral];

    //Valor defecto normas - Calculo Normas
    for ( int idCol = 0; idCol < i_numClustersGeneral; ++idCol ){
        normas[idCol] = 0;

        for( int idRow = 0; idRow < listaProteinasBase.size(); ++idRow ){
            normas [ idCol ] += pow( dmat_footprintsModelo [ idRow ] [ idCol ], 2.0 );
        }

        normas [ idCol ] = sqrt( normas [ idCol ] );
    }

    cout<<"Calculó las normas"<<endl;

    // Aplico normas a features
    for( int idRow = 0; idRow < i_numFootprints; ++idRow ){
        for ( int idCol = 0; idCol < i_numClustersGeneral; ++idCol ){
            dmat_footprintsModelo [ idRow ] [ idCol ] = dmat_footprintsModelo [ idRow ] [ idCol ] / normas [ idCol ];
        }
    }
}

