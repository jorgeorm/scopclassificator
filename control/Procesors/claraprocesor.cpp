#include "claraprocesor.h"
#include <QDir>
#include "control/RParser.h"
#include <QTextStream>
#include <QDate>

#include <fstream> //Para lectura y escritura de archivos
#include <iostream> //Para lectura y escritura de archivos
#include "utils/archivosutils.h" //Para copia y demás de archivos
#include <control/FootprintCalculator.h>
#include <control/gestorconfiguracion.h>

//Instancio propiedad estática
QString ClaraProcesor::qstr_maquetaComando = "clara_res <- clara(x=%1, k=%2, metric =%3 , stand = %4, samples = %5, sampsize = %6, medoids.x = %7, keep.data = %8, rngR = %9, pamLike = %10)";
QString ClaraProcesor::qstr_resultadoClustering = "clara_med <- clara_res$medoids";
QString ClaraProcesor::qstr_nomVarMedoids = "clara_med";

//Cuando no tenía gestión de la configuración
//QString FeaturesProcesor::qstr_pathDirTrabajo = QString( "%1/SCOPP_Files/clara").arg( QDir::homePath() );

//Constructor / Destructor
ClaraProcesor::ClaraProcesor(QObject *parent):
    FeaturesProcesor(parent){

    Q_INIT_RESOURCE(rScripts);

    this->initAtributos();

//    QString qstr_pathArchivoFeats = QString( "%1/features.csv.tmp" ).arg( QDir::tempPath() );
    qstr_pathFeatsComunesProteinas = QString("%1/featuresClaraComunesProt.tmp.csv").arg(qstr_pathDirTrabajo);

    QString pathTmp = QString("%1/tmp/clara").arg(qstr_pathDirTrabajo);
    QDir tmpDir( pathTmp );
    if(!tmpDir.exists()){
        tmpDir.mkdir( pathTmp );
    }

//    tmfl_dsetFeatures = new QTemporaryFile( qstr_pathArchivoFeats );
//    tmfl_dsetFeatures->setAutoRemove( true );

}

void ClaraProcesor::initAtributos(){

    b_flagFootsOcup = false;

    //Config CLARA
    i_tamanoMuestra = 0;
    i_numMuestras = 0;
    qstr_tipoMetrica = "";
    b_datosEstandarizados = false;
    b_copiaMedoides = true;
    b_copiaDatos = true;
    b_generadorAleatR = false;
    b_intercambioPam = false;

    //qstr_nomBigProt = "";

    //Archivos
//    tmfl_dsetFeatures = NULL;
//    tmfl_finalFeatures = NULL;

    //Proteínas gigantes
    b_flagProcesoGigante = false;

    //Paralelo
    i_numFeatsComunes = 0;

}

ClaraProcesor::~ClaraProcesor(){

    if( !qtmt_hilosClustering.empty() ){
        QThread * pqt_hiloTmp = NULL;

        for( int idThread = 0; idThread < qtmt_hilosClustering.size(); ++idThread ){
            pqt_hiloTmp  = qtmt_hilosClustering[ idThread ];
            if(pqt_hiloTmp->isRunning()){
                pqt_hiloTmp->quit();
                pqt_hiloTmp->wait();
            }
        }
    }

    qtmt_hilosClustering.clear();

    Q_CLEANUP_RESOURCE(rScripts);


    //borro archivos temporales
//    if( tmfl_dsetFeatures != NULL ){
//        tmfl_dsetFeatures->remove();
//        tmfl_dsetFeatures->deleteLater();
//    }

//    while( !qltfl_listaArchivosTmp.isEmpty() ){
//        QTemporaryFile * qtf_archivoTmp = qltfl_listaArchivosTmp.takeFirst();
//        qtf_archivoTmp->remove();
//        qtf_archivoTmp->deleteLater();
//    }

    qsl_listaArchivosTmp.clear();

}

// Setters y Getters
// Setters

void ClaraProcesor::setNumHilos(int numHilos){
    i_numMaxHilos = numHilos;
    cout<<"(II) ClaraProcesor( setNumHilos ): Se cambió el número de hilos simultaneos: "<<numHilos<<endl;
}

void ClaraProcesor::setTamMuestra( int tamMuestra ){
    i_tamanoMuestra =  tamMuestra;
}

void ClaraProcesor::setNumMuestras( int numMuestras ){
    i_numMuestras = numMuestras;
}

void ClaraProcesor::setMetrica(QString tipoDistancia)
{
    qstr_tipoMetrica = tipoDistancia;
}

void ClaraProcesor::setFlagDatEstand(bool flag)
{
    b_datosEstandarizados = flag;
}

