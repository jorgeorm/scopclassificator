#ifndef CLARAPROCESOR_H
#define CLARAPROCESOR_H

#include <QObject>
#include <RInside.h>
#include "control/FeaturesProcesor.h"
#include "control/Procesors/RServClient.h"
#include <QThread>
#include <QString>
#include <QStringList>

/**
 * \class ClaraProcesor
 * \brief La clase ClaraProcesor es un controlador para el procesamiento del dataset de proteínas con el algorítmo de clustering CLARA
 * \note El clustering con Rserve se realizó siguiendo el modelo propuesto en [Managing Threads](http://qt-project.org/doc/qt-5.0/qtcore/qthread.html#managing-threads)
 */
class ClaraProcesor : public FeaturesProcesor
{
    Q_OBJECT
public:
    explicit ClaraProcesor(QObject *parent = 0);

    /**
     * @brief ~ClaraProcesor Destructor sin parámetros
     */
    ~ClaraProcesor();

    static QString qstr_maquetaComando;//! < Comando de clara parametrizado clara(x, k, metric = "euclidean", stand = FALSE, samples = 5, sampsize = min(n, 40 + 2 * k), trace = 0, medoids.x = TRUE, keep.data = medoids.x, rngR = FALSE, pamLike = FALSE)
    static QString qstr_resultadoClustering;//! < Comando de clara que extrae los medoides
    static QString qstr_nomVarMedoids;//! < Nombre de la variable que contiene los medoides obtenidos.
    static QString qstr_maquetaComandoFF;//! < Comando de clara parametrizado ffClara(x, k, metric = "euclidean", stand = FALSE, samples = 5, sampsize = min(n, 40 + 2 * k), trace = 0, medoids.x = TRUE, keep.data = medoids.x, rngR = FALSE, pamLike = FALSE)

    //Setters
    //  Configuración CLARA
    /**
     * @brief setNumHilos
     * @param numHilos
     */
    void setNumHilos( int numHilos );

    /**
     * @brief setTamMuestra
     * @param tamMuestra
     */
    void setTamMuestra( int tamMuestra );

    /**
     * @brief setNumMuestras
     * @param numMuestras
     */
    void setNumMuestras( int numMuestras );

    /**
     * @brief setMetrica
     * @param tipoDistancia
     */
    void setMetrica( QString tipoDistancia );

    /**
     * @brief setFlagDatEstand
     * @param flag
     */
    void setFlagDatEstand(bool flag);

    /**
     * @brief setFlagCopiMedoi
     * @param flag
     */
    void setFlagCopiMedoi(bool flag);

    /**
     * @brief setFlagCopiDatos
     * @param flag
     */
    void setFlagCopiDatos(bool flag);

    /**
     * @brief setFlagRGenerad
     * @param flag
     */
    void setFlagRGenerad(bool flag);

    /**
     * @brief setFlagInterPam
     * @param flag
     */
    void setFlagInterPam(bool flag);



    //Getters
    //  Config CLARA
    /**
     * @brief getTamMuestra
     * @return
     */
    int getTamMuestra( );

    /**
     * @brief getNumHilos
     * @return
     */
    int getNumHilos();

    /**
     * @brief getNumMuestras
     * @return
     */
    int getNumMuestras();

    /**
     * @brief getMetrica
     * @return
     */
    QString getMetrica();

    /**
     * @brief getFlagDatEstand
     * @return
     */
    bool getFlagDatEstand();

    /**
     * @brief getFlagCopiMedoi
     * @return
     */
    bool getFlagCopiMedoi();

    /**
     * @brief getFlagCopiDatos
     * @return
     */
    bool getFlagCopiDatos();

    /**
     * @brief getFlagRGenerad
     * @return
     */
    bool getFlagRGenerad();

    /**
     * @brief getFlagInterPam
     * @return
     */
    bool getFlagInterPam();

    //  Carga medoides
    /**
     * @brief getPathMedoides
     * @return
     */
    QString getPathMedoides();


signals:

    /**
     * @brief obtuveFeatures señal encargada de notificar cuando se ha generado un conjunto de features para el footprint
     * @param url_Features URL al archivo con los features
     */
    void obtuveFeatures(QString url_Features);

public slots:

    /**
     * @brief procesar Inicia el procesamiento de las proteínas
     * Inicia con el procesado de las proteínas en paralelo, configura y ejecuta en paraleo clustering de proteínas, estas tareas son controladas a traves de señales y slots que continuan con la ejecución del procesamiento.
     */
    void procesar();

    /**
     * @brief procesoTerminado Slot encargado de verificar si el proceso terminado es una proteína gigante e informar al exterior cuando un proceso ha sido finalizado.
     * @param nomProceso Nombre del proceso que finalizó.
     */
    void procesoTerminado( QString nomProceso );

