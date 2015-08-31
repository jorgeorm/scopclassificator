#ifndef RSERVCLIENT_H
#define RSERVCLIENT_H

#include <QObject>
#include <libs/rLib/Rconnection.h>
#include <QTemporaryFile>

/**
 * @brief Clase cliente para RServ, ejecuta comandos de clusters en una conexión  al servidor R.
 */
class RServClient : public QObject
{
    //Funciones privadas
private:
    int sockerrorchecks(char *buf, int blen, int res);

    /**
     * @brief escribirSalida escribe al archivo temporal el array pasado como referencia
     * Teniendo en cuenta el tamaño del feature escribe a un archivo temporal la salida del clustering formateada ( En el caso de los medoides )
     * @param salidaCluster puntero a array de double que contiene la salida de los clusters encontrados
     */
    void escribirSalida(double *salidaCluster);

    Q_OBJECT
public:
    enum ClustTechnique {Medoid, Herarchichal, HybridHierarchical, EM};
    enum TipoSalida {R_OUTPUT, CSV_OUTPUT};
    //Constructores N Destructores
    /**
     * @brief RServClient instancia objeto de la clase con valores por defecto
     * @param parent Objeto padre
     * @param idClient Identificador para el cliente, util cuando se utilizan multiples clientes, por defecto -1
     */
    explicit RServClient(QObject *parent = 0, int idClient = -1);

    /**
     * @brief RServClient instancia objeto de la clase con valores por defecto
     * @param parent Objeto padre
     * @param idClient Identificador para el cliente, util cuando se utilizan multiples clientes, por defecto -1
     * @param ipRserve Dirección IP para el servidor de Rserve
     * @param numPuerto Puerto en el cual el servidor estará escuchando
     */
    explicit RServClient(QObject *parent = 0, int idClient = -1, QString ipRserve="127.0.0.1",int numPuerto=6311);
    /**
     * @brief ~RClient destructor sin parámetros
     * Limpia el objeto para poder liberar memoria
     */
    virtual ~RServClient();

    //Métodos públicos
    //SETTERS - GETTERS
    void setNombre(QString _nomCliente);
    void setNomVarDset(QString _nomVarDset);
    void setPathArchivoSalida(QString _pathSalida);
    void setArchivoTmpSalida (QTemporaryFile * archivo);
    void setNomVarCluster( QString _nomvarCluster );
    void setTamanoFeature( int i_tamFeat );
    void setNumClusters( int i_numClusts );
    void setTamanoDataset ( int tamDset );

    QString getQstr_ipRServe() const;
    void setQstr_ipRServe( const QString &direcIP );
//    void setVarResultClust( QString _resultClust );

//    QString getVarResultClust( );

    /**
     * @brief setTipoTecnica Indica el tipo de técnica de clustering que se correra
     * @param i_tipoTech
     */
    void setTipoTecnica( int i_tipoTech );

    /**
     * @brief getTipoTecnica Retorna el tipo de técnica que se va a correr
     * @return
     */
    int getTipoTecnica();

    QString getNombreCliente();
    /**
     * @brief setComandoResultado Setea el comando de R que permite extraer el resultado del clustering
     * @param _comandoResult <QString> Comando de resultado ej: "varSalida <- clusterResult$medoids"
     */
    void setComandoResultado(QString _comandoResult);
    QString getComandoResultado();


    //Métodos configuración
    /**
     * @brief setComandoCluster Setea el comando de R que será ejecutado en la tarea y la variable que almacenará su resultado
     * @param comando Comando de R que será ejecutado en la tarea principal.
     */
    void setComandoCluster(QString comando );

    /**
     * @brief setTipoSalida Setea el tipo de archivo de salida que se producirá para respaldar la tarea ejecutada.
     * @param tipoSalida <int> Tipo de archivo de salida generada por el hilo al terminar la tarea de clustering, se obtiene a partir del enum TipoSalida
     */
    void setTipoSalida( int tipoSalida = CSV_OUTPUT);

    /**
     * @brief getTipoSalida Obtiene el tipo de salida que generará el algoritmo.
     */
    int getTipoSalida();

    /**
     * @brief cargarLibreriaR Carga una librería al entorno de R.
     * A partir del nombre de la librería  realiza la carga en el entorno de R.
     * @param nomLib Nombre de la librería a cargar en el entorno d eR.
     */
    void cargarLibreriaR(QString nomLib);

    /**
     * @brief asignarArray Asigna a una variable del entorno de R el array de valores double
     * @param nomVarArray Variable a la cual se le va a asignar el array en R.
     * @param array Array de doubles que serán asignados a la variable, es pasado como valor
     * @param tamArray Tamaño del array
     */
    void asignarArray(QString nomVarArray, double array [], int tamArray);