void ClaraProcesor::setFlagCopiMedoi(bool flag)
{
    b_copiaMedoides = flag;
}

void ClaraProcesor::setFlagCopiDatos(bool flag)
{
    b_copiaDatos = flag;
}

void ClaraProcesor::setFlagRGenerad(bool flag)
{
    b_generadorAleatR = flag;
}

void ClaraProcesor::setFlagInterPam(bool flag)
{
    b_intercambioPam = flag;
}



// Getters
//      Config Clara
int ClaraProcesor::getNumMuestras(){
    return i_numMuestras;
}

QString ClaraProcesor::getMetrica()
{
    return qstr_tipoMetrica;
}

bool ClaraProcesor::getFlagDatEstand()
{
    return b_datosEstandarizados;
}

bool ClaraProcesor::getFlagCopiMedoi()
{
    return b_copiaMedoides;
}

bool ClaraProcesor::getFlagCopiDatos()
{
    return b_copiaDatos;
}

bool ClaraProcesor::getFlagRGenerad()
{
    return b_generadorAleatR;
}

bool ClaraProcesor::getFlagInterPam()
{
    return b_intercambioPam;
}

int ClaraProcesor::getTamMuestra(){
    return i_tamanoMuestra;
}

int ClaraProcesor::getNumHilos(){
    return i_numMaxHilos;
}

//      Carga de medoides
QString ClaraProcesor::getPathMedoides(){
    return qstr_pathArchivoMedoides;
}
// Métodos "core"

void ClaraProcesor::asignarHiloColaProteinaGrande(int idHilo){
    //Obtengo el hilo al cual voy a asigar el cliente
    QThread *qtrd_hilo = qtmt_hilosClustering[ idHilo ];
    QStringList qsl_splitedURL = GestorConfiguracion::getInstancia().getConfig()->getQstr_ipRserve().split(":");
    QString ipRserve = qsl_splitedURL.at(0);
    int numPuerto = qsl_splitedURL.at(1).toInt();

    RServClient * rscl_cliente = new RServClient( 0, idHilo, ipRserve, numPuerto );

    //Saco el primer elemento de la cola de proteínas pendientes por procesar
    QString qstr_nomBigProt = qslst_proteinasGigantes.first();
    qslst_proteinasGigantes.removeFirst();
    qstr_nomBigProtsProc.push_back(qstr_nomBigProt);


    pdbh_manejadorProts->leerPDB( qPrintable(
                                      qmap_poolProteinas->value( qstr_nomBigProt ) ) );

    configurarClienteR( rscl_cliente, qtrd_hilo, GRAN_PROTEINA );
    qtrd_hilo->start();
    ++i_numProcsSimult;

    if( !b_flagProcesoGigante ){
        b_flagProcesoGigante = true;
    }

    cout<<"(II) ClaraProcesor(asignarH...Grande): Proteína "<<qPrintable(qstr_nomBigProt);
    cout<<" grande mandada a procesar."<<endl;


}

void ClaraProcesor::procesar(){


    i_numFeatsComunes = i_numClustersProteina*qmap_poolProteinas->size();//Determino número de features por proteína que se obtendran


    qmit_iterPoolProots = qmap_poolProteinas->begin();//Inatancio iterador en el primer path del dataset.

    //Empiezo a instanaciar cada uno de los hilos.
    for( int idThread = 0; idThread < i_numMaxHilos; ++idThread ){
        //Creo hilo nuevo y lo añado a la cola de hilos de proceso
        QThread * qtrd_hiloTmp = new QThread();
        qtmt_hilosClustering.push_back( qtrd_hiloTmp );

        if( qmit_iterPoolProots == qmap_poolProteinas->end() &&
                qslst_proteinasGigantes.isEmpty() ){//Condición de parada
            i_numProcsSimult++;
            cout<<"(II) ClaraProcesor (procesar ()): No hay nada más para procesar"<<endl;
            liberarHilo( idThread );
            break;
        }else {
            reutilizarHilo( idThread );
        }

    }

}