    /**
     * @brief liberarHilo Se encarga de liberar y reutilizar un hilo una vez un proceso de clustering ha terminado.
     * @param id_Hilo
     */
    void liberarHilo( int id_Hilo );

protected:
    //Configuración CLARA_R
    int i_tamanoMuestra;/// < Tamaño de la muestra para el algoritmo de clara
    int i_numMuestras;/// < Número de muestras que se tomarán
    QString qstr_tipoMetrica;//! Tipo de distancia a utilizar en algoritmo de CLARA {euclidean, manhattan}
    bool b_datosEstandarizados;//! Indica si los valores de X son estandarizados.
    bool b_copiaMedoides;//! Indica si se va a sacar una copia de los medoides de los datos X en la respuesta clara obtenga.
    bool b_copiaDatos;//! Indica si en la respuesta de R se obtiene una copia de los datos
    bool b_generadorAleatR;//! Indica si el generador de aleatorios de R es utilizado
    bool b_intercambioPam;//! Indica si la etapa de intercambio será igual a la de PAM
    //QString qstr_nomBigProt;//! Nombre de la proteína gigante que se está procesando
    QStringList qstr_nomBigProtsProc;//! Nombre de las proteína gigante que se están procesando
    QString qstr_pathFinalFeatures;///Path a archivo temporal donde se almacenarán los features encontrados tras todo el procesamiento de las proteínas.

    //Archivos temporales para salidas
    QString qstr_pathFeatsComunesProteinas, //!< Path a archivo temporal donde se almacenaran los features encontrados en cada proteína.
    qstr_pathFeaturesObtenidos;//!< Path a archivo donde se almacenarán los features encontrados tras todo el procesamiento de proteínas.
//    QTemporaryFile *tmfl_dsetFeatures, //! < Archivo temporal donde se almacenarán los features comunes por proteína.
//    *tmfl_finalFeatures;//! < Archivo temporal donde se almacenarán los features encontrados tras todo el procesamiento de las proteínas.
    QStringList qsl_listaArchivosTmp;//! < Lista de archivos temporales con los features comunes obtenidos del clustering para cada una de las proteinas.


    //Para el procesamiento de proteínas gigantes
    QStringList qslst_proteinasGigantes; //! < Cola de proteínas gigantes, se irá llenando con forme aparezcan y se vaciará conforme se procesen.
    bool b_flagProcesoGigante; //! < Indica cuando se esta procesando una proteína gigante.

    //Para que el procesamiento sea con hilos en paralelo
    int i_numFeatsComunes; //! < Número de features comunes obtenidos tras clustering en dominios de proteína.
    vector <QThread *> qtmt_hilosClustering; //! < Lista de hilos que se han puesto a correr, estos son reutilizados o destruidos con forme un hilo reporte haber acabado


    bool b_flagFootsOcup; //! < Flag para pseudo semaforo con los footprints



    //Enum de control interno
    enum tipoClienteProceso {PROTEINA, GRAN_PROTEINA, FINAL};

    //Métodos adicionales para que todo funcione

    /**
     * @brief configurarClienteR Un RservClient dependiendo del tipo de proceso que se le va a asignar
     * @param cliente Puntero a un RservClient creado el cual será confgurado
     * @param hilo Hilo asociado en el cual correrá.
     * @param tipoProceso Indica si este procesará una proteína, una proteína gigante o hará el clustering de los features deacuerdo al ENUM definido
     */
    void configurarClienteR(RServClient *cliente, QThread *hilo, int tipoProceso);

private:

    /**
     * @brief asignarHiloColaProteinaGrande Toma el hilo provisto y lo asigna para procesar proteína grande de la cola de proteínas
     * @param idHIlo identificador del Hilo en array de hilos corriendo
     */
    void asignarHiloColaProteinaGrande(int idHilo);

    /**
     * @brief initAtributos Inicializa los atributos de la clase con valores por defecto adecuados
     */
    void initAtributos();

    /**
     * @brief reutilizarHilo Se encarga de asignar a un hilo determinado todas las variables necesarias para procesar
     * @param <int> idHilo  Id del hilo en la lista de hilos que se han puesto a correr.
     */
    void reutilizarHilo(int idHilo);

    /**
     * @brief concatenarSalidasTmp Concatena los archivos temporales generados por los clusterings de las proteínas en un solo archivo.
     * Concatena cada uno de los archivos temporales resultantes del clustering sobre cada proteína en un unico archivo para ser cargado a R antes del clustering final
     */
    void concatenarSalidasTmp();

    /**
     * @brief salvarFeaturesComunes
     */
    void salvarFeaturesComunes();


};

#endif // CLARAPROCESOR_H
