#include "RServClient.h"
#include "libs/rLib/sisockV2.h"
#include <cstdio>
#include <QTextStream>
#include <cmath>//Para que funcione el sqrt

#include <fstream> //Para lectura y escritura de archivos
#include <iostream> //Para lectura y escritura de archivos
#include <control/gestorconfiguracion.h>//Necesario para obtener datos de la configuración

using namespace std;

//Funciones Privadas
int RServClient::sockerrorchecks( char *buf, int blen, int res ){
    *buf = 0;
    if ( res == -1 ){
        switch (sockerrno) {
        case EBADF:
            strncpy( buf, "Descriptor erroneo", blen );         break;
        case EINVAL:
            strncpy( buf, "Ya está en uso", blen );             break;
        case EACCES:
            strncpy( buf, "Acceso denegado", blen );            break;
        case ENOTSOCK:
            strncpy( buf, "El descriptor no es un socket", blen );break;
        case EOPNOTSUPP:
            strncpy( buf, "Operación no soportada", blen );    break;
        case EFAULT:
            strncpy( buf, "fault", blen );                      break;
        case EWOULDBLOCK:
            strncpy( buf, "La operación se bloquearía", blen ); break;
        case EISCONN:
            strncpy( buf, "Ya está conectado", blen );          break;
        case ETIMEDOUT:
            strncpy( buf, "La operación marcó timeout", blen ); break;
        case ENETUNREACH:
            strncpy( buf, "La red es inalcanzable", blen );     break;
        case EADDRINUSE:
            strncpy( buf, "La dirección ya está siendo utilizada", blen );break;
        case EINPROGRESS:
            strncpy( buf, "En progreso", blen );                break;
        case EALREADY:
            strncpy( buf, "Requisito anterior no fue completado", blen );break;
#ifdef unix
        default:
            sprintf( buf, "Error %d socket desconocido", sockerrno );break;
#else
        default: sprintf( buf, "Error $d socket desconocido", sockerrno );
#endif

        }
    }
    return res;
}

//Constructoes/Desctructores
RServClient::RServClient( QObject *parent, int idClient ) :
    QObject(parent){
    this->i_idCliente = idClient;//< Inicializo el Id del servlet
    this->rcon_conexionRsrv = new Rconnection();//< Instancio Objeto de conexión a R
    // Realizo la conexión al servidor de RServe
    int i= rcon_conexionRsrv->connect();
    i_tamDset = 0;
    if( i != 0 ){

        char mensaje[256];
        sockerrorchecks( mensaje, 256, -1 );//< Verifico lo que pasó con el socket
        printf( "(EE) RservClient(RservClient): No fue posible la conexión (Resultado:%d, socket:%s)\n", i, mensaje );

    }

    this->darr_features = NULL; //< Aterrizo el array de features

    i_tipoArchivoSalida = CSV_OUTPUT;

}

RServClient::RServClient(QObject *parent, int idClient, QString ipRserve, int numPuerto) :
    QObject(parent){

    qstr_ipRServe = ipRserve;
    i_numPuerto = numPuerto;

    this->i_idCliente = idClient;//< Inicializo el Id del servlet
    this->rcon_conexionRsrv = new Rconnection(qPrintable(qstr_ipRServe), i_numPuerto);//< Instancio Objeto de conexión a R
    // Realizo la conexión al servidor de RServe
    int i= rcon_conexionRsrv->connect();
    i_tamDset = 0;
    if( i != 0 ){

        char mensaje[256];
        sockerrorchecks( mensaje, 256, -1 );//< Verifico lo que pasó con el socket
        printf( "(EE) RservClient(RservClient): No fue posible la conexión (Resultado:%d, socket:%s)\n", i, mensaje );

    }

    this->darr_features = NULL; //< Aterrizo el array de features

    i_tipoArchivoSalida = CSV_OUTPUT;

}

RServClient::~RServClient(){
    // Limpio el array de features
    if( darr_features != NULL ){
        delete [] darr_features;
        darr_features = 0;
    }

    // Destruyo la conexión


    rcon_conexionRsrv->disconnect();    
    delete rcon_conexionRsrv;
    rcon_conexionRsrv = NULL;
}

//Métodos públicos
//SETTERS - GETTERS

void RServClient::setTamanoFeature(int i_tamFeat){
    this-> i_tamFeature = i_tamFeat;
}

void RServClient::setNumClusters(int i_numClusts){
    this-> i_numClusters = i_numClusts;
}

void RServClient::setTamanoDataset(int tamDset){
    i_tamDset = tamDset;
}

void RServClient::setTipoTecnica( int i_tipoTech ){
    this->i_tipoTecnica = i_tipoTech;
}