void ClaraProcesor::reutilizarHilo(int idHilo){

    //Verifico si ya la proteína existe
    QString tmpPath = QString("%1/tmp/clara").arg(qstr_pathDirTrabajo);//Path al directorio de trabajo donde se almacenan los resultados temporales
    QString archivoProceso = "";

    ifstream salida;//Flujo requerido para verificar que no se hayan obtenido respuestas previamente
    bool existeSalida = false;//Bandera que evalua si ya hay una salida para el proceso

    //Instancio lo que voy a necesitar y empiezo a reutilizar    
    QStringList qsl_splitedURL = GestorConfiguracion::getInstancia().getConfig()->getQstr_ipRserve().split(":");
    QString ipRserve = qsl_splitedURL.at(0);
    int numPuerto = qsl_splitedURL.at(1).toInt();

    QThread *qtrd_hilo = qtmt_hilosClustering[ idHilo ];//Obtengo el hilo al cual le voy a asignar los parametros
    RServClient * rscl_cliente = new RServClient( 0, idHilo, ipRserve, numPuerto );//Instancio el cliente de R que ejecutará el trabajo


    bool b_hiloAsignado = false;//Bandera para verificar si ya pude asignar los parámetros al hilo
    while( !b_hiloAsignado ){
        /**
          Validaciones generales
        **/
        // Para evitar muchas lecturas por ff y posibles errores solamente procesaré de
        // una proteína gigante a la vez
        //Verifico si ya acabé con el conjunto de proteínas
//        if(qmit_iterPoolProots == qmap_poolProteinas->end()){
//            delete rscl_cliente;


//            if(!qslst_proteinasGigantes.isEmpty()){
//                // hay proteínas grandes pendientes por procesar
//                asignarHiloColaProteinaGrande(idHilo);
//                b_hiloAsignado = true;  // Lo modifico pero verdadera no se utiliza dado el return abajo
//                                        // pues asignar...Grande ya hace start

//            }else {
//                // Si ya llegué al final y no hay proteínas grandes por procesar
//                cout<<"(II) ClaraProcesor( reutilizarHilo ): finalizó con todas las proteínas";
//                cout<<"y no hay nada más que procesar"<<endl;
//            }

//            return;
//        }

        //Verifico que no haya ninguna proteína grande en proceso y que la lista de proteínas grandes no esté vacía.
        //Esto se hizo así para seguir procesando proteínas que sean más manejables en memoria y tiempo
        if( !b_flagProcesoGigante &&
                !qslst_proteinasGigantes.empty() ){
            delete rscl_cliente;

            asignarHiloColaProteinaGrande(idHilo);

            b_flagProcesoGigante = true;
            b_hiloAsignado = true;

            return;
        } else if( qmit_iterPoolProots == qmap_poolProteinas->end() ){
            delete rscl_cliente;
            if(qslst_proteinasGigantes.isEmpty()){
                // Si ya llegué al final y no hay proteínas grandes por procesar
                cout<<"(II) ClaraProcesor( reutilizarHilo ): finalizó con todas las proteínas y no hay nada más que procesar"<<endl;
            }else {
                // Si ya llegué al final y hay proteínas grandes por procesar
                cout<<"(II) ClaraProcesor( reutilizarHilo ): finalizó con todas las proteínas normales, quedan grandes"<<endl;
            }

            return;

        }

        /**
          Validaciones asociadas a la proteína actual
        **/
        // Valido que exista un archivo temporal asociado al proceso de clustering sobre la proteína.
        archivoProceso = tr("%1/%2.tmp.csv").
                arg( tmpPath, qmit_iterPoolProots.key() );
        salida.open( qPrintable( archivoProceso ) );
        existeSalida = salida.good();
        salida.close();

        // Obtengo número de residuos en la proteína para validaciones futuras.
        pdbh_manejadorProts->leerPDB( qPrintable( qmit_iterPoolProots.value() ) );
        int numResiduosProtTmp = pdbh_manejadorProts->getNumResiduos();


        int i_rowsDset = numResiduosProtTmp-i_tamSubmatriz+1;
        i_rowsDset = i_rowsDset*i_rowsDset;

        //Si ya existe un archivo con salidas temporales entonces paso a la siguiente proteína.
        if( existeSalida ){
//            cout<<"(II) ClaraProcesor(ReutilizarHilo):La proteína "<<qPrintable(qmit_iterPoolProots.key())<< " ya se procesó."<<endl;
            emit( proceseProteina( qmit_iterPoolProots.key() ) );
            ++qmit_iterPoolProots;
            qsl_listaArchivosTmp.append(archivoProceso);
            continue;
        }

        //Avanzo si no es una proteína con dimensiones minimas ( tamaño de una dimesión de la submatriz o feature )
        if(numResiduosProtTmp  <= i_tamSubmatriz ){
            cout<<"(II) ClaraProcesor(Reutilizar hilo): ";
            cout<<"Proteína "<<qPrintable(qmit_iterPoolProots.key())<<" más pequeña que el tamaño de la submatriz de distancias (";
            cout<<pdbh_manejadorProts->getNumResiduos()<<" residuos)."<<endl;

            emit(proceseProteina( qmit_iterPoolProots.key() ));

            ++qmit_iterPoolProots;
            continue;
        }

        //Obtengo el número máximo de residuos a procesar en memoria
        int i_maxNumRes = GestorConfiguracion::getInstancia().getConfig()->getI_maxResMem();
        //Verifico si la proteína que voy a procesar requiere ser procesada fuera de memoria
        if(  numResiduosProtTmp >= i_maxNumRes ){

            cout<<"(II) ClaraProcesor(ReutilizarHilo): Proteína "<<qPrintable(qmit_iterPoolProots.key())<<" grande."<<endl;
            //Si la proteína va a procesarse fuera de memoria
            //Decido si puedo procesar o la pongo en cola
            if(!b_flagProcesoGigante){
                //Si no se está procesando una proteína fuera de memoria
                //Configuro e inicio el procesamiento de una proteína fuera de memoria.
                QString qstr_nomBigProt = qmit_iterPoolProots.key() ;

                qstr_nomBigProtsProc.push_back(qstr_nomBigProt);

                configurarClienteR(rscl_cliente, qtrd_hilo, GRAN_PROTEINA);
                cout<<"(II) ClaraProcesor(ReutilizarHilo): Proteína "<<qPrintable(qstr_nomBigProt);
                cout<<" grande mandada a procesar."<<endl;

                b_flagProcesoGigante = true;
                b_hiloAsignado = true;
            }else {
                //Si ya hay una proteína procesando fuera de memoria
                //Coloco la actual en una cola para que sea procesada luego, paso a la siguiente.
                cout<<"(II) ClaraProcesor(ReutilizarHilo): Proteína "<<qPrintable(qmit_iterPoolProots.key());
                cout<<" grande en cola."<<endl;
                qslst_proteinasGigantes.push_back( qmit_iterPoolProots.key() );
                ++qmit_iterPoolProots;
                continue;
            }

        }else {
            //Si no se va a procesar en memoria
            //Configuro un cliente y pongo a correr el procesado
            cout<<"(II) ClaraProcesor(ReutilizarHilo): Proteína normal en proceso"<<qPrintable(qmit_iterPoolProots.key())<<endl;
            configurarClienteR(rscl_cliente, qtrd_hilo, PROTEINA);
            b_hiloAsignado = true;
        }

    }

    qtrd_hilo->start();
    ++i_numProcsSimult;
    //Avanzo a la siguiente proteína
    ++qmit_iterPoolProots;
}

