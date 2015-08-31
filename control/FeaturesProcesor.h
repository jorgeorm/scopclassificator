#ifndef FEATURESPROCESOR_H
#define FEATURESPROCESOR_H

#include <QString>
#include <QObject>
#include <modelo/pdbhandler.h>
#include <QMap>
#include <QStringList>
#include "control/FootprintCalculator.h"
#include <QThread>

class FeaturesProcesor : public QObject
{
    Q_OBJECT
public:
    //Constructores / Destructores

    explicit FeaturesProcesor(QObject *parent = 0);

    FeaturesProcesor(QObject *parent,
                    int i_numFeatsDom_,
                    int i_numFeatsClass_,
                    double d_medidaMax_,
                    QString str_tipoMat_);

    virtual ~FeaturesProcesor();

    //Setters
    void setNumFeatsDom(int i_numFeatsClass_);
    void setNumFeatsClase(int i_numFeatsDom_);
    void setSubMatSize(int i_featSize_);
    void setTipoMat(QString qstr_tipoMatriz);
    void setPoolProteinas(QMap<QString, QString> * qmap_poolProteinas_);
    void setMaxMed(double max);
    void setFeaturesEncontrados( double * dptr_features, int i_numFeatsC);
    //  Para cargar medoides
    /**
     * @brief setPathArchivoMedoides
     * @param pathMedoides
     */
    void setPathArchivoMedoides( QString pathMedoides );

    /**
     * @brief salvarModelo Guarda el modelo de footprints en una ubicación específica.
     */
    void salvarModelo();

    //Getters
    int getNumFeatsDom();
    int getNumFeatsClase();
    int getFeatSize();
    QString getTipoMat();
    QMap<QString, QString> * getPoolProteinas();
    double getMaxMed();
    double * getFeaturesEncontrados();


    /**
     * @brief parsearMatricesComoArrays parsea a un array de matrices representadas como un vector
     * @param matriz <double **> Puntero a matriz con datos, la cual será preprocesada y almacenada en el entorno de R
     * @param i_rows Número de filas de la matriz original
     * @param i_cols Número de columnas de la matriz original
     * @param i_rowsSub Número de filas en la submatriz a representar el feature
     * @param i_colsSub Número de columnas en la submatriz a representar el feature
     * @return
     */
    static double ** parsearMatricesComoArrays(double ** matriz, int i_rows, int i_cols, int i_rowsSub, int i_colsSub);

signals:
    void proceseProteina( QString qstr_nomProt );
    void finalizado();
    void error( QString msg_error );
    /**
     * @brief calculeFootprintsModelo Señal encargada de notificar cuando se ha generado las footprints para el conjunto de proteínas de entrenamiento.
     * @param qstr_pathArchivoModelo
     */
    void calculeFootprintsModelo ( QString qstr_pathArchivoModelo );



public slots:
    /**
     * @brief procesar ejecuta la tarea de clustering sobre cada de una de las proteínas generando un conjunto de clusters como features comunes.
     */
    virtual void procesar ()  = 0;

    /**
     * @brief errorSlot
     * @param qstr_error
     */
    void errorSlot(QString qstr_error);
    /**
     * @brief calcularFootprints calcula los vectores footprint deacuerdo a los features que se le haya sumistrado a la clase y el algorítmo de clustering utilizado.
     */
    void calcularFootprintsModelo ();

    /**
     * @brief liberarHiloFootprinter Gestiona la liberación de un hilo que ya terminó con su procesamiento
     * @param idHilo
     */
    void liberarHiloFootprinter( int idHilo );

    /**
     * @brief proteinaFootprinted Slot para reportar cuando el footprrinting sobre una proteína ha terminado
     * @param nomProt
     */
    void proteinaFootprinted( QString nomProt );

protected:
    int i_numClustersProteina; /// < <int> Número de features o clusters a encontrar por dominio de proteína
    int i_numClustersGeneral;  /// < <int> Número de features o clusters a encontrar entre los features comunes por dominio de proteína
    int i_tamFeature; /// < <int> Tamaño de cada feature ( Cantidad de elementos en la submatriz )
    int i_tamSubmatriz; /// < <int>Tamaño de una de las submatrices
    int i_numMaxHilos;/// < Número máximo de hilos simultaneos

    double d_medidaMax; /// < <double> Cota superior para medidas, permite fijar especial atención en los que son cercanos o angulos cerrados
    QString qstr_tipoMatriz; /// < <QString> Tipo de matriz a utilizar ej: Distancias o Angulos
    QMap<QString, QString> * qmap_poolProteinas; /// < Puntero a <QMap<QString, QString>> Dataset de proteínas con el que se trabajará.
    bool b_semaforoFootprint;//! Flag de semaforo para paralelismo que cambia de estado cuando un footprinter está reportando resultados
    double ** dmat_footprintsModelo; /// < Puntero a <QMap<QString, double *> Dataset de proteínas>
    int i_numFootprints;/// < <int> Número de Footprints que se van a obtener
    PDBHandler * pdbh_manejadorProts; /// < Puntero a <PDBHandler> Clase util para gestionar los datos formateados de la proteína.
    QStringList listaProteinasBase; /// < Listado de proteínas que generaron los features comunes.
    QString qstr_pathArchivoMedoides; //! < Path a archivo de medoides que se leera
    double ** dmat_caracteristicasComunes; ///! < Matriz con las características comunes encontradas por el algoritmo de clustering
    vector <QThread *> threadList;//! < Lista de hilos que se han puesto a correr, estos son reutilizados o destruidos con forme un hilo reporte haber acabado
    int i_numProcsSimult; //! < Número de procesos que están corriendo simultaneamente
    QMap <QString, QString>:: iterator qmit_iterPoolProots; //! < Iterador del pool de proteínas que se está procesando.
    int key_footprint;//Sirve para indicar el identificador de la footprint en la matriz de footprints
    QStringList::iterator itProteinasBase;
    QString qstr_pathDirTrabajo;//! < Path que se tomará como directorio de trabajo para el algoritmo, allí se guardan datos temporales y soluciones encontradas.


    /**
     * @brief normalizarFootprintsXDimension Normaliza la frecuencia de patrones de interacción locales en las proteínas.
     * Normaliza la matriz de footprints calculando la norma de cada una de las columnas y dividiendo cada elemento de la matriz por dicha norma (de acuerdo a la columna en la que el elemento se encuentre) \f$ A_{pk} = \frac{n(p,k)}{||n(*, k)||} \f$
     */
    void normalizarFootprintsXDimension();

    /**
     * @brief leerArchivoCaracticasComunes Lee las características comunes encontradas durante la etapa de clustering en el método de footprinting
     * @param pathModelos
     */
    void leerArchivoCaracticasComunes();

    private:
    /**
     * @brief initAtributos Inicializa los atributos de la clase
     */
    void initAtributos();

    /**
     * @brief configurarFPC Configura un footprintCalculator para que corra sobre un hilo
     * @param pQThread Hilo sobre el cual correrá el Footprinter
     * @param pFootprinter Footprinter a configurar
     */
    void configurarFPC(QThread * pQThread, FootprintCalculator * pFootprinter);

    /**
     * @brief asignarHiloFootprinter de la cola de hilos que hay corriendo simultaneamente toma el hilo especificado y utiliza para un footprinter
     * @param idHilo
     */
    void asignarHiloFootprinter(int idHilo);

};

#endif // FEATURESPROCESOR_H