int RServClient::getTipoTecnica(){
    return this->i_tipoTecnica;
}

void RServClient::setNombre( QString _nomCliente ){
    this->qstr_nomCliente = _nomCliente;
}

void RServClient::setNomVarDset( QString _nomVarDset ){
    this->qstr_nomVarDatset = _nomVarDset;
}

void RServClient::setPathArchivoSalida(QString _pathSalida)
{
    this->qstr_pathOutputFile = _pathSalida;
}

QString RServClient::getNombreCliente(){
    return qstr_nomCliente;
}

void RServClient::setComandoResultado(QString _comandoResult){
    qstr_comandoResultado = _comandoResult;
}

QString RServClient::getComandoResultado(){
    return qstr_comandoResultado;
}

void RServClient::setNomVarCluster( QString _nomvarCluster ){
    qstr_nomVarCluster = _nomvarCluster;
}

//Métodos configuración

void RServClient::setTipoSalida(int tipoSalida){
    i_tipoArchivoSalida = tipoSalida;
}

int RServClient::getTipoSalida(){
    return i_tipoArchivoSalida;
}

void RServClient::setComandoCluster( QString comando ){
    qstr_comandClust = comando;

}

//void RServClient::setArchivoTmpSalida(QTemporaryFile * p_archivo){
//    this->tmfl_salidaCluster = p_archivo;
//}


void RServClient::cargarLibreriaR(QString nomLib){
    QString qstr_loadRLib( "library(%1);" );

    int res = rcon_conexionRsrv->voidEval( qPrintable( qstr_loadRLib.arg( nomLib ) ) );
    printf( "Al cargar la librería %s se obtuvo %i.\n", qPrintable( nomLib ), res );
}

void RServClient::asignarArray(QString nomVarArray, double array[], int tamArray){
    Rdouble * rd_arrayTmp = new Rdouble( array, tamArray );
    rcon_conexionRsrv->assign( qPrintable( nomVarArray ), rd_arrayTmp );

    delete rd_arrayTmp;//Limpio el puntero
}

void RServClient::borrarVar(QString nomVar){
    QString qstr_borrarVar("rm(%1);");
    int resp = rcon_conexionRsrv->voidEval( qPrintable( qstr_borrarVar.arg( nomVar ) ) );

    printf( "Borrar la variable %s dió como resultado %i\n", qPrintable( nomVar ), resp );
}

void RServClient::ejecutarComandoSimple( QString comando  ){
    int resp = rcon_conexionRsrv->voidEval( qPrintable( comando ) );
    if(resp != 0){
        printf( "(EE) RserveClient(eje..ComandoSimple): La ejecución sin retorno dió como respuesta: %i\n", resp );
    }
}

void RServClient::resetearConexion(){
    //Desconecto y destruyo
    rcon_conexionRsrv->disconnect();
    delete rcon_conexionRsrv;

    //Conecto a R
    rcon_conexionRsrv = new Rconnection();
    rcon_conexionRsrv->connect();

}