void ClaraProcesor::concatenarSalidasTmp(){

    ofstream flujoSalida;

    ifstream flujoEntrada;
//    char buffer [200];
    string buffer;

    QStringList::iterator itArchivosCluster;


    QString qstr_pathCsv;

    flujoSalida.open( qPrintable( qstr_pathFeatsComunesProteinas ) );

    if( flujoSalida.is_open() ){
        cout<<"(II) ClaraProcesor (concatenarSalidas): Abrió archivo de concatenación "<<qPrintable(qstr_pathFeatsComunesProteinas)<<endl;

        //TODO: Esto va en otro lugar, cuando acabe coloco que archivos lo generaron
//        for( itArchivosCluster = qsl_listaArchivosTmp.begin();
//             itArchivosCluster != qsl_listaArchivosTmp.end();
//             ++itArchivosCluster){

//            qstr_pathCsv = *itArchivosCluster;

//            QString fileExt = qstr_pathCsv.split("/", QString::SkipEmptyParts).last();
//            QString file = fileExt.split(".", QString::SkipEmptyParts).first();

//            flujoSalida<<qPrintable(file);
//            if(itArchivosCluster +1 != qslst_proteinasGigantes.end()){
//                flujoSalida<<"\n";
//            }

//        }

        for( itArchivosCluster = qsl_listaArchivosTmp.begin();
             itArchivosCluster != qsl_listaArchivosTmp.end();
             ++itArchivosCluster){

            qstr_pathCsv = *itArchivosCluster;
            flujoEntrada.open(qPrintable( qstr_pathCsv ));

            if( flujoEntrada.is_open() ){
                cout<<"(II) ClaraProcesor(concatenarSalida): Abrió archivo a concatenar "<<qPrintable(qstr_pathCsv)<<endl;

                while( getline( flujoEntrada, buffer ) ){
                    flujoSalida<<buffer;
                    endl(flujoSalida);
                }

                flujoEntrada.close();

                cout<<"(II) ClaraProcesor(concatenarSalida): Terminó de concatenar "<<qPrintable(qstr_pathCsv)<<endl;
            }else{
                cout<<"(EE) ClaraProcesor ( Concatenando salidas ): El archivo de entrada "<<qPrintable( qstr_pathCsv )<<" no puede ser abierto."<<endl;
                exit(1);
            }

        }

        flujoSalida.close();
    }else{
        cout<<"(EE) ClaraProcesor( Concatenando salidas ): El archivo de salida "<<qPrintable( qstr_pathFeatsComunesProteinas )<<" no puede ser abierto."<<endl;
        exit(1);
    }

}