    /**
     * @brief borrarVar Elimina variable del entorno de R, libera memoria
     * Libera memoria del entorno de R
     * @param nomVar
     */
    void borrarVar(QString nomVar);

    /**
     * @brief ejecutarComandoSimple Ejecuta un comando en la instancia de R
     * Permite ejecutar comandos de R sencillos pero necesarios para configurar un entorno para la tarea que corre en el hilo
     * @param comando
     */
    void ejecutarComandoSimple(QString comando);

    /**
     * @brief resetearConexion Reinicia el entorno de R
     * Destruye la conexión y vuelve a conectarse en un entorno nuevo y limpio.
     */
    void resetearConexion();

    /**
     * @brief ejecutarComandoSalida Ejecuta comando en la instancia de R y retorna la respuesta de R como QString
     * @param comando
     * @return QString con el resultado de la evaluación del comando
     */
    QString ejecutarComandoSalida(QString comando);




    int getI_numPuerto() const;
    void setI_numPuerto(int value);

signals:
    /**
     * @brief termineTarea Señal que notifica cuando el clustering ha terminado
     * @param idCliente Identificador del cliente
     * @param resultadoClustering <double*> Puntero a array double con la respuesta del cluster
     * @param nombreTarea Nombre de la tarea ejecutada
     */
    //void termineTarea(int idCliente, double * resultadoClustering, QString nombreTarea);

    /**
     * @brief termineTarea Señal que notifica cuando el clustering ha terminado
     * @param idCliente Identificador del cliente
     * @param nombreTarea Nombre de la tarea ejecutada
     */
    void termineTarea(QString nombreTarea);

    /**
     * @brief error Señal que notifica cuando ocurrió un error interpretable
     * @param qstr_error Descripción del error
     */
    void error(QString qstr_error);

    /**
     * @brief finalizado Señal que notifica cuando todo el proceso sobre el cliente ha terminado.
     * @param idCliente Id del cliente que notifica la terminación de la tarea de Clustering
     */
    void finalizado ( int idCliente );

public slots:
    /**
     * @brief correrTarea Inicia el hiilo encargado de correr la tarea de clustering asignada
     */
    void correrTarea();

protected:
    //Atributos
    double * darr_features = NULL; ///<< double array con los features a procesar
    int i_numClusters, i_tamFeature, i_tamDset; ///<< Numero de clusters a buscar y tamaño del feature del darr_features
    Rconnection * rcon_conexionRsrv; ///<< Conexión a Rserve
    QString qstr_nomCliente;  /// << Identifica el cliente con un nombre, sirve para saber que se está procesando
    QString qstr_comandClust; /// << Comando para hacer clustering en el dataset
    QString qstr_comandoResultado;/// < Comando para obtener el resultado del clustering
    QString qstr_nomVarOutput;/// << Variable de salida donde se almacenara el objeto de R con resultado del clustering
    QString qstr_nomVarCluster;/// << Variable que contiene el cluster encontrado por el algoritmo de clustering
    QString qstr_nomVarDatset;/// << Variable donde se va a almacenar el dataset en R
    int i_idCliente = -1; /// << Identificador del cliente, util cuando se utilizan multiples clientes al tiempo
//    QTemporaryFile * tmfl_salidaCluster; /// << Referencia a archivo temporal donde se almacenará la salida de un clustering
    QString qstr_pathOutputFile;/// << Path a archivo temporal donde se almacenará la salida de un clustering
    int i_tipoTecnica; /// << Entero del enúm en ClustTechnique que indica el tipo de técnia que se va a correr.
    QString qstr_varResultadoTarea; /// << <QString> nombre de la variable que en R tendrá el resultado de la tarea de clustering.

    //Utiles para almacenar y cargar una Routput
    int i_tipoArchivoSalida; /// << <int> Indica que tipo de archivo de salida se generará en el directorio temporal, sale dle enum TipoSalida y por defecto será CSV_OUTPUT
    QString qstr_urlArchivoROutput;/// << <QString> Path al archivo con la salida del algoritmo almacenada en objeto de R
    QString qstr_ipRServe;/// << <QString> Dirección IP al servidor de Rserve
    int i_numPuerto;/// <int> Número del puerto en el cual el servidor está escuchando

    //Métodos protegidos
    /**
     * @brief initFeaturesEncontrados Inicializa array donde se almacena la respuesta del clustering
     */
    void initFeaturesEncontrados();
};

#endif // RSERVCLIENT_H