//Slots
void RServClient::correrTarea(){
    Rdouble * features = NULL;

    if( i_numClusters <  i_tamDset){
        rcon_conexionRsrv->voidEval( qPrintable( qstr_comandClust ) );
        rcon_conexionRsrv->voidEval( qPrintable( qstr_comandoResultado ) );

        //Preproceso la salida deacuerdo al tipo de algoritmo
        //TODO: Mirar cómo hacerlo más generalizado.
        switch( i_tipoTecnica ){
            case Medoid:
            //Transponer la respuesta
                features = ( Rdouble * ) rcon_conexionRsrv->eval( qPrintable( tr("resp <- t( %1 );").arg(qstr_nomVarCluster) ) );
                break;
        }

        // Borro el dataset adecuadamente
        int i_numResiduos = sqrt(i_tamDset)-1+sqrt(i_tamFeature);
        QString borrarDset = "";

        //Obtengo el número máximo de residuos que soporto en memoria y borro adecuadamente
        int i_maxNumRes = GestorConfiguracion::getInstancia().getConfig()->getI_maxResMem();
        if(i_numResiduos >= i_maxNumRes ){
            borrarDset = QString ( "delete ( %1 );rm ( %1 ); gc()" ).arg(qstr_nomVarDatset);
        }else{
            borrarDset = QString ( "rm ( %1 ); gc()" ).arg(qstr_nomVarDatset);
        }

        rcon_conexionRsrv->voidEval( qPrintable( borrarDset ) );
        // Valido si hubo respuesta del clustering
        if ( features ){
            //Extraigo los features encontrados
            double * puntero_features = features->doubleArray();

            //Inicializo array que los contendrá
            initFeaturesEncontrados();
            int i_tamArray = i_tamFeature * i_numClusters;

            for( int id = 0; id < i_tamArray; ++id){
                darr_features[id] = puntero_features[id];
            }

            escribirSalida(darr_features);

            delete features;
            puntero_features = NULL;

            QString borrarVarSalida("rm ( %1 );");
            rcon_conexionRsrv->voidEval( qPrintable( borrarVarSalida.arg( qstr_nomVarOutput ) ) );

            QString borrarVariableCluster = QString( "rm ( %1 );" ).arg( qstr_nomVarCluster );
            rcon_conexionRsrv->voidEval( qPrintable( borrarVariableCluster ) );
            emit termineTarea(qstr_nomCliente);

        } else {
            cout<<"(EE) RservClient: No se obtuvo features para "<<qPrintable(qstr_nomCliente)<<endl;
            emit error(QString("(EE) RservClient: Error procesando %1").arg(
                           qstr_nomCliente));
        }


    } else {

        features = ( Rdouble * ) rcon_conexionRsrv->eval( qPrintable( tr("resp <- t(%1);").arg(qstr_nomVarDatset) ) );

        if ( features ){
            //Extraigo los features encontrados
            double * puntero_features = features->doubleArray();

            //Inicializo array que los contendrá
            initFeaturesEncontrados();
            int i_tamArray = i_tamFeature * i_tamDset;

            for( int id = 0; id < i_tamArray; ++id){
                darr_features[id] = puntero_features[id];
            }

            escribirSalida(darr_features);

            delete features;
            puntero_features = NULL;

            emit termineTarea(qstr_nomCliente);

        } else {
            cout<<"(EE) RservClient("<<qPrintable(qstr_nomCliente)<<"): Menor numRows a numClusters, no se obtuvo features."<<endl;
            emit error(QString("(EE) RservClient: Error procesando %1").arg(
                           qstr_nomCliente));
        }

        QString borrarDset = QString ( "rm ( %1 );" ).arg(qstr_nomVarDatset);
        rcon_conexionRsrv->voidEval( qPrintable( borrarDset ) );
    }

    emit finalizado ( i_idCliente );
}
int RServClient::getI_numPuerto() const
{
    return i_numPuerto;
}

void RServClient::setI_numPuerto(int value)
{
    i_numPuerto = value;
}

QString RServClient::getQstr_ipRServe() const{
    return qstr_ipRServe;
}

void RServClient::setQstr_ipRServe(const QString &value){
    qstr_ipRServe = value;
}


void RServClient::escribirSalida( double *salidaCluster ){
    int idElemento = 0;
    //    todo: que hutilice files normales
    ofstream flujoSalida;

    flujoSalida.open( qPrintable(qstr_pathOutputFile) );

    if( flujoSalida.is_open() ){

        if(i_numClusters < i_tamDset){
            for(int idRow = 0; idRow < i_numClusters; ++idRow){
                for(int idCol = 0; idCol < i_tamFeature; ++idCol){
                    flujoSalida<<salidaCluster[ idElemento ];
                    if(idCol < i_tamFeature -1){
                        flujoSalida<<";";
                    }
                    ++idElemento;
                }

                flujoSalida<<"\n";
            }
        }else {
            for(int idRow = 0; idRow < i_tamDset; ++idRow){
                for(int idCol = 0; idCol < i_tamFeature; ++idCol){
                    flujoSalida<<salidaCluster[ idElemento ];
                    if(idCol < i_tamFeature -1){
                        flujoSalida<<";";
                    }
                    ++idElemento;
                }

                flujoSalida<<"\n";
            }
        }

        flujoSalida.close();
    }else {
        cout<<"(EE) RservClient("<<qPrintable(qstr_nomCliente)<<"): El archivo "<<qPrintable( qstr_pathOutputFile )<<" no puede ser abierto"<<endl;
    }

}

QString RServClient::ejecutarComandoSalida(QString comando){
    Rstring * r_output =  (Rstring *) rcon_conexionRsrv->eval( qPrintable( comando ) );
    QString output = QString(r_output->string());

    delete r_output;
    return output;
}

//Métodos protegidos
void RServClient::initFeaturesEncontrados(){
    int i_tamArray = 0;

    if(i_tamDset > i_numClusters){
        i_tamArray = i_numClusters * i_tamFeature;
    }else{
        i_tamArray = i_tamFeature * i_tamDset;
    }

    if(darr_features){
        delete darr_features;
        darr_features = NULL;
    }

    darr_features = new double [i_tamArray];
    for ( int id = 0; id < i_tamArray; ++id ){
        darr_features[ id ] = 0;
    }
}