void ClaraProcesor::salvarFeaturesComunes(){

    QDir qdir_workDir(qstr_pathDirTrabajo);

    bool b_dirExist = qdir_workDir.exists();

    if( !b_dirExist ){
        b_dirExist = qdir_workDir.mkpath( qstr_pathDirTrabajo );
    }

    if( b_dirExist ){
        QString qstr_pathArchivoFeats = tr("%1/features_found_%2.csv").arg( qstr_pathDirTrabajo, QDate::currentDate().toString("dd-MM-yy") );
        QFile qfle_fileFeatures(qstr_pathArchivoFeats);
        QFile qfle_featuresComunes(qstr_pathFeaturesObtenidos);

        bool b_openFeats = qfle_fileFeatures.open(QIODevice::WriteOnly|QIODevice::Text);
        bool b_openRespClust = qfle_featuresComunes.open(QIODevice::ReadOnly|QIODevice::Text);

        if ( b_openFeats && b_openRespClust){
            QTextStream flujoSalida ( &qfle_fileFeatures );
            QTextStream flujoEntrada ( &qfle_featuresComunes );


            QStringList::iterator itArchivosCluster;
            QString qstr_pathCsv;
            for( itArchivosCluster = qsl_listaArchivosTmp.begin();
                 itArchivosCluster != qsl_listaArchivosTmp.end();
                 ++itArchivosCluster){

                qstr_pathCsv = *itArchivosCluster;

                QString fileExt = qstr_pathCsv.split("/", QString::SkipEmptyParts).last();
                QString file = fileExt.split(".", QString::SkipEmptyParts).first();

                cout<<"(II) Salvada el nombre de la proteína "<<qPrintable(file)<<endl;
                flujoSalida<<"P:";
                flujoSalida<<file;
                flujoSalida<<"\n";

            }

            do {
                flujoSalida<<flujoEntrada.readLine()<<"\n";
            }while( !flujoEntrada.atEnd() );

            flujoSalida.flush();
            qfle_fileFeatures.flush();
            qfle_fileFeatures.close();

            flujoEntrada.flush();
            qfle_featuresComunes.flush();
            qfle_featuresComunes.close();

            //TODO: Mirar como lograr que esta señal almiente un singleton

            emit obtuveFeatures(qstr_pathArchivoFeats);

        }else {
            if( b_openFeats ) cout<<"(EE) ClaraProcesor(SalvarFeatures): El archivo donde se guardan los features no se pudo abrir"<<endl;
            if( b_openRespClust ) cout<<"(EE) ClaraProcesor(SalvarFeatures): El archivo temporal de respuesta no pudo ser abierto"<<endl;
        }
    } else {
        cout<<"(EE) ClaraProcesor(SalfavarFeatures): No se pudo crear el foder de trabajo"<<endl;
    }

}

void ClaraProcesor::procesoTerminado(QString nomProceso ){


//    cout<<"ClaraPRoc: Notificó un proceso terminado de: "<<qPrintable(nomProceso)<<endl;
    //Verifico si la proteína procesada pertenece a las proteínas dedicadas.
    if( qstr_nomBigProtsProc.contains( nomProceso ) ){
        qstr_nomBigProtsProc.removeAll( nomProceso );
        cout<<"(II) ClaraProcesor(procesoTerminado): Terminé proteina grande "<<qPrintable(nomProceso)<<endl;
        b_flagProcesoGigante = false;
    }else{
        cout<<"(II) ClaraProcesor(procesoTerminado): Terminé proteína normal "<<qPrintable(nomProceso)<<endl;
    }

    emit proceseProteina( nomProceso );
}

