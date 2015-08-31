#ifndef FOOTPRINTCALCULATOR_H
#define FOOTPRINTCALCULATOR_H

#include <QObject>
#include <QSemaphore>
#include "control/Procesors/RServClient.h"

class FootprintCalculator : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief FootprintCalculator Constructor por defecto para la clase FootprintCalculator
     * @param parent QObject parent, en caso de existir.
     */
    explicit FootprintCalculator(QObject *parent = 0);

    /**
     * @brief ~RController Destructor sin parámetros.
     */
    virtual ~FootprintCalculator();

    //SETERS
    /**
     * @brief setFeaturesMedoides Configura la matriz de medoides utilizada en le proceso del calculo de foootprints
     * @param dmat_matrizMedoides referencia a la matriz de medoides
     * @param numRows Número de medoides que tienen la matriz
     * @param numCols Número de features que tiene cada medoide
     */
    void setFeaturesMedoides(double ** dmat_matrizMedoides, int numRows, int numCols);

    /**
     * @brief setFootprints
     * @param dmat_footprints
     */
    void setFootprints(double ** dmat_footprints);

    /**
     * @brief setIdFootprint Setea en qué fila se almacenará la footprint calculada
     * @param idFootprint Indica cual fila será donde se almacenará la footprint calculada
     */
    void setIdFootprint(int idFootprint);

    /**
     * @brief setArraySubmatrices Configura la matriz con las submatrices a utilizar para el calculo del footprint
     * @param dmat_arraySubmatrices referencia a la matriz de doubles
     * @param numSubmats Número de submatrices que contendrá la matriz
     */
    void setArraySubmatrices(double ** dmat_arraySubmatrices, int numSubmats);

    /**
     * @brief setMatrizOriginal
     * @param dmat_matrizBruta
     * @param numRow
     * @param numCols
     */
    void setMatrizOriginal( double ** dmat_matrizProt, int numRow, int numCols);


    /**
     * @brief setTamSubmat
     * @param tamSmat
     */
    void setTamSubmat( int tamSmat );

    /**
     * @brief setNombreHilo
     * @param nomHilo
     */
    void setNombreHilo(QString nomHilo);

    /**
     * @brief setSemaforo
     * @param & flagSemaforo saca la referencia y la almacena en un puntero
     */
    void setSemaforo(bool &flagSemaforo);

    /**
     * @brief setIdHilo Permite setear el identificador del hilo que lo está corriendo
     * @param idHilo
     */
    void setIdHilo(int idHilo);

//    TODO: refactoring de borrarMedoides para ubicarlo en la clase adecuada
    /**
     * @brief borrarMedoides borra los medoides con los que fue configurado
     */
    void borrarMedoides();

    //GETERS
    /**
     * @brief getNumMedoides
     * @return
     */
    int getNumMedoides();

    /**
     * @brief getNumFeatures
     * @return
     */
    int getNumFeatures();

    /**
     * @brief getIdFootprint
     * @return
     */
    int getIdFootprint();

    /**
     * @brief getTipoTecnica
     * @return
     */
    int getTipoTecnica();

    /**
     * @brief getNumSubmatrices
     * @return
     */
    int getNumSubmatrices();

signals:
//    /**
//     * @brief finalice indica cuando el copiado ha sido finalizado
//     */
//    void finalice(FootprintCalculator * pfc_calculador);
    /**
     * @brief footprinted Señal para indicar cuando un hilo está listo para ser desocupado
     * @param idHilo
     */
    void footprinted( int idHilo );

    /**
     * @brief proteinaFootprinted Señal que indica el nombre de la proteína procesada
     * @param nomProt
     */
    void proteinaFootprinted( QString nomProt );



public slots:
    void calcularFootprintMedoides();

protected:
    int i_tipoTecnica; /// < <in> Bandera que indica el tipo de técnica con la cual calculará el footprint
    double ** dmat_features_medoides; ///< Matríz de doubles donde las rows serán los medoides y loas cols serán los elementos que componen ese medoide
    int i_numMedoides; /// < Número de medoides con los que se calculará el footprint
    int i_numFeatures; /// < Número de elementos que componen el medoide
    int i_tamMatrizProt;/// < Tamaño de la matriz de proteína
    double ** dmat_footprints_output; /// Matríz de double donde se almacenará el footprint calculado, depende del i_idFootprint para asignar la respuesta en la ROW correcta.
    int i_idFootprint; /// < Identificador del footprint utilizado para almacenar la respuesta en la matriz.
    double ** dmat_matrizProteina; /// Matriz de doubles donde se tendrán las submatrices de una proteína nada
    int i_numSubmatrices; /// < Número de submatrices que tendra la matriz con las submatrices como arrays
    QString qstr_nomHilo; /// < Nombre de la proteína a la cual se le está calculando el footprint
    int i_tamSubmatFeature;/// < <int>Tamaño de la submatrices o característica en la matriz general
    int idHiloFoot;
    bool *bptr_salidaOcupada;/// Puntero a un booleano que indica si la matriz de salida está siendo ocupada

};

#endif // FOOTPRINTCALCULATOR_H