void ClaraProcesor::liberarHilo(int id_Hilo){
    QThread *hiloFinalizado = qtmt_hilosClustering[ id_Hilo ];

    //Finalizo el hilo
    if( hiloFinalizado->isRunning() ){
        hiloFinalizado->quit();

        while(!hiloFinalizado->isFinished()){
            //Loop para garantizar que haya acabado
        }
    }


    delete hiloFinalizado;

    int idColaHilo = id_Hilo;
    int idTmp = -1;
    qtmt_hilosClustering[id_Hilo] = NULL;

//    if(i_numProcsSimult <= i_numMaxHilos){
//      Poner a correr los hilos nuevos hasta que se alcance el maximo de hilos
    /*TODO: Si se cambia la cantidad de hilos en ejecución el vector de hilos se va llenando,
     * corregir para que identifique espacios en la cola de hilos que estén sin utilizar.
     */
    while   (i_numProcsSimult <= i_numMaxHilos){

        QThread * hiloNuevo = new QThread();

        //Valido si el idColaHilo es valido
        if(idColaHilo >= 0){
            //Si me fue provisto un id_Hilo valido
            qtmt_hilosClustering[ idColaHilo] = hiloNuevo ;
            idTmp = idColaHilo;
            idColaHilo = -1;

        } else if( idColaHilo < 0 ){
            //Si tiene un idColaHilo invalido
            qtmt_hilosClustering.push_back(hiloNuevo);
            idTmp = qtmt_hilosClustering.size() -1;
        }


        if( qmit_iterPoolProots != qmap_poolProteinas->end() ){
        //Si estoy intentando procesar proteínas
            reutilizarHilo( idTmp );
        }else if( !qslst_proteinasGigantes.empty() ){
        //Si solamente me queda la cola de proteínas grandes.
            asignarHiloColaProteinaGrande( idTmp );
            break;
        }else if( --i_numProcsSimult == 0 ){
        //Si ya acabé con todo lo que iba a procesar
//                cout<<"LiberarHilo: Voy a ejecutar ultimo proceso"<<endl;
            qtmt_hilosClustering.clear();
            concatenarSalidasTmp();
            cout<<"(II) ClaraProcesor(liberarHilo): Salió del concatenar las salidas temporlaes"<<endl;

            QThread * qtrd_hiloFinal = new QThread();

            //parseo la URL y separo la IP del puerto
            QStringList qsl_splitedURL = GestorConfiguracion::getInstancia().getConfig()->getQstr_ipRserve().split(":");
            QString ipRserve = qsl_splitedURL.at(0);
            int numPuerto = qsl_splitedURL.at(1).toInt();

            RServClient * rscl_clienteFinal = new RServClient( 0, 0, ipRserve, numPuerto );
            configurarClienteR( rscl_clienteFinal, qtrd_hiloFinal, FINAL );

            qtrd_hiloFinal->start();
            qtmt_hilosClustering.push_back( qtrd_hiloFinal );
            cout<<"(II) ClaraProcesor(liberarHilo): Corriendo clustering final"<<endl;

            break;

        } else if( i_numProcsSimult < 0 ){
            salvarFeaturesComunes();
            emit finalizado();
            break;
        }

    }
//      No reutilizar el hilo
    --i_numProcsSimult;


}

void ClaraProcesor::configurarClienteR(RServClient *cliente, QThread *hilo, int tipoProceso){
    //TODO:Cargarle al cliente de R características de la configuración
    //Matriz que se va a cargar
    double ** matrizProt = NULL;
    int tamMatrizProt = 0;
    //Identifico el tipo de matriz
    int i_tipoMat = 0;
    if(qstr_tipoMatriz.compare(tr("Distancias")) == 0){
        i_tipoMat = pdbh_manejadorProts->DIST;
    }else if(qstr_tipoMatriz.compare(tr("Angulos")) == 0){
        i_tipoMat = pdbh_manejadorProts->ANGS;
    }

    //Añado librería principal para CLARA
    cliente->ejecutarComandoSimple(tr("library(cluster);"));

    QString nomRData = NULL;
    QString nomCliente = NULL;
    QString comandoClust;
    int numeroClusters = 0;

    QString stand = "FALSE";
    if( b_datosEstandarizados ){
        stand = "TRUE";
    }

    QString medoids_x = "TRUE";
    if( !b_copiaMedoides ){
        medoids_x = "FALSE";
    }

    QString randR = "FALSE";
    if( b_generadorAleatR ){
        randR = "TRUE";
    }

    QString pamLike = "FALSE";
    if( b_intercambioPam ){
        pamLike = "TRUE";
    }

    QString qstr_numClust;

    int i_numFilasDset = pdbh_manejadorProts->getNumResiduos()-i_tamSubmatriz+1;
    i_numFilasDset = i_numFilasDset*i_numFilasDset;

    QString qstr_numMuestras = QString::number( i_numMuestras );
    QString qstr_tamMuestra = QString::number( i_tamanoMuestra );
    QString qstr_tecnica = tr("\"%1\"").arg( qstr_tipoMetrica );
    QString pathTmpDir = QString("%1/tmp/clara").arg(qstr_pathDirTrabajo);


    switch (tipoProceso) {
    case PROTEINA:
        {
            //Cargo el archivo de proteínas y extraigo las matrices de distancia
            pdbh_manejadorProts->mapearaMatriz( i_tipoMat, d_medidaMax);
            matrizProt = pdbh_manejadorProts->getMatriz( i_tipoMat );
            tamMatrizProt = pdbh_manejadorProts->getTamMatriz( i_tipoMat );

            //Verifico que el tamaño de la muestra no exceda la cantidad de features
//            cout<<"(II) ClaraProcesor("<<qPrintable(qmit_iterPoolProots.key())<<"): Número de residuos: "<<tamMatrizProt<<endl;
//            cout<<"(II) ClaraProcesor("<<qPrintable(qmit_iterPoolProots.key())<<"): Número de elementos: "<<i_numFilasDset<<endl;
//            cout<<"(II) ClaraProcesor("<<qPrintable(qmit_iterPoolProots.key())<<"): Tamaño Muestra "<<i_tamanoMuestra<<endl;

            if( i_numFilasDset < i_tamanoMuestra){

                qstr_tamMuestra = QString::number( i_numFilasDset );
            }

            //Variables de R
            qstr_numClust = QString::number( i_numClustersProteina );
            nomRData = tr("dset_%1").arg(qmit_iterPoolProots.key());
            nomCliente = qmit_iterPoolProots.key();
            comandoClust = qstr_maquetaComando.arg(
                        nomRData,
                        qstr_numClust,
                        qstr_tecnica,
                        stand).arg(
                        qstr_numMuestras,
                        qstr_tamMuestra,
                        medoids_x,
                        medoids_x,
                        randR,
                        pamLike
                        );
            numeroClusters = i_numClustersProteina;
        //Creo archivo temporal para la salida del proceso de clustering
            QString qstr_archivoTmpFeats = QString("%1/%2.tmp.csv")
                    .arg(pathTmpDir, nomCliente );
            qsl_listaArchivosTmp.append(qstr_archivoTmpFeats);
            cliente->setPathArchivoSalida( qstr_archivoTmpFeats );

        //Cargo las submatrices a R
            RParser::parsearMatrizClienteR(matrizProt, tamMatrizProt, tamMatrizProt,
                                           i_tamSubmatriz, i_tamSubmatriz, cliente, nomRData);

            break;
        }
    case GRAN_PROTEINA:
    {

        //Obtengo características de la configuración para librería ff
        long long i_maxffBatchBytes = GestorConfiguracion::getInstancia().getConfig()->getI_maxBytesFFLote();
        long long i_maxffBytes = GestorConfiguracion::getInstancia().getConfig()->getI_maxBytesFF();
        QString qstr_ffTempDir = GestorConfiguracion::getInstancia().getConfig()->getQstr_pathFFTMP();
        //Seteo características de ff
        QString qstr_options = tr("options(ffbatchbytes=%1, ffmaxbytes=%2, fftempdir=\"%3\")").
                arg(QString::number(i_maxffBatchBytes),
                    QString::number(i_maxffBytes),
                    qstr_ffTempDir);
        cliente->ejecutarComandoSimple(qstr_options);

        //Cargo el archivo de proteínas y extraigo las matrices de distancia
        pdbh_manejadorProts->mapearaMatriz( i_tipoMat, d_medidaMax);
        matrizProt = pdbh_manejadorProts->getMatriz( i_tipoMat );
        tamMatrizProt = pdbh_manejadorProts->getTamMatriz( i_tipoMat );
        QString qstr_nomBigProt = qstr_nomBigProtsProc.back();
        qstr_nomBigProtsProc.pop_back();
        qstr_nomBigProtsProc.push_front(qstr_nomBigProt);

        cout<<"(II) ClaraProcesor(Grande: "<<qPrintable(qstr_nomBigProt)<<"): Número de residuos: "<<tamMatrizProt<<endl;
        cout<<"(II) ClaraProcesor(Grande: "<<qPrintable(qstr_nomBigProt)<<"): Número de elementos: "<<i_numFilasDset<<endl;
        cout<<"(II) ClaraProcesor(Grande: "<<qPrintable(qstr_nomBigProt)<<"): Tamaño Muestra "<<i_tamanoMuestra<<endl;


        QFile qf_ffClara(":/rscripts/ffClara");
        //cout<<"(II) ClaraProcesor(configCliente:GranProt): cargar archivo "<<qPrintable(qf_ffClara.readLink())<<endl;

        ifstream scriptR;
        scriptR.open("/tmp/ffClara.R");
        ArchivosUtils::copiarQFile(&qf_ffClara, tr("/tmp/ffClara.R"));
        scriptR.close();

        //cout<<"(II) Claraprocesor(configCliente:GranProt): copió archivo de R al tmp";
        //Cargo las librerías para procesar fuera de memoria
        cliente->cargarLibreriaR(tr("ff"));
        cliente->cargarLibreriaR(tr("ffbase"));
        cliente->cargarLibreriaR(tr("pnmath"));

        cliente->ejecutarComandoSimple(tr("source(file=\"/tmp/ffClara.R\")"));

        QString maquetaFFClara = "clara_res <- ffClara(x=%1, k=%2, metric =%3 , stand = %4, samples = %5, sampsize = %6, medoids.x = %7, keep.data = %8, rngR = %9, pamLike = %10)";

        nomCliente = qstr_nomBigProt;
        qstr_numClust = QString::number( i_numClustersProteina );
        nomRData = tr("dset_%1").arg( nomCliente );
        comandoClust = maquetaFFClara.arg(
                    nomRData,
                    qstr_numClust,
                    qstr_tecnica,
                    stand).arg(
                    qstr_numMuestras,
                    qstr_tamMuestra,
                    medoids_x,
                    medoids_x,
                    randR,
                    pamLike
                    );
        numeroClusters = i_numClustersProteina;
        //Creo archivo temporal para la salida del proceso de clustering
        QString qstr_archivoTmpFeats = QString("%1/%2.tmp.csv")
                .arg(pathTmpDir, nomCliente );
        qsl_listaArchivosTmp.append(qstr_archivoTmpFeats);
        cliente->setPathArchivoSalida( qstr_archivoTmpFeats );

        //Cargo las submatrices a R
        RParser::parsearMatrizClienteR(matrizProt, tamMatrizProt, tamMatrizProt,
                                       i_tamSubmatriz, i_tamSubmatriz, cliente, nomRData);


            break;
        }
    case FINAL:
        {

            qstr_numClust = QString::number( i_numClustersGeneral );
            nomRData = "dataset_features";
            nomCliente = "Final_Cluster";
            // Valido que el tamaño de la muestra sea menor o igual al del dataset
            if( i_tamanoMuestra > i_numFeatsComunes ){
                i_tamanoMuestra = i_numFeatsComunes;
            }
            qstr_tamMuestra = QString::number( i_tamanoMuestra );

            //Armo el comando de clustering a ejecutar
            comandoClust = qstr_maquetaComando.arg(
                        nomRData,
                        qstr_numClust,
                        qstr_tecnica,
                        stand,
                        qstr_numMuestras
                        ).arg(
                        qstr_tamMuestra,
                        medoids_x,
                        medoids_x,
                        randR,
                        pamLike
                        );
            numeroClusters = i_numClustersGeneral;

            qstr_pathFeaturesObtenidos = tr("%1/finalFeatures.csv").arg( QString("%1/tmp").arg(qstr_pathDirTrabajo) );
//            tmfl_finalFeatures = new QTemporaryFile( );
            cliente->setPathArchivoSalida( qstr_pathFeaturesObtenidos );

            //Cargo archivo de los medoides
            QString cargarArchivo = tr( "%1 <- read.csv( file=\"%2\", header = FALSE, sep=\";\", dec = \".\" );" ).arg(
                        nomRData,
                        qstr_pathFeatsComunesProteinas );
            cliente->ejecutarComandoSimple( cargarArchivo );
        break;
        }
    }


    //Meto la configuración efectiva
    cliente->setComandoCluster( comandoClust );
    cliente->setComandoResultado( qstr_resultadoClustering );
    cliente->setNombre(nomCliente);
    cliente->setNomVarCluster( qstr_nomVarMedoids );
    cliente->setNomVarDset( nomRData );
    cliente->setNumClusters( numeroClusters );
    cliente->setTamanoFeature( i_tamFeature );
    cliente->setTamanoDataset( i_numFilasDset );
    cliente->setTipoSalida( RServClient::CSV_OUTPUT );
    cliente->setTipoTecnica( RServClient::Medoid );

    //Conecto las señales del cliente con los slots de la clase
    cliente->moveToThread( hilo );

    //Señales de inicio y fin
    connect( hilo, SIGNAL( started() ), cliente, SLOT( correrTarea() ) );
    connect( hilo, SIGNAL( finished() ), cliente, SLOT( deleteLater() ) );
    connect( hilo, SIGNAL( finished() ), hilo, SLOT(deleteLater()));

    //Señales de procesamiento
    connect( cliente, SIGNAL( termineTarea( QString ) ), this, SLOT( procesoTerminado( QString ) ) );
    connect( cliente, SIGNAL( finalizado(int) ), this, SLOT( liberarHilo(int) ) );
    connect( cliente, SIGNAL( error(QString) ), this, SLOT( errorSlot(QString) ) );

}
